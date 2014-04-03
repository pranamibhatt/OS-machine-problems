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
 * Desc : A local pointer not calling free before returning
 */

#include <stdlib.h>
#include <stdio.h>

extern int debug_lvl;

int * allocate ( void )
{
	int *p = malloc ( 10 );
	return (int *) p;
}

int local ( iteration )
{
	int *ptr = NULL;
	printf ("Leaked loccal ...\n");

	int i;
	for ( i = 0; i < iteration ; i++ )
	{
		ptr = allocate ();
	}
	
	// The variable "ptr" has to be ideally freed, 
	// which is NOT and hence there is a leak
	
	return 0;
}

