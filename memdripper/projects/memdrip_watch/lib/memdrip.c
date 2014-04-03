
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
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "memdrip.h"

#define MAX_LINE_LEN	255
#define SYSFS_MEMDRIP	"/proc/memdrip/"

char string[MAX_LINE_LEN]; 

int read_memdrip_count ( char *string )
{
	int resp = 0;
	int posn = 0;
	int i;
	int j = 0;
	FILE *fp = NULL;
	char file[MAX_LINE_LEN];
	char buffer[MAX_LINE_LEN];
	char str[MAX_LINE_LEN];

	memset( file, 0, sizeof( file ));
	strcpy( file, string );
	
	fp = fopen( file, "r" );
	if( !fp ) {
		printf("error: can't open %s file, %d \n", file, errno );
		return -EINVAL;
	}

	memset( buffer, 0, sizeof( buffer ));	
	fgets( buffer, MAX_LINE_LEN, fp );
	fclose( fp );
	for ( i = 0; i < strlen ( buffer ); i++)
	{
		if ( buffer[i] == '=' )
		{
			posn = i;
		}
	}

	for ( i = posn + 2; i < strlen ( buffer ); i++ )
	{
		str[j] = buffer[i];
		j++;
	}

	resp = atoi( str );	

	return resp ;
}


int read_memdrip_interval ( char *string )
{
	int resp = 0;
	FILE *fp = NULL;
	char file[MAX_LINE_LEN];
	char buffer[MAX_LINE_LEN];

	memset( file, 0, sizeof( file ));
	strcpy( file, string );
	
	fp = fopen( file, "r+" );
	if( !fp ) {
		printf("error: can't open %s file, %d \n", file, errno );
		return -EINVAL;
	}

	memset( buffer, 0, sizeof( buffer ));	
	fgets( buffer, MAX_LINE_LEN, fp );
	fclose ( fp );
	
	resp = atoi ( buffer );
	return resp;
}

int sysfs_get_memdrip ( char *parameter )
{
	int resp = 0;

	memset( string, 0, sizeof( string ));
	strcat( string, SYSFS_MEMDRIP );

	if ( strcmp ( parameter, "count") == 0 )
	{
		strcat( string, "count" );
		resp = read_memdrip_count ( string ); // check the count and return
	}
	else
	{	
		strcat( string, "interval" );
		resp = read_memdrip_interval ( string ); // check the interval and return
	}

	return resp;
}

int write_memdrip( char *string, int interval )
{
	FILE *fp = NULL;
	char file[MAX_LINE_LEN];
	char buffer[MAX_LINE_LEN];

	memset( file, 0, sizeof( file ));	
	strcpy( file, string );
	
	fp = fopen( file, "r+" );
	if( !fp ) {
		printf("error: can't open %s file\n", file );
		return -EINVAL;
	}

	memset( buffer, 0, sizeof( buffer ));
	snprintf( buffer, MAX_LINE_LEN, "%d\n", interval );

	fputs( buffer, fp );
	fclose( fp );

	return 0;
}

int sysfs_set_memdrip( int interval )
{
	memset( string, 0, sizeof( string ));
	strcat( string, SYSFS_MEMDRIP );
	strcat( string, "interval");

	/* Open the thread and set the new interval */
	write_memdrip( string, interval );
	
	return 0;
}

