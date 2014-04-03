/*
 *  memdump.c -  create a "file" in /proc and use for dump of a process
 *
 *  Copyright 2011 Sony Corporation
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  version 2 of the  License.
 *
 *  THIS  SOFTWARE  IS PROVIDED   ``AS  IS'' AND   ANY  EXPRESS OR IMPLIED
 *  WARRANTIES,   INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 *  NO  EVENT  SHALL   THE AUTHOR  BE    LIABLE FOR ANY   DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED   TO, PROCUREMENT OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF
 *  USE, DATA,  OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Suite 500, Boston, MA 02110-1335, USA.
 *
 *  Author: abhilash v r
 */

#include <linux/module.h>	
#include <linux/kernel.h>	
#include <linux/proc_fs.h>	
#include <asm/uaccess.h>	
#include <linux/mm.h>
#include <linux/sched.h>
#include <asm/io.h>
#include <asm/cacheflush.h>
#include <linux/pagemap.h>
#include <linux/init.h>
#include <linux/time.h>
#include <linux/efi.h>
#include "file_write.h"

#define procfs_name 		"memdump"
#define PROCFS_MAX_SIZE		 1024

#define DEBUG
#ifdef DEBUG
#define MEMDUMP_DBG(fmt, ...) printk(fmt, ##__VA_ARGS__)
#else
#define MEMDUMP_DBG(fmt, ...) 
#endif

static char procfs_buffer[PROCFS_MAX_SIZE];
/*
 *  the /proc file structure 
 */
struct proc_dir_entry *memdip_proc_file;
static unsigned long procfs_buffer_size = 0;
static struct task_struct *dump_task = NULL;

/* function invoked during read call to this proc file */
int procfile_read(char *buffer,
	      char **buffer_location,
	      off_t offset, int buffer_length, int *eof, void *data)
{
	int ret;
		
	MEMDUMP_DBG(KERN_INFO "procfile_read (/proc/%s) called\n", procfs_name);
	
	if (offset > 0) {
		/* we have finished to read, return 0 */
		ret  = 0;
	} else {
		/* fill the buffer, return the buffer size */
		ret = sprintf(buffer, "Hi Memdump V 1.0.0 alive!\n");
	}

	return ret;
}

/* function to get the content of user memory */
static struct page *get_dump_page_task(unsigned long addr)
{
        struct vm_area_struct *vma;
        struct page *page;

        if (get_user_pages(dump_task, dump_task->mm, addr, 1,
                        FOLL_FORCE , 1, &page, &vma) < 1)
                return NULL;
        flush_cache_page(vma, addr, page_to_pfn(page));
        return page;
}
/* Get all user pages and call wrapper to write the content to a file */
static int dump_memory(unsigned long start_addr,unsigned long total_page,
				char *seg_name,struct task_struct *task)
{
	unsigned int cnt,page_cnt,row;
	char file_name[128];
	char file_location[]="/tmp/";
	struct timeval tv;

	do_gettimeofday(&tv);
	
	MEMDUMP_DBG("\n1.Segment To dump           : %s\n",seg_name);
	MEMDUMP_DBG("2.Number of Page to dump      : %ld(d)\n",total_page);
	MEMDUMP_DBG("3.Starting Address of Segment : %lx\n",start_addr);
	

	sprintf (file_name,"%s%s_%s_%d_%d",file_location,task->comm,seg_name,(unsigned int)tv.tv_sec,(unsigned int )tv.tv_usec);
	MEMDUMP_DBG("File name  : %s\n",file_name);
	
	for ( page_cnt =0 ;page_cnt < total_page; page_cnt++) {
		struct page *page; 
		dump_task = task;	
		if (page_cnt == 0) {
			open_file(file_name);
		}
		page = get_dump_page_task(start_addr);
		
		if (page) {
			void *kaddr = kmap(page);
			MEMDUMP_DBG("%lx\t",start_addr); 
			for(cnt =0,row=1; (cnt< PAGE_SIZE/4);cnt++,row++) {
				MEMDUMP_DBG("%08lx\t",*((unsigned long *)(kaddr)+cnt));
				if (row%4 == 0) {
					start_addr = start_addr+0x10; 
					MEMDUMP_DBG("\n%lx\t",start_addr);
				}
			}
			write_to_file(kaddr,PAGE_SIZE);
			kunmap(page);
			page_cache_release(page);			
		}

	}
	close_file();
	return 0;
}

