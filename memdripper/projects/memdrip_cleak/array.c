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
 * Desc : Leak in an array
 */

#include <stdlib.h>
#include <stdio.h>

#define MAX_ARRAY_SIZE 10
extern int debug_lvl;

int array ( iteration )
{
	int i;
	int *arr [ MAX_ARRAY_SIZE ];
	printf ("Leaked array ...\n");
	for ( i = 0; i < iteration; i++  )
	{
		/* allocated memory for ten int */
		for (i = 0; i < MAX_ARRAY_SIZE; ++i )
		{
        		arr[ i ] = malloc ( sizeof ( int ) + i );
	    	}	   

	   	/* Freed only 9 locations */
		for ( i = 1; i < 10; ++i) 
		{
	        	free ( arr[ i ] );
    		}
		
		// The first element of arr[i] has not been freed
	}
    	return 0;
}

