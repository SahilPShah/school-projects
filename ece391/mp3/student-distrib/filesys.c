#include "filesys.h"
// File system module = 507,904 bytes long -> starts at addr 0x0040F000 and ends at addr 0x004B8000
// Each block in file system is 4 kB long = 4096 bytes, so file system contains 124 blocks
// First block is the boot block (holds file system statistics and directory entries)
// Both stats and each directory entry occupy 64 bytes, so file system can hold up to 63 files
// First dentry always refers to directory itself (has name ".") -> can only really hold 62 files
// Num. dentrys = 0x11 = 17, num. inodes = 0x40 = 64, num. data blocks = 0x3B = 59
// 1 block (boot) + 64 for inodes + 59 for data blocks = 124 blocks

// keep track of the number of inodes and the memory address where data blocks begin
int32_t num_inodes;
int32_t* data_block_start;

/* filesys_init
 * DESCRIPTION: Populates global boot_block with dir_count, inode_count, data_count, and directory entry array information directly  * from filesystem memory, global inode_arr with inode_ptrs.
 * INPUT: starting address of filesystem
 * OUTPUT: none
 * SIDE EFFECT: boot_block, inode_arr are initialized
 */
void filesys_init(uint32_t filesys_addr) {

	int* fs_ptr = (int*)filesys_addr; //filesys_addr determined in kernel.c
	int* inode_ptr = fs_ptr + BLOCK_SIZE / sizeof(int); // 4096 / 4 = 1024, points to start of inodes
	int i, j;

	boot_block.dir_count = *fs_ptr; //dir_count is 1st value in filesystem memory as seen in MP3 documentation
	boot_block.inode_count = *(++fs_ptr); //2nd value in filesystem memory
	num_inodes = boot_block.inode_count;
	boot_block.data_count = *(++fs_ptr); //# of data blocks is 3rd value in filesystem
	fs_ptr += (BOOT_RESERVED / sizeof(int) + 1); //increment fs_ptr to move past 52B of boot_block's reserved blocks

	for(i = 0; i < boot_block.dir_count; i++) { //initialize each dentry_t with file_name, file_type, inode_num from filesystem memory
		strncpy(boot_block.directories[i].file_name, (int8_t*)fs_ptr, MAX_NAME_LEN);
		fs_ptr += (MAX_NAME_LEN / sizeof(int));  //increment fs_ptr past file_name (32B/4B) = 8B
		boot_block.directories[i].file_type = *fs_ptr;
		boot_block.directories[i].inode_num = *(++fs_ptr);
		fs_ptr += (DENTRY_RESERVED / sizeof(int) + 1); //increment fs_ptr to move past 24B of boot_block's reserved blocks
	}

	for(i = 0; i < NUM_DIRECTORIES + 1; i++) { //num_directories refers to largest # of directories filesystem can hold
		//if i is less than actual number of inodes in use, set inode struct length value to value in memory
		if(i < boot_block.inode_count) {
			inode_arr[i].length = *inode_ptr; //length of inode block is first element in inode
			for(j = 1; j <= INODE_DATA; j++)
				inode_arr[i].data_block_num[j - 1] = *(inode_ptr + j);
		}
		//otherwise, set length to -1, and all data blocks to -1
		else {
			inode_arr[i].length = -1;
			for(j = 1; j <= INODE_DATA; j++)
				inode_arr[i].data_block_num[j - 1] = -1;
		}
		inode_ptr += BLOCK_SIZE / sizeof(int); //increment inode_ptr by 1024 to move to next inode in memory
	}
	data_block_start = inode_ptr;
}

/* read_dentry_by_name
 * DESCRIPTION: Populate dentry struct in argument with the file name, file type, and inode number for the file, calls
 * read_dentry_by_index to assign values to dentry_t struct values
 * INPUT: fname: filename, dentry: empty dentry_t struct to be populated after function call
 * OUTPUT: returns 0 if successful, -1 else
 * SIDE EFFECT: dentry argument is populated with file_name, file_type, & inode_num
 */
int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry) {
	uint32_t i;
	for(i = 0; i < boot_block.dir_count; i++)
	{
		if(strncmp(boot_block.directories[i].file_name, (int8_t*)fname, MAX_NAME_LEN) == 0) //if filenames are equal
		{
			//call read_dentry_by_index to populate struct. Eliminates redundancy of populating struct on filename basis
			read_dentry_by_index(i, dentry);
			return 0; //return success after read_dentry_by_index call because file found
		}
	}

	return -1;
}