/* Function to dump the a task specific segment content */
static void print_mem(struct task_struct *task,char *segment)
{
	struct mm_struct *mm;
	struct vm_area_struct *vma;
	
	unsigned long stack_end=0,stack_start=0,stack_page_cnt=0;	
	unsigned long code_end=0,code_start=0,code_page_cnt=0;	
	unsigned long heap_end=0,heap_start=0,heap_page_cnt=0;	
	unsigned long data_end=0,data_start=0,data_page_cnt=0;	
	unsigned char data_end_flag;
	
	mm = task->mm;
	vma = mm->mmap;
	
	/* Data segment can extend in multiple pages so to get the end of 
	   data segment vma ,using this flag */
	data_end_flag = 1;
	
	/* go through each vma and find the start and end of each segment */	
	for (vma = mm->mmap ; vma ; vma = vma->vm_next) {
                
		MEMDUMP_DBG ("\n* 0x%lx - 0x%lx", vma->vm_start,vma->vm_end);
		
		if (vma->vm_start <= mm->start_stack && vma->vm_end >= mm->start_stack){
			stack_end = vma->vm_end;
			stack_start = vma->vm_start;
			stack_page_cnt = (stack_end - stack_start)/PAGE_SIZE;
		}
		if ( vma->vm_start <= mm->start_code && vma->vm_end >= mm->end_code ) {
			code_end   = vma->vm_end;	
			code_start = vma->vm_start;
			code_page_cnt = (code_end - code_start)/PAGE_SIZE;
		}
		if ( vma->vm_start <= mm->start_brk && vma->vm_end >= mm->brk 
						&& (mm->brk - mm->start_brk) ){
			heap_start = vma->vm_start;
			heap_end   = vma->vm_end;	
			heap_page_cnt = (heap_end - heap_start)/PAGE_SIZE;
		}	
		if (vma->vm_start <= mm->start_data ) {
			data_start = vma->vm_start;
		}	
		if( vma->vm_end >= mm->end_data && data_end_flag ) {
			data_end   = vma->vm_end;
			data_page_cnt = (data_end - data_start)/PAGE_SIZE;
			/* Got the data end ,there may be lot of VMA after this 
			   no need to check data_end in that so set the flag */
			data_end_flag = 0;
		}	
	}	
	/* print starting address of code ,data and stack and heap segment */
	MEMDUMP_DBG("\nCode  Segment start = 0x%lx, end = 0x%lx No:Pages = 0x%ld\n"
			"Data  Segment start = 0x%lx, end = 0x%lx No:Pages = 0x%ld\n"
			"Heap  Segment start = 0x%lx, end = 0x%lx No:Pages = 0x%ld\n"
			"Stack Segment start = 0x%lx, end = 0x%lx No:Pages = 0x%ld\n",
			code_start, code_end,code_page_cnt,
			data_start, data_end,data_page_cnt,
			heap_start, heap_end,heap_page_cnt,
			stack_start,stack_end,stack_page_cnt);
	
	if ( memcmp(segment ,"code",4) == 0) {
		/* dump memory */
		MEMDUMP_DBG("\n -------------- Code Segment ---------------\n");
		dump_memory(code_start,code_page_cnt,segment,task);

	} else if (memcmp (segment ,"data",4 ) == 0) {
		
		MEMDUMP_DBG("\n -------------- Data Segment ---------------\n");
		dump_memory(data_start,data_page_cnt,segment,task);
	
	}
	else if (memcmp (segment ,"heap",4 ) == 0) {
		
		MEMDUMP_DBG("\n -------------- Heap Segment ---------------\n");
		dump_memory(heap_start,heap_page_cnt,segment,task);
	
	}
	else if (memcmp (segment ,"stack",4 ) == 0) {
	
		MEMDUMP_DBG("\n ---------------Stack Segment -------------- \n");
		dump_memory(stack_start,stack_page_cnt,segment,task);
	}
	else {
		MEMDUMP_DBG("\n User input invalid segment to dump :%s\n",segment);
	}
}

/* Function to find out user task from given pid */
static int find_the_user_task(int pid ,char *segment)
{
	struct task_struct *task;
	unsigned short flag=0;

	MEMDUMP_DBG("\nCheck process id present %d.\n", pid);

	/* check user given task present or not */
	for_each_process(task) {
		if ( task->pid == pid) {

			/* print the name and id of the process */
			MEMDUMP_DBG("%s[%d]\n", task->comm, task->pid);
			/* Now call a function to print the process address space */
			print_mem(task,segment);
			flag =1;
			break;
		}
	}
	if(!flag ) {

		MEMDUMP_DBG("\nNo process present in system with PID : %x\n",pid);
		return -1;
	}
	return 0;
}

/* function invoked when writing into this proc file */
int procfile_write(struct file *file, const char *buffer, unsigned long count
			,void *data)
{
	int pid;
	char segment[6];	
	
	/* get user input buffer size */
	procfs_buffer_size = count;
	if (procfs_buffer_size > PROCFS_MAX_SIZE ) {
		procfs_buffer_size = PROCFS_MAX_SIZE;
	}
	
	/* write data to the buffer */
	if ( copy_from_user(procfs_buffer, buffer, procfs_buffer_size) ) {
		MEMDUMP_DBG (KERN_INFO "Error in reading user buffer :Exiting\n");
		return -EFAULT;
	}

	/* get the pid and segment to dump from user */
	sscanf (procfs_buffer,"%d %s",&pid,segment);
	MEMDUMP_DBG (KERN_INFO "pid segment %d %s\n",pid,segment);

	/* find the user task having Process ID pid */	
	find_the_user_task( pid,segment);

	return procfs_buffer_size;
}

/* module init routine */
static int __init memdump_init(void)
{
	/* create a proc entry under /proc/ for user to input pid,segmnet name*/
	memdip_proc_file = create_proc_entry(procfs_name, 0644, NULL);
	
	if (memdip_proc_file == NULL) {
		remove_proc_entry(procfs_name, NULL);
		MEMDUMP_DBG(KERN_ALERT "Error: Could not initialize /proc/%s\n",
				procfs_name);
		return -ENOMEM;
	}

	memdip_proc_file->read_proc = procfile_read;
	memdip_proc_file->write_proc = procfile_write;
	memdip_proc_file->mode 	 = S_IFREG | S_IRUGO | S_IWUGO;
	memdip_proc_file->uid 	 = 0;
	memdip_proc_file->gid 	 = 0;

	MEMDUMP_DBG(KERN_INFO "/proc/%s created\n", procfs_name);	
	return 0;
}

/* module clean up routine */
static void __exit cleanup_mod(void)
{
	remove_proc_entry(procfs_name, NULL);
	MEMDUMP_DBG(KERN_ERR "/proc/%s removed\n", procfs_name);
}
module_init(memdump_init);
module_exit(cleanup_mod);
