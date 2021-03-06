/* 
    File: file_system.H

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 10/04/05

    Description: File System.
    

*/

#ifndef _FILE_SYSTEM_H_                   // include file only once
#define _FILE_SYSTEM_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "utils.H"
#include "simple_disk.H"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARD DECLARATIONS */ 
/*--------------------------------------------------------------------------*/

class FileSystem;

/*--------------------------------------------------------------------------*/
/* F i l e */
/*--------------------------------------------------------------------------*/

class File {

	friend class FileSystem; //needed for LookupFile() to be able to initialize the File object passed
	
private: 
     /* -- DEFINE YOUR FILE DATA STRUCTURES HERE. */
     
     static FileSystem   * file_system;

     unsigned int   file_id;

	 unsigned int size;

	 unsigned int starting_block;


	 unsigned int curr_pos;
	 unsigned int curr_block;


     /* -- You may want to store other information, such as 
             .. position in the file
             .. cached block(s)
             .. etc.
      */

public:

    File();
    /* Constructor for the file handle. Set the 'current
       position' to be at the beginning of the file. */

	File(unsigned int file_id);

    unsigned int Read(unsigned int _n, char * _buf);
    /* Read _n characters from the file starting at the 
       current location and copy them in _buf.
       Return the number of characters read. */

    unsigned int Write(unsigned int _n, char * _buf);
    /* Write _n characters to the file starting at the current 
       location, if we run past the end of file, we increase 
       the size of the file as needed. 
     */

    void Reset();
    /* Set the 'current position' at the beginning of the file. */

    void Rewrite();
    /* Erase the content of the file. Return any freed blocks. 
       Note: This function does not delete the file! It just erases its
       content. */

    BOOLEAN EoF();
    /* Is the current location for the file at the end of the file? */


};

/*--------------------------------------------------------------------------*/
/* F i l e S y s t e m  */
/*--------------------------------------------------------------------------*/

class FileSystem {

friend class File; // file needs to access the methods in file system	

private:
     /* -- DEFINE YOUR FILE SYSTEM DATA STRUCTURES HERE. */
     
     SimpleDisk * disk;
     unsigned int size;

	 unsigned int free_block;

	 unsigned int number_of_files;

	 unsigned int file_data[40][3]; //0 -> file_id, 1 -> size, 2 -> starting_block


	 
public:

   enum {
	   BLOCK_SIZE = 512,
	   FS_BLOCK_LENGTH = 3, //first three blocks reserved for FS
	   FS_DATA_OFFSET = 12, //first 12 bytes reserved for FileSystem data members 'size', 'free_block' and 'number_of_files'
   };
   

   FileSystem();   
   /* Just initializes local data structures. Does not connect to disk yet. */

   ~FileSystem();
   /* Flushes the file_data onto disk*/

   BOOLEAN Mount(SimpleDisk * _disk);
   /* Associates the file system with a disk. We limit ourselves to at most one
      file system per disk. Returns TRUE if 'Mount' operation successful (i.e. there
      is indeed a file system on the disk. */

   static BOOLEAN Format(SimpleDisk * _disk, unsigned int _size);
   /* Wipes any file system from the given disk and installs a new, empty, file
      system that supports up to _size Byte. */

   BOOLEAN LookupFile(int _file_id, File * _file);
   /* Find file with given id in file system. If found, initialize the file 
      object and return TRUE. Otherwise, return FALSE. */

   BOOLEAN CreateFile(int _file_id);
   /* Create file with given id in the file system. If file exists already,
      abort and return FALSE. Otherwise, return TRUE. */

   BOOLEAN DeleteFile(int _file_id);
   /* Delete file with given id in the file system and free any disk block
      occupied by the file. */

   BOOLEAN FlushFile(int _file_id, int _size, int _starting_block);

   
};
#endif
