/**
 * @file lv_fatfs_sd.c
 * Implementation of RAM file system which do NOT support directories.
 * The API is compatible with the lv_fs_int module. 
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_fatfs_sd.h"
#if USE_LV_FILESYSTEM

#include "lv_ll.h"
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "diskio.h"
#include "ff.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/


/**********************
 *  STATIC VARIABLES
 **********************/
static bool inited = false;

/*
typedef enum
{
    LV_FS_MODE_WR = 0x01,
    LV_FS_MODE_RD = 0x02,
    LV_FS_MODE_CREATE = 0x04,
    LV_FS_MODE_TRUNC = 0x08,
}lv_fs_mode_t;
*/
/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Create a driver for fatfs_sd and initialize it.
 */
void lv_fatfs_sd_init(void)
{
	lv_fs_drv_t fatfs_sd_drv;
	
	if(fsInit()!=NO_ERROR)
		return;
	memset(&fatfs_sd_drv, 0, sizeof(lv_fs_drv_t));    /*Initialization*/
    
	fatfs_sd_drv.file_size = sizeof(lv_fatfs_sd_file_t);
	fatfs_sd_drv.rddir_size = sizeof(lv_fatfs_sd_dir_t);
	fatfs_sd_drv.letter = FATFS_SD_LETTER;
	fatfs_sd_drv.ready = lv_fatfs_sd_ready;
    
	fatfs_sd_drv.open = lv_fatfs_sd_open;
	fatfs_sd_drv.close = lv_fatfs_sd_close;
	fatfs_sd_drv.remove = lv_fatfs_sd_remove;
	fatfs_sd_drv.read = lv_fatfs_sd_read;
	fatfs_sd_drv.write = lv_fatfs_sd_write;
	fatfs_sd_drv.seek = lv_fatfs_sd_seek;
	fatfs_sd_drv.tell = lv_fatfs_sd_tell;
	fatfs_sd_drv.size = lv_fatfs_sd_size;
	fatfs_sd_drv.trunc = lv_fatfs_sd_trunc;
	fatfs_sd_drv.free = lv_fatfs_sd_free;
    
	fatfs_sd_drv.dir_open = lv_fatfs_sd_dir_open;
	fatfs_sd_drv.dir_read = lv_fatfs_sd_dir_read;
	fatfs_sd_drv.dir_close = lv_fatfs_sd_dir_close;
    
	lv_fs_add_drv(&fatfs_sd_drv);
    
	inited = true;
}

/**
 * Give the state of the fatfs_sd
 * @return true if fatfs_sd is initialized and can be used else false
 */
bool lv_fatfs_sd_ready(void)
{
	return inited;
}

/**
 * Open a file in fatfs
 * @param file_p pointer to a lv_fatfs_sd_file_t variable
 * @param fn name of the file. There are no directories so e.g. "myfile.txt"
 * @param mode element of 'fs_mode_t' enum or its 'OR' connection (e.g. FS_MODE_WR | FS_MODE_RD)
 * @return LV_FS_RES_OK: no error, the file is opened
 *         any error from lv__fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_open (void * file_p, const char * fn, lv_fs_mode_t mode)
{
	lv_fatfs_sd_file_t * fp = file_p;    /*Convert type*/
	uint_t fsMode=0;
	char_t name[FS_MAX_NAME_LEN + 1];
	
	if((file_p==NULL)||(fn==NULL))
		return LV_FS_RES_INV_PARAM;
	
	if(mode&LV_FS_MODE_WR)
		fsMode|=FS_FILE_MODE_WRITE;
	if(mode&LV_FS_MODE_RD)
		fsMode|=FS_FILE_MODE_READ;
	if(mode&LV_FS_MODE_CREATE)
		fsMode|=FS_FILE_MODE_CREATE;
	if(mode&LV_FS_MODE_TRUNC)
		fsMode|=FS_FILE_MODE_TRUNC;
	if(strlen(fn)>(FS_MAX_NAME_LEN-3))
		return LV_FS_RES_INV_PARAM;
	memset(name,0,sizeof(name));
	name[0]=SDDISK + '0';
	name[1]=':';
	name[2]='/';
	strcpy(&name[3],fn);
	fp->fd=fsOpenFile(name, fsMode);
	/*If the file not exists ...*/
	if( fp->fd == NULL) { 
		return LV_FS_RES_NOT_EX;       /*Can not read not existing file*/
	}
	return LV_FS_RES_OK;
}


