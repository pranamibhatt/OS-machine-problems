/******************************************************************************
**	To test the smaps using mmap
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

#define LEN             ( 64 * 1024 )
int main ( void )
{
        int fd = -1;
	unsigned int *kaddr;
	while (1)
	{
		fd = open ( "/dev/mmap_test", O_RDONLY );
		if ( fd < 0 )
		{
			printf ( "Error in open \n" );
			return -1;
		}
		else
		{
			printf ( "Device opened : %d \n", fd );
			kaddr = mmap ( 0, LEN, PROT_READ, MAP_SHARED, fd, 0 );
			if ( kaddr == MAP_FAILED )
			{
				printf ( "mmap failed \n" );
				exit ( -1 );
			}
			else
				printf ( "mmap success \n" ) ;
		}
		close ( fd );
		sleep (2);
	}
        return 0;
}
