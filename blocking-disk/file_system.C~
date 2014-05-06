/* 
    File: file_system.C

    Author: A. Sood
            Department of Computer Science
            Texas A&M University

    Description: File System.
    

*/


/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "file_system.H"
#include "utils.H"
#include "console.H"

/*--------------------------------------------------------------------------*/
/* F i l e */
/*--------------------------------------------------------------------------*/
FileSystem* File::file_system = 0;

    /* Constructor for the file handle. Set the 'current
       position' to be at the beginning of the file. */
    File::File(unsigned int _file_id) {
    
    	if(_file_id == 0) {
			Console::puts("Incorrect file ID\n");
			for(;;);
    	}
		
    	curr_pos = 0;
		curr_block = 0;
		file_id = _file_id;

		if(file_system->LookupFile(_file_id, this)) {
			Console::puts("Found file with ID, size and starting block: ");
			Console::putui(file_id); Console::putui(size); Console::putui(starting_block);
		}
		else if(file_system->CreateFile(_file_id)) {
			size = 0;
			starting_block = 0;
		}
		else 
			Console::puts("Can't lookup and can't create!!!\n");
	}

    /* Read _n characters from the file starting at the 
       current location and copy them in _buf.
       Return the number of characters read. */
    unsigned int File::Read(unsigned int _n, char * _buf) {
    	if(_n==0 || _buf == NULL || EoF() || size == 0)	return 0;
		Console::puts("Reading...\n");

		char temp_buf[512];
		unsigned int data_read = 0;
		
		while(_n) {

			file_system->disk->read(curr_block, temp_buf);
			
			unsigned int block_offset = curr_pos % 508;
			unsigned int data_count_to_read = (_n > 508 - block_offset) ? 508 - block_offset: _n ; //minimum of _n and available bytes in block and size - curr_pos
			if(data_count_to_read > size - curr_pos)
				data_count_to_read = size - curr_pos;
			memcpy(_buf + data_read, temp_buf + block_offset, data_count_to_read);

			data_read += data_count_to_read;
			_n -= data_count_to_read;
			curr_pos += data_count_to_read;

			if((curr_pos % 508 == 0) && !EoF()) { //need to switch to next existing block
				file_system->disk->read(curr_block, temp_buf);
				memcpy(&curr_block, temp_buf+508, 4);
			}
		}
		
		return data_read;
    }
	
    /* Write _n characters to the file starting at the current 
       location, if we run past the end of file, we increase 
       the size of the file as needed. 
     */
    unsigned int File::Write(unsigned int _n, char * _buf) {
    	if(_n == 0 || _buf == NULL) return 0;
		Console::puts("Writing...\n"); Console::putui(_n);

		char temp_buf[512];
		unsigned int data_written = 0;
		
		while(_n) {
			if(curr_pos % 508 == 0) { //curr block ended, need to switch to next block
				unsigned int block_to_use;
				
				if(EoF()) { //need to add a new block
					block_to_use = file_system->free_block;
					//Console::putui(block_to_use);for(;;);
					if((int)block_to_use == -1) {
						Console::puts("No more free blocks to use!\n");
						for(;;);
					}
					file_system->disk->read(file_system->free_block, temp_buf);
					memcpy(&(file_system->free_block), temp_buf+508, 4);
					

					//recording link to the new block
					if(curr_block != 0) { //not a new file, save new block no. at the end of curr block
						file_system->disk->read(curr_block, temp_buf);
						memcpy(temp_buf+508, &block_to_use, 4);
						file_system->disk->write(curr_block, temp_buf);
					}
					else { //new file, save new block no. in starting block data member
						starting_block = block_to_use;
					}

					//terminating new block
					//memset(temp_buf, 0, 512);
					int last_block_limiter = -1;
					memcpy(temp_buf + 508, &last_block_limiter, 4);
					file_system->disk->write(block_to_use, temp_buf);

					curr_block = block_to_use;
					
				}
			}

			//prepare data to write
			unsigned int block_offset = curr_pos % 508;
			file_system->disk->read(curr_block, temp_buf);
			unsigned int data_count_to_write = (_n > 508 - block_offset) ? 508 - block_offset: _n ; //minimum of _n and available bytes in block
			memcpy(temp_buf + block_offset, _buf + data_written, data_count_to_write);
			file_system->disk->write(curr_block, temp_buf);

			//update size
			if(data_count_to_write + curr_pos > size)
				size = curr_pos + data_count_to_write;
			data_written += data_count_to_write;
			_n -= data_count_to_write;
			curr_pos += data_count_to_write;
			
			if((curr_pos % 508 == 0) && !EoF()) { //need to switch to next existing block
				file_system->disk->read(curr_block, temp_buf);
				memcpy(&curr_block, temp_buf+508, 4);
			}
		}
		
		file_system->FlushFile(file_id, size, starting_block);
		//Console::puts("Starting block is: "); Console::putui(starting_block);
		return data_written;
    }

    /* Set the 'current position' at the beginning of the file. */
    void File::Reset() {
    	curr_pos = 0;
		curr_block = starting_block;
    }

    /* Erase the content of the file. Return any freed blocks. 
       Note: This function does not delete the file! It just erases its
       content. */
    void File::Rewrite() {
    	curr_pos = 0;
		curr_block = 0;

		char temp_buf[512];
		unsigned int block = starting_block; //starting block
		unsigned int next_block = 0;
		while(1) {
			if((int)block == -1) {
				break;
			}
			file_system->disk->read(block, temp_buf);
			memcpy(&next_block, temp_buf+508, 4);
		
			memcpy(temp_buf+508, &(file_system->free_block), 4);
			file_system->disk->write(block, temp_buf);
			file_system->free_block = block;
		
			block = next_block;
		}
		starting_block = 0;
		size = 0;
		file_system->FlushFile(file_id, size, starting_block);
    }

    /* Is the current location for the file at the end of the file? */
    BOOLEAN File::EoF() {
    	return size == curr_pos;
    }



