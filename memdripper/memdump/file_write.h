#ifndef FILE_WRITE_H
#define FILE_WRITE_H

extern int close_file(void);
extern int write_to_file(unsigned long *buf ,unsigned long size);
extern int open_file(char *file_name);

#endif 
