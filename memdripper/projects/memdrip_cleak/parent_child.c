/*
 * Sony CONFIDENTIAL
 *
 * Copyright 2012 Sony Corporation
 *
 * DO NOT COPY AND/OR REDISTRIBUTE WITHOUT PERMISSION.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *
 * Author: Pranami Bhattacharya
 * Desc : This example shows parent node freed before child node 
 */
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct lattice { 
	int a; 
	char * ptr; 
};

struct lattice *ptr_obj;

extern int debug_lvl;

int parent_child ( iteration )
{
	int i;
	printf ("Leaked parent-child ...\n");
	for ( i = 0; i < iteration ; i++ )
	{
	        ptr_obj = malloc ( sizeof ( struct lattice ));
	        ptr_obj->ptr = malloc ( 10 );

	        free ( ptr_obj );
		// No free for member pointer "ptr_obj->ptr" 
	}
        return 0;
}


struct record { 
	int id;
	char *name;
};

struct employee {
	struct record *emp_record;
	int type;
};

struct department {
	struct employee *demp;
	int bu;
};

enum EMP
{
	PS = 501,
	AC
};

enum BU
{
	GSC = 200,
	GDC
};

int nested_parent_child( iteration ) 
{
	struct department *linux_odc = NULL;
	
	printf ("Leaked nested parent-child ...\n");
	linux_odc = malloc( sizeof( struct department ));
	if( linux_odc ) 
	{
		linux_odc->bu  = GSC;
		linux_odc->demp = malloc( sizeof( struct employee ));
		if( linux_odc->demp != NULL ) 
		{
			linux_odc->demp->type = PS;
			linux_odc->demp->emp_record = malloc( sizeof( struct record ));	
			if( linux_odc->demp->emp_record != NULL ) 
			{
				linux_odc->demp->emp_record->id = 0x12345678;
				linux_odc->demp->emp_record->name = malloc( sizeof( 20 ));
				strcpy( linux_odc->demp->emp_record->name, "MCA12" );
			}
		}
	}
#if 0	
	printf("Employee Details are: \n");
	printf("Name: %s\n", linux_odc->demp->emp_record->name );
	printf("ID: 0x%x\n", linux_odc->demp->emp_record->id );
	printf("Dept Type: %d\n", linux_odc->demp->type );
	printf("BU : %d\n", linux_odc->bu );
#endif
	/* Nested Freeing is Required */
	free( linux_odc->demp->emp_record->name );
	free( linux_odc->demp->emp_record );
	free( linux_odc->demp );
	free( linux_odc );
	
	return 0;
}


