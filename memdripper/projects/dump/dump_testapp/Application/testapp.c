/******************************************************************************
**	Name : testapp.c
**	This is a sample leaky code
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/include/tmdbuffer.h"
#define MAX_COUNT 50

int main ( void )
{
	char *p1, *p2=NULL;
	int count;

	for ( count = 0; count < MAX_COUNT; count++ )
	{
		p1 = malloc ( 100 );
		strcpy(p1,"leak");
		printf(" string in p1 :%s\n",p1);
		p1 = p2;
		sleep(10);
	}
	return 0;
}
	
