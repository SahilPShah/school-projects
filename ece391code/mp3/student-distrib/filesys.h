#ifndef _FILESYS_H
#define _FILESYS_H

#include "types.h"
#include "syscall.h"
#include "lib.h"
#include "rtc.h"

#define USER_LVL        0
#define DIRECTORY       1
#define REGULAR         2
#define MAX_NAME_LEN    32
#define DENTRY_RESERVED 24
#define BOOT_RESERVED   52
#define NUM_DIRECTORIES 63
#define INODE_DATA      1023
#define BLOCK_SIZE      4096

typedef struct dentry_t {
  int8_t file_name[MAX_NAME_LEN];
  int32_t file_type;
  int32_t inode_num;
} dentry_t;

typedef struct inode_t {
  int32_t length;
  int32_t data_block_num[INODE_DATA];
} inode_t;

typedef struct boot_block_t {
  int32_t dir_count;
  int32_t inode_count;
  int32_t data_count;
  dentry_t directories[NUM_DIRECTORIES];
} boot_block_t;

boot_block_t boot_block;
inode_t inode_arr[NUM_DIRECTORIES + 1];

// initializes the file system
void filesys_init(uint32_t filesys_addr);

// copies dentry struct info corresponding to fname into dentry_t argument
int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry);

// copies dentry info into argument struct based on directory index (not inode number)
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);

// fills a file descriptor block, used in syscall.c (sys_open)
int32_t fill_fd_block_by_name(const uint8_t* fname, fd_t* f);

// finds size of a file based on its inode
uint32_t sc_get_file_size(uint32_t inode);

// reads the data stored in a file offset bytes from the beginning of the file
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

// reads from file "filename" and copies bytes read into buf (calls read_data)
int32_t file_read(uint32_t inode_num, void* buf, int32_t nbytes, uint32_t offset);

// read only file system (this does nothing for now)
int32_t file_write(const void* buf, int32_t nbytes);

// performs cleanup after file operations (currently does nothing)
int32_t file_close();

// opens a file by populating a dentry structure
int32_t file_open();

// reads from an entire directory (displays all file names)
int32_t dir_read();

// modifies a directory (read only filesystem, so currently does nothing)
int32_t dir_write(const void* buf, int32_t nbytes);

// closes the directory file by performing cleanup work (currently doesn't need to do anything)
int32_t dir_close();

// opens a directory file (named ".") to read from it
int32_t dir_open(); 


#endif
