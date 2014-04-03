/*
 * Sony CONFIDENTIAL
 *
 * Copyright 2011 Sony Corporation
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
 * Desc : Allocating memory to a temp variable and assigning that to another pointer
 */

#include <stdlib.h>
#include <stdio.h>
#define DEBUG
#define DBG_ALERT (2)
#define DBG_INFO (1)


#define PRINTD( level, ... )	if ( level ==  debug_lvl ) {\
		printf( __VA_ARGS__ ) ;\
		} \
		
extern int debug_lvl;
struct test
{
	int id;
	char *ptr;	
};

int propagation ( iteration )
{
	int i;
	printf ( "Leaked propagation ...\n" );
	for ( i = 0; i < iteration; i++ ) 
	{
		char *temp;	
		struct test *pobj;
		pobj = malloc ( sizeof( struct test ));
		temp = malloc ( 1024 );
		pobj->ptr = temp;

		free ( pobj ); 
	}
	// The temp object has not been freed
	return 0;
}
