/* Sony CONFIDENTIAL
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
 * Author : Pranami Bhattacharya
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 256

int main ( void )
{
	char parameter[MAX_BUFFER];
	char option;
	int resp = 0;
	int intv = 0;
	printf ( "Enter the parameter \n" );
	gets ( parameter );
		
	if ( strcmp ( parameter, "interval") == 0 )
	{
		resp = sysfs_get_memdrip ( parameter );
		printf ("The interval is %d \n", resp );
		printf ("Do you wish to change the interval : y/n \n");
		scanf ( "%c", &option );
			if ( option == 'y' )
			{
				printf ("Enter the new interval in msec\n");
				scanf ("%d", &intv );
				sysfs_set_memdrip ( intv );
				printf ( "The new interval is %d \n", intv );
			}
			else
			{
				exit (1);
			}
	}

	else
	{
		resp = sysfs_get_memdrip ( parameter );
		printf ( "The count is %d \n", resp );
	}

	return 0;
}