/* read_dentry_by_index
 * DESCRIPTION: Populate dentry struct in argument with the file name, file type, and inode number for the file. Helper for
 * read_dentry_by_name.
 * INPUT: index: index of file in bootblock directory, dentry: dentry_t struct from read_dentry_by_name call
 * OUTPUT: returns 0 if successful, -1 if not
 * SIDE EFFECT: dentry struct with corresponding index in filesystem is initialized
 */
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry) {
	if(index >= boot_block.dir_count) //if index outside of range of directories in use
	{
		return -1;
	}
	//fill dentry members
	strncpy(dentry->file_name, boot_block.directories[index].file_name, MAX_NAME_LEN);
	dentry->inode_num = boot_block.directories[index].inode_num;
	dentry->file_type = boot_block.directories[index].file_type;
	return 0;
}


/*
fill_fd_block_by_name			FOR SYS_CALLS
Syscall-version of read_dentry_by_name. Used in sys_open, to find if the file exists, and it's index.
Also fills in the appropriate data in the file descriptor block.

Inputs: fname (file name), fd_block
Outputs: none
Returns: 0 on success, -1 on failure
SFX: fills the file descriptor entry based on the filename
*/

int32_t fill_fd_block_by_name(const uint8_t* fname, fd_t *fd_block) {
	if (fd_block == NULL){
		return -1;
	}

	uint32_t i;
	fops_t* fo_table;

	for(i = 0; i < boot_block.dir_count; i++)
	{
		if(strncmp(boot_block.directories[i].file_name, (int8_t*)fname, MAX_NAME_LEN) == 0) //if filenames are equal
		{
			uint8_t file_type = boot_block.directories[i].file_type;
			fo_table = fd_block->fops_table;
			fd_block->flags += (file_type << 1);

			switch(file_type){
				case 0: // rtc clock
					fd_block->inode_num = 0;
					fo_table->read_func = (uint32_t)rtc_read;
					fo_table->write_func = (uint32_t)rtc_write;
					fo_table->open_func = (uint32_t)rtc_open;
					fo_table->close_func = (uint32_t)rtc_close;
					break;
				case 1: // directory
					fd_block->inode_num = 0;
					fo_table->read_func = (uint32_t)dir_read;
					fo_table->write_func = (uint32_t)dir_write;			// cannot write, not allowed to
					fo_table->open_func = (uint32_t)dir_open;		// TODO: do dir/file open/close actually need to be used?
					fo_table->close_func = (uint32_t)dir_close;
					break;
				case 2: // file
					fd_block->inode_num = boot_block.directories[i].inode_num;
					fo_table->read_func = (uint32_t)file_read;
					fo_table->write_func = (uint32_t)file_write;
					fo_table->open_func = (uint32_t)file_open;
					fo_table->close_func = (uint32_t)file_close;
					break;
				default:
					return -1;
					break;
			}
			return 0;
		}
	}
	return -1;
}

/*
*Description: get the size of a file
*Input: the file's inode
*Output: none
*Return: the size of the file
*used in system_read too keep track of how much of a file has been read.
*/
uint32_t sc_get_file_size(uint32_t inode) {
	return inode_arr[inode].length;
}

