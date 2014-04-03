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
 * Desc : Allocating memory to a global variable multiple times in
 * different functions
 */

#include <stdio.h>
#include <stdlib.h>

char *buffer ;
extern int debug_lvl;

int name1 ( void )
{
	buffer = malloc ( sizeof( char ) * 10 ); 
	return 0;
}

int name2 ( void )
{
	buffer  = malloc ( 80 ); 
	return 0;
}

int global ( iteration )
{
	int i;
	printf ( "Leaked global ...\n" );
	for ( i = 0; i < iteration; i++ )
	{
		name1 ();
		name2 ();
	}
	
	// No free of the global pointer variable "buffer" 
	return 0;
}
