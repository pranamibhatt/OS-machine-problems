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
 * Desc  : Reallocating a malloc pointer and not freeing the same  
 */

#include <stdio.h>
#include <stdlib.h>

int realloc_leak ( iteration )
{
	int i;
	char *realloc_ptr ;

	realloc_ptr = malloc(100);
	if (realloc_ptr == NULL)	
	{
		return -1;	
	}
	printf ( "Leaked Realloc ...\n" );
	
	for ( i = 0; i < iteration; i++ )
	{
		realloc_ptr = (char *)realloc(realloc_ptr,100*i);	
	}
	
	// No free of the realloc pointer variable "realloc_ptr" 
	return 0;
}