/**
 * Create a file with a constant data
 * @param fn name of the file (directories are not supported)
 * @param const_p pointer to a constant data
 * @param len length of the data pointed by 'const_p' in bytes
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv__fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_create_const(const char * fn, const void * const_p, uint32_t len)
{
	lv_fatfs_sd_file_t file;
	lv_fs_res_t res;
	int write_len=0;
	char_t name[FS_MAX_NAME_LEN + 1];
	
	if((fn==NULL)||(const_p==NULL))
		return LV_FS_RES_INV_PARAM;
	if(strlen(fn)>(FS_MAX_NAME_LEN-3))
		return LV_FS_RES_INV_PARAM;
	memset(name,0,sizeof(name));
	name[0]=SDDISK + '0';
	name[1]=':';
	name[2]='/';
	strcpy(&name[3],fn);
	res = lv_fatfs_sd_open(&file, name, LV_FS_MODE_RD);
	if(res == LV_FS_RES_OK) {
		lv_fatfs_sd_close(&file);
		return LV_FS_RES_DENIED;
	}
    
	lv_fatfs_sd_close(&file);
    
	res = lv_fatfs_sd_open(&file, name, LV_FS_MODE_CREATE);
	if(res != LV_FS_RES_OK){
		return res;
	}
	res=lv_fatfs_sd_write(&file,const_p,len,&write_len);
	if(res != LV_FS_RES_OK){
		lv_fatfs_sd_close(&file);
		return res;
	}
	
	if(write_len!=len){
		lv_fatfs_sd_close(&file);
		return LV_FS_RES_FULL;
	}
	
	res = lv_fatfs_sd_close(&file);
	if(res != LV_FS_RES_OK)
		return res;
	return LV_FS_RES_OK;
}

/**
 * Close an opened file 
 * @param file_p pointer to an 'fatfs_sd_file_t' variable. (opened with lv_fatfs_sd_open) 
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv__fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_close (void * file_p)
{
	lv_fatfs_sd_file_t * fp = file_p;    /*Convert type*/
    	
	if(file_p==NULL)
		return LV_FS_RES_INV_PARAM;
	
	if(fp->fd == NULL)
		return LV_FS_RES_OK;
	fsCloseFile(fp->fd);
	return LV_FS_RES_OK;
}

/**
 * Remove a file. The file can not be opened.
 * @param fn '\0' terminated string
 * @return LV_FS_RES_OK: no error, the file is removed
 *         LV_FS_RES_DENIED: the file was opened, remove failed
 */
lv_fs_res_t lv_fatfs_sd_remove(const char * fn)
{
	char_t name[FS_MAX_NAME_LEN + 1];
	
	if(fn==NULL)
		return LV_FS_RES_INV_PARAM;
	if(strlen(fn)>(FS_MAX_NAME_LEN-3))
		return LV_FS_RES_INV_PARAM;
	memset(name,0,sizeof(name));
	name[0]=SDDISK + '0';
	name[1]=':';
	name[2]='/';
	strcpy(&name[3],fn);
	if(fsDeleteFile(name)!=NO_ERROR){
		return LV_FS_RES_DENIED;	/*File not exists*/
	}
	return LV_FS_RES_OK;
}

/**
 * Read data from an opened file
 * @param file_p pointer to an 'fatfs_sd_file_t' variable. (opened with lv_fatfs_sd_open )
 * @param buf pointer to a memory block where to store the read data
 * @param btr number of Bytes To Read 
 * @param br the real number of read bytes (Byte Read)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv__fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_read (void * file_p, void * buf, uint32_t btr, uint32_t * br)
{
	lv_fatfs_sd_file_t * fp = file_p;    /*Convert type*/
	
	if((file_p==NULL)||(buf==NULL)||(br==NULL))
		return LV_FS_RES_INV_PARAM;
	*br = 0;
	if(fsReadFile(fp->fd, buf, btr,br)!=NO_ERROR)
		return LV_FS_RES_DENIED;   
    	return LV_FS_RES_OK;
}

/**
 * Write data to an opened file
 * @param file_p pointer to an 'fatfs_sd_file_t' variable. (opened with lv_fatfs_sd_open)
 * @param buf pointer to a memory block which content will be written
 * @param btw the number Bytes To Write
 * @param bw The real number of written bytes (Byte Written)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv__fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_write (void * file_p, const void * buf, uint32_t btw, uint32_t * bw)
{
	lv_fatfs_sd_file_t * fp = file_p;    /*Convert type*/
	
	if((file_p==NULL)||(buf==NULL)||(bw==NULL))
		return LV_FS_RES_INV_PARAM;
	*bw = 0;
	if(fsWriteFile(fp->fd, buf,btw)!=NO_ERROR)
		return LV_FS_RES_DENIED; /*Not opened for write*/
	*bw=btw;
	return LV_FS_RES_OK;
}

/**
 * Set the read write pointer. Also expand the file size if necessary.
 * @param file_p pointer to an 'fatfs_sd_file_t' variable. (opened with lv_fatfs_sd_open )
 * @param pos the new position of read write pointer
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv__fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_seek (void * file_p, uint32_t pos)
{    
	lv_fatfs_sd_file_t * fp = file_p;    /*Convert type*/
	
	if(file_p==NULL)
		return LV_FS_RES_INV_PARAM;
	
	if(fsSeekFile(fp->fd, pos, FS_SEEK_SET)!=NO_ERROR)
		return LV_FS_RES_DENIED;
	
	return LV_FS_RES_OK;
}