/*--------------------------------------------------------------------------*/
/* F i l e S y s t e m  */
/*--------------------------------------------------------------------------*/

     
   /* Just initializes local data structures. Does not connect to disk yet. */
   FileSystem::FileSystem() {
   		disk = NULL;
		size = 0;
		free_block = 0;
		number_of_files = 0;

		File::file_system = this;
		//memset(file_data, 0, 480);
	}

	FileSystem::~FileSystem() {
		char _buf[512] = {0,};
		
		memcpy(_buf, &size, 4);
		memcpy(_buf+4, &free_block, 4);
		memcpy(_buf+8, &number_of_files, 4);
		memcpy(_buf + FS_DATA_OFFSET, file_data, 480);
		disk->write(0, _buf);
	}

   /* Associates the file system with a disk. We limit ourselves to at most one
      file system per disk. Returns TRUE if 'Mount' operation successful (i.e. there
      is indeed a file system on the disk. */
   BOOLEAN FileSystem::Mount(SimpleDisk * _disk) {
   		Console::puts("Mounting...\n");
   		disk = _disk;
		char _buf[512] = {0,};
		disk->read(0, _buf);
		memcpy(&size, _buf, 4);
		memcpy(&free_block, _buf+4, 4);
		memcpy(&number_of_files, _buf+8, 4);
		memcpy(file_data, _buf + FS_DATA_OFFSET, 480);	//40 File data, each being 3 integers, each being 4 bytes :)
   }

   /* Wipes any file system from the given disk and installs a new, empty, file
      system that supports up to _size Byte. */
   BOOLEAN FileSystem::Format(SimpleDisk * _disk, unsigned int _size) {
   		Console::puts("Formatting...\n");
   		char _buf[512] = {0,};
		
		//initialize the FS reserved blocks except the first block
		for(unsigned int block_no = 1; block_no < FS_BLOCK_LENGTH; block_no++) {
			_disk->write(block_no, _buf);
		}

		//initialize the free blocks (except the last block) assuming _size is a multiple of BLOCK_SIZE
		unsigned int block_no = FS_BLOCK_LENGTH;
   		for(; block_no < _size / BLOCK_SIZE - 1; block_no++) {
			unsigned int next_block = block_no +1;
			memcpy(_buf + 508, &next_block, 4);
			_disk->write(block_no, _buf);
   		}

		//initialize the last block
		int last_block_limiter = -1;
		memcpy(_buf + 508, &last_block_limiter, 4);
		_disk->write(block_no, _buf);
		

		//initialize the first block
		unsigned int temp = _size;
		memcpy(_buf+0, &temp, 4); 				//disk size
		temp = FS_BLOCK_LENGTH; 				//first free block
		memcpy(_buf+4, &temp, 4);
		temp = 0; 								//number of files
		memcpy(_buf+8, &temp, 4);
		memset(_buf+12, 0, 500);				//File data -- necessary to format as file_id == 0 is considered as a hole for filling in file data
		_disk->write(0, _buf);

   }

   /* Find file with given id in file system. If found, initialize the file 
      object and return TRUE. Otherwise, return FALSE. */
   BOOLEAN FileSystem::LookupFile(int _file_id, File * _file) {
   
		if(_file_id == 0 || _file == NULL)	return FALSE;

	   for(unsigned int file_counter = 0; file_counter < 40; file_counter++) { //check all 40 file data due to holes
		   if(file_data[file_counter][0] == _file_id) {//file found
		   	   _file->size = file_data[file_counter][1];
			   _file->starting_block = file_data[file_counter][2];
			   return TRUE;
		   }
	   }
   }

   /* Create file with given id in the file system. If file exists already,
      abort and return FALSE. Otherwise, return TRUE. */
   BOOLEAN FileSystem::CreateFile(int _file_id) {
   		Console::puts("Creating file...\n");
   		char temp_buf[512];

		if(number_of_files > (BLOCK_SIZE - FS_DATA_OFFSET)/12) { // checking whether File data has exceeded the first block, 12 bytes used by each File data ~ 40 files
			//TODO
		}
		else {
			for(unsigned int file_counter = 0; file_counter < 40; file_counter++) { //check all 40 file data due to holes
				if(file_data[file_counter][0] == _file_id) //file exists already
					return FALSE;
			}
		}
		
		unsigned int file_counter;
		for(file_counter = 0; file_counter < 40; file_counter++) { //check all 40 file for a hole
			//Console::putui(file_data[file_counter][0]);
			if(file_data[file_counter][0] == 0) //hole found
				break;
		}
		if(file_counter == 40) { //currently only support for 40 files
			Console::puts("You've reached the max file count\n");
			for(;;);
		}
		else { //hole found, create file now
			file_data[file_counter][0] = _file_id;
			file_data[file_counter][1] = 0; //initial size
			file_data[file_counter][2] = 0; //initial starting block

			number_of_files++;
			return TRUE;
		}
		
   }

   /* Delete file with given id in the file system and free any disk block
      occupied by the file. */
   BOOLEAN FileSystem::DeleteFile(int _file_id) {
   		unsigned int file_counter;
   		for(file_counter = 0; file_counter < 40; file_counter++) {
			if(file_data[file_counter][0] == _file_id)
				break;
   		}
		if(file_counter == 40) return FALSE;

		char temp_buf[512];
		unsigned int block = file_data[file_counter][2]; //starting block
		unsigned int next_block = 0;
		while(1) {
			if((int)block == -1) {
				break;
			}
			disk->read(block, temp_buf);
			memcpy(&next_block, temp_buf+508, 4);

			memcpy(temp_buf+508, &free_block, 4);
			disk->write(block, temp_buf);
			free_block = block;

			block = next_block;
		}

		file_data[file_counter][0] = 0;
		file_data[file_counter][1] = 0;
		file_data[file_counter][2] = 0;

		return TRUE;
   }

   BOOLEAN FileSystem::FlushFile(int _file_id,int _size,int _starting_block) {
   		unsigned int file_counter;
		for(file_counter = 0; file_counter < 40; file_counter++) {
			if(file_data[file_counter][0] == _file_id)
				break;
		}
		if(file_counter == 40) return FALSE;

		file_data[file_counter][1] = _size;
		file_data[file_counter][2] = _starting_block;
		return TRUE;
   }

