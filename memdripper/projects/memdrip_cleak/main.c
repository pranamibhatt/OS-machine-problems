/*Sony CONFIDENTIAL
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

   Author : Pranami Bhattacharya
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
//#include <tmdbuffer.h>
#include "main.h"

#undef MEMDUMP

int option, optopt;
char *optarg; 
int test_num, iteration;
int  count, repeat;

int perform_test( int test_num )
{
	switch( test_num ) 
	{
		case 1:
			propagation( iteration );
			break; 

		case 2:
			global ( iteration );
			break;

		case 3: 
			local ( iteration );
			break;

		case 4: 
			array ( iteration );
			break;

		case 5: 
			parent_child ( iteration );
			nested_parent_child( iteration );
			break;

		case 6: 
			reassignment ( iteration );
			break;

		case 7: 
			ret ( iteration );
			break;

		case 8: 
			realloc_leak ( iteration );
			break;

		default: 
			break;
			
	} // End of switch 
	return 0;
}


int main( int argc, char *argv[] )
{
#ifdef MEMDUMP  
        int pid;
        pid = getpid();
        char buffer[100];
        sprintf(buffer,"echo 1 10 > /proc/%d/memdump",pid);
        system(buffer);
#endif

	while (( option = getopt ( argc, argv, "ht:i:r" )) != -1 )
        {
                switch ( option )
                {
                        case 'h' :
                        	usage();
                        	break;
                        case 't' :
				test_num = atoi ( optarg );
				printf ( "The test number is %d \n", test_num );
                        	break;
                        case 'i' :
                        	iteration = atoi ( optarg );
				printf ("The iteration is %d \n", iteration );
				break;

                        case 'r' :
				repeat = 1;
				printf ( "Repeat Option set \n" );
        	                break;
                        case ':' :
                        	fprintf ( stderr, "%s : option -%c required an argument \n",
                                                                argv[0], optopt );
                	        break;

                        default :
                        	break;
                }
        }

	if ( argc == 1 )
		de_fault();
	if ( argc == 2 )
	{
		usage ();
		exit ( 1 );
	}

	// Loops until user presses 'q'
	if( repeat != 1 )
		perform_test( test_num );
	else
	{
		printf ("\n");
		printf ("Looping test \n");
		printf ("Press 'q' to exit \n ");
		//do
		while( 1 ) 
		{
			perform_test ( test_num );	
			sleep( 3 );
		}
		//while ( getchar() != 'q' );	
	}

	return 0;
}

int usage ( void )
{
        printf ("Usage : \n");
        printf ("mcleak < -t -i -r >\n");
        printf ("-t : Test numbers \n");
        printf ("-i : Iterations \n");
        printf ("-r : Repeat \n");
	return 0;

}

int de_fault ( void )
{
	static int i =0;
	printf ( "Opted for default settings \n ");
	printf ( "All 7 leak tests running \n" );
	
	/* example of fogotten memory */
	char *f_ptr = malloc ( 20);
		
	while( 1 )
	{
		printf("\nIteration = %d\n", i++ );
		printf("____________________________\n" );

		propagation ( iteration = 1  );
		global ( iteration = 1  );
		local ( iteration = 1  );
		array ( iteration = 1  );
		parent_child ( iteration = 1  );
		nested_parent_child ( iteration = 1  );
		reassignment ( iteration = 1  );
		ret ( iteration = 1  );
		realloc_leak ( iteration = 1 );
		sleep( 3 );
	}
	return 0;
}  

