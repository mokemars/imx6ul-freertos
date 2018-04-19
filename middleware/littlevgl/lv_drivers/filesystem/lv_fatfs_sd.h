#ifndef LV_FATFS_SD_H
#define LV_FATFS_SD_H

#ifdef __cplusplus
extern "C" {
#endif


/*********************
 *      INCLUDES
 *********************/
#include "../../lv_conf.h"

#if USE_LV_FILESYSTEM

#include <stdbool.h>
#include "lv_fs.h"
#include "fs_port.h"

/*********************
 *      DEFINES
 *********************/
#define FATFS_SD_LETTER 'D'

/**********************
 *      TYPEDEFS
 **********************/

/*File descriptor, used to handle opening an entry more times simultaneously 
 Contains unique informations about the specific opening*/
typedef struct
{
	FsFile * fd; /*Pointer to the entry*/
}lv_fatfs_sd_file_t;

/* Read directory descriptor.
 * It is used to to iterate through the entries in a directory*/
typedef struct
{
	FsDir *dir;
	FsDirEntry *dirEntry;
}lv_fatfs_sd_dir_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a driver for fatfs_sd and initialize it.
 */
void lv_fatfs_sd_init(void);

/**
 * Give the state of the fatfs_sd
 * @return true if fatfs_sd is initialized and can be used else false
 */
bool lv_fatfs_sd_ready(void);

/**
 * Open a file in fatfs_sd
 * @param file_p pointer to a lv_fatfs_sd_file_t variable
 * @param fn name of the file. There are no directories so e.g. "myfile.txt"
 * @param mode element of 'fs_mode_t' enum or its 'OR' connection (e.g. FS_MODE_WR | FS_MODE_RD)
 * @return LV_FS_RES_OK: no error, the file is opened
 *         any error from lv_fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_open (void * file_p, const char * fn, lv_fs_mode_t mode);

/**
 * Create a file with a constant data
 * @param fn name of the file (directories are not supported)
 * @param const_p pointer to a constant data
 * @param len length of the data pointed by 'const_p' in bytes
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_create_const(const char * fn, const void * const_p, uint32_t len);

/**
 * Close an opened file
 * @param file_p pointer to an 'fatfs_sd_file_t' variable. (opened with lv_fatfs_sd_open)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_close (void * file_p);

/**
 * Remove a file. The file can not be opened.
 * @param fn '\0' terminated string
 * @return LV_FS_RES_OK: no error, the file is removed
 *         LV_FS_RES_DENIED: the file was opened, remove failed
 */
lv_fs_res_t lv_fatfs_sd_remove(const char * fn);

/**
 * Read data from an opened file
 * @param file_p pointer to an 'fatfs_sd_file_t' variable. (opened with lv_fatfs_sd_open )
 * @param buf pointer to a memory block where to store the read data
 * @param btr number of Bytes To Read
 * @param br the real number of read bytes (Byte Read)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_read (void * file_p, void * buf, uint32_t btr, uint32_t * br);

/**
 * Write data to an opened file
 * @param file_p pointer to an 'fatfs_sd_file_t' variable. (opened with lv_fatfs_sd_open)
 * @param buf pointer to a memory block which content will be written
 * @param btw the number Bytes To Write
 * @param bw The real number of written bytes (Byte Written)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_write (void * file_p, const void * buf, uint32_t btw, uint32_t * bw);

/**
 * Set the read write pointer. Also expand the file size if necessary.
 * @param file_p pointer to an 'fatfs_sd_file_t' variable. (opened with lv_fatfs_sd_open )
 * @param pos the new position of read write pointer
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_seek (void * file_p, uint32_t pos);

/**
 * Give the position of the read write pointer
 * @param file_p pointer to an 'fatfs_sd_file_t' variable. (opened with lv_fatfs_sd_open )
 * @param pos_p pointer to to store the result
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_tell (void * file_p, uint32_t * pos_p);

/**
 * Truncate the file size to the current position of the read write pointer
 * @param file_p pointer to an 'fatfs_sd_file_t' variable. (opened with lv_fatfs_sd_open )
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_trunc (void * file_p);

/**
 * Give the size of the file in bytes
 * @param file_p file_p pointer to an 'fatfs_sd_file_t' variable. (opened with lv_fatfs_sd_open )
 * @param size_p pointer to store the size
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_size (void * file_p, uint32_t * size_p);

/**
 * Initialize a lv_fatfs_sd_read_dir_t variable to directory reading
 * @param rddir_p pointer to a 'fatfs_sd_read_dir_t' variable
 * @param path uFS doesn't support folders so it has to be ""
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_dir_open(void * rddir_p, const char * path);

/**
 * Read the next file name
 * @param dir_p pointer to an initialized 'fatfs_sd_read_dir_t' variable
 * @param fn pointer to buffer to sore the file name
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_dir_read(void * dir_p, char * fn);

/**
 * Close the directory reading
 * @param rddir_p pointer to an initialized 'fatfs_sd_read_dir_t' variable
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
lv_fs_res_t lv_fatfs_sd_dir_close(void * rddir_p);

/**
 * Give the size of a drive
 * @param total_p pointer to store the total size [kB]
 * @param free_p pointer to store the free site [kB]
 * @return LV_FS_RES_OK or any error from 'fs_res_t'
 */
lv_fs_res_t lv_fatfs_sd_free (uint32_t * total_p, uint32_t * free_p);

/**********************
 *      MACROS
 **********************/

#endif /*USE_LV_FILESYSTEM*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