/**
 * Give the position of the read write pointer
 * @param file_p pointer to an 'fatfs_sd_file_t' variable. (opened with lv_fatfs_sd_open )
 * @param pos_p pointer to to store the result
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv__fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_tell (void * file_p, uint32_t * pos_p)
{
	lv_fatfs_sd_file_t * fp = file_p;    /*Convert type*/
	
    	if(file_p==NULL)
		return LV_FS_RES_INV_PARAM;
	*pos_p = f_tell((FIL *) fp->fd);
    
	return LV_FS_RES_OK;
}

/**
 * Truncate the file size to the current position of the read write pointer
 * @param file_p pointer to an 'fatfs_sd_file_t' variable. (opened with lv_fatfs_sd_open )
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv__fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_trunc (void * file_p)
{
	lv_fatfs_sd_file_t * fp = file_p;    /*Convert type*/
    
	if(file_p==NULL)
		return LV_FS_RES_INV_PARAM;
	if(f_truncate((FIL *) fp->fd)!= FR_OK)
		return LV_FS_RES_DENIED; /*Not opened for write*/
    
	return LV_FS_RES_OK;
}

/**
 * Give the size of the file in bytes
 * @param file_p file_p pointer to an 'fatfs_sd_file_t' variable. (opened with lv_fatfs_sd_open )
 * @param size_p pointer to store the size
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv__fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_size (void * file_p, uint32_t * size_p)
{
	lv_fatfs_sd_file_t * fp = file_p;    /*Convert type*/
    
	if((file_p==NULL)||(size_p==NULL))
		return LV_FS_RES_INV_PARAM;
	*size_p = f_size((FIL *) fp->fd);
    
	return LV_FS_RES_OK;
}

/**
 * Initialize a lv_fatfs_sd_read_dir_t variable to directory reading
 * @param rddir_p pointer to a 'fatfs_sd_dir_t' variable
 * @param path uFS doesn't support folders so it has to be ""
 * @return LV_FS_RES_OK or any error from lv__fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_dir_open(void * rddir_p, const char * path)
{
	lv_fatfs_sd_dir_t * lv_fatfs_sd_rddir_p = rddir_p;
	char_t name[FS_MAX_NAME_LEN + 1];
    
	if((rddir_p==NULL)||(path==NULL))
		return LV_FS_RES_INV_PARAM;
	if(strlen(path)>(FS_MAX_NAME_LEN-3))
		return LV_FS_RES_INV_PARAM;
	memset(name,0,sizeof(name));
	name[0]=SDDISK + '0';
	name[1]=':';
	name[2]='/';
	strcpy(&name[3],path);
	lv_fatfs_sd_rddir_p->dir=fsOpenDir(name);
	if(lv_fatfs_sd_rddir_p->dir)
		return LV_FS_RES_OK;
	return LV_FS_RES_FS_ERR;
}

/**
 * Read the next file name
 * @param dir_p pointer to an initialized 'sd_dir_dir_t' variable
 * @param fn pointer to buffer to sore the file name
 * @return LV_FS_RES_OK or any error from lv__fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_dir_read(void * dir_p, char * fn)
{
	error_t ret;
	lv_fatfs_sd_dir_t * fatfs_sd_dir_p = dir_p;
	
    	if((dir_p==NULL)||(fn==NULL))
		return LV_FS_RES_INV_PARAM;
	
	ret=fsReadDir(fatfs_sd_dir_p->dir, fatfs_sd_dir_p->dirEntry);
	if(ret==ERROR_END_OF_STREAM){
		fn[0] = '\0';
		return LV_FS_RES_OK;
	}else if(ret==NO_ERROR){
		strcpy(fn,fatfs_sd_dir_p->dirEntry->name);
    		return LV_FS_RES_OK;
	}else{
		return LV_FS_RES_FS_ERR;
	}
}

/**
 * Close the directory reading
 * @param rddir_p pointer to an initialized 'fatfs_sd_dir_t' variable
 * @return LV_FS_RES_OK or any error from lv__fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_dir_close(void * rddir_p)
{
	lv_fatfs_sd_dir_t * fatfs_sd_dir_p = rddir_p;
	
    	if(rddir_p==NULL)
		return LV_FS_RES_INV_PARAM;
	
	fsCloseDir(fatfs_sd_dir_p->dir);
	return LV_FS_RES_OK;
}

/**
 * Give the size of a drive
 * @param total_p pointer to store the total size [kB]
 * @param free_p pointer to store the free site [kB]
 * @return LV_FS_RES_OK or any error from 'lv_fs_res_t'
 */
lv_fs_res_t lv_fatfs_sd_free (uint32_t * total_p, uint32_t * free_p)
{
	if((total_p==NULL)||(free_p==NULL))
		return LV_FS_RES_INV_PARAM;
	
	*total_p = 0;    /*Convert bytes to kB*/
	*free_p = 0;
	return LV_FS_RES_OK;
}

#endif /*USE_LV_FILESYSTEM*/