/* read_data
 * DESCRIPTION: Populate dentry struct in argument with the file name, file type, and inode number for the file
 * INPUT: inode: inode number of file to read from. offset: bytes away from start of file to read from, buf:
 * buffer that will contain data after function call, length: total number of bytes to read
 * OUTPUT: returns number of bytes read from filename, returns -1 if invalid inode or offset,
 					 returns -(bytes read) if could not read nbytes
 * SIDE EFFECT: input buffer buf contains data after call.
 */
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length) {
	if(inode >= num_inodes || inode == 0)
		return -1;
	if(offset >= inode_arr[inode].length) {//check if offset is greater than size of data length (in blocks)
		return 0;
	}
	uint32_t start_data_idx = offset / BLOCK_SIZE;
	uint8_t* starting_addr = (uint8_t*)data_block_start + inode_arr[inode].data_block_num[start_data_idx] * BLOCK_SIZE + (offset % BLOCK_SIZE); //starting address of data in data block
	uint32_t i;
	uint32_t block_cross = 0;
	int32_t bytes_read = 0;
	for(i = 0; i < length; i++)	{
		//if non-zero offset + index causes loop to go out of bounds of file, stop reading
		if(offset + i == (inode_arr[inode].length))
			break;
		//if current index is at boundary of two blocks, set the starting_addr to be at the start of the block we're crossing into
		if(i && ( ((uint32_t)starting_addr + i - (uint32_t)data_block_start) % BLOCK_SIZE) == 0) {
			start_data_idx++; //increment block to start from
			block_cross = i; //save the byte index where a block is crossed
			starting_addr = (uint8_t*)data_block_start + inode_arr[inode].data_block_num[start_data_idx] * BLOCK_SIZE;
		}

		buf[i] = *(starting_addr + (i - block_cross)); //buf[current] = value at current address
		bytes_read++;
	}
	return bytes_read;
}

/* file_read
 * DESCRIPTION: opens file if it's found, and reads data into buffer using read_data
 * INPUT: inode_num: the inode number, retrieved from fd in syscall, buf: buffer to write into, nbytes: number of bytes to read, offset: bytes away from start
 * OUTPUT: returns number of bytes read from file, -1 if file can't be found.
 * SIDE EFFECT: buffer will be filled with data from file
 */
int32_t file_read(uint32_t inode_num, void* buf, int32_t nbytes, uint32_t offset) {
	if(nbytes < 0)
		return -1;
	return read_data(inode_num, offset, buf, nbytes); // return bytes read and populate buf argument
}

/* file_write
 * DESCRIPTION: does not have functionality currently because filesystem is read-only, but would system callwrite to RTC files and   * terminal
 * INPUT: buffer to write into file, number of bytes to write
 * OUTPUT: returns -1
 * SIDE EFFECT: none
 */
int32_t file_write(const void* buf, int32_t nbytes)	{	//read-only fs
	return -1;
}

/* file_close
 * DESCRIPTION: no functionality currently (no cleanup work to do)
 * INPUT: none
 * OUTPUT: returns 0
 * SIDE EFFECT: none
 */
int32_t file_close() {
	return 0;
}

/* file_open
 * DESCRIPTION: called to check if file exists, wrapper for read_dentry_by_name
 * INPUT: filename: self-explanatory, file_dentry: empty dentry struct
 * OUTPUT: returns 0 on dentry read success, -1 on dentry read failure
 * SIDE EFFECT: none
 */
int32_t file_open() {
	return 0;
}

/* dir_read
 * DESCRIPTION: displays all files in the "." directory, including ".", from boot_block
 * INPUT: none
 * OUTPUT: returns 0
 * SIDE EFFECT: prints all filenames to terminal
 */
int32_t dir_read(uint32_t ignore, void* buf, int32_t nbytes, uint32_t offset) {
	dentry_t file; //general purpose struct for read_dentry_by_index calls
	uint32_t num_dentrys = boot_block.dir_count; // # of directories in use
	if(offset >= num_dentrys)
		return 0;
	read_dentry_by_index(offset, &file);
	uint32_t length = strlen(file.file_name); //more readable than strlen

	if(length > MAX_NAME_LEN)
		length = MAX_NAME_LEN;
		
	if(nbytes < length) {
		strncpy((int8_t*)buf, file.file_name, nbytes);
		return nbytes;
	}
	else {
		strncpy((int8_t*)buf, file.file_name, length);
		return length;
	}
}

/* dir_write
 * DESCRIPTION: nothing currently because read-only fs
 * INPUT: buf to write into
 * OUTPUT: returns -1
 * SIDE EFFECT:
 */
int32_t dir_write(const void* buf, int32_t nbytes) {		//read-only fs
	return -1;
}

/* dir_close
 * DESCRIPTION: does nothing as dir_open does
 * INPUT: none
 * OUTPUT: returns 0
 * SIDE EFFECT: none
 */
int32_t dir_close() {
	return 0;
}

/* dir_open
 * DESCRIPTION: copies directory entry for directory itself into the dentry_t argument
 * INPUT: dir_dentry (dentry structure to copy info into)
 * OUTPUT: returns 0
 * SIDE EFFECT: none
 */
int32_t dir_open() {
	return 0;
}
