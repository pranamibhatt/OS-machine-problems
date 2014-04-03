/*
 *  file_write.c -  Writing data to a file from kernel module 
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
 *  author: abhilash v r
 *
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/errno.h>
#include <linux/signal.h>
#include <linux/binfmts.h>
#include <linux/string.h>
#include <linux/file.h>
#include <asm/uaccess.h>
struct file *dump_file;	

static void __write_to_file(struct file *file, void *buf, size_t len)
{
        file->f_op->write(file, buf, len, &file->f_pos);
        if (file->f_op->flush) file->f_op->flush(file, 0);
}
int write_to_file(ulong *buf ,unsigned long size)
{
	mm_segment_t fs;
	fs = get_fs();
        set_fs(KERNEL_DS);
	__write_to_file(dump_file,buf,size);
	set_fs(fs);	
	return 0;
}
EXPORT_SYMBOL( write_to_file );

int open_file(char *fname)
{
	
	dump_file = filp_open(fname, O_CREAT | O_TRUNC, S_IRUGO | S_IWUSR);
	
	if (IS_ERR(dump_file)) {
		printk("Can't create '%s'\n", fname);
		return 0;
	}
	if (!dump_file->f_op->write) {
		filp_close(dump_file, NULL);
		printk("No write op\n");
		return 0;
	}
	return 1;

}
EXPORT_SYMBOL( open_file );

int close_file(void)
{
	if (dump_file){
		filp_close(dump_file, NULL);
		return 0;
	}
	return 1;
}
EXPORT_SYMBOL( close_file );


