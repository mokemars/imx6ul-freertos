/**
 * @file main.c
 * @brief Resource compiler
 *
 * @section License
 *
 * Copyright (C) 2010-2017 Oryx Embedded SARL. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.8.0
 **/

//Disable compiler warning
#define _CRT_SECURE_NO_WARNINGS

//Dependencies
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
   #include <direct.h>
   #include <io.h>
#else
   #include <unistd.h>
   #include <dirent.h>
   #include <libgen.h>
#endif

//Maximum path size
#ifndef PATH_MAX
   #define PATH_MAX 256
#endif

//Error codes
#define NO_ERROR               0
#define ERROR_FAILURE          -1
#define ERROR_FILE_TOO_LARGE   -2
#define ERROR_OPEN_FAILED      -3
#define ERROR_INVALID_RESOURCE -4


/**
 * @brief Resource type
 **/

typedef enum
{
   RES_TYPE_DIR  = 1,
   RES_TYPE_FILE = 2
} tResType;


//Set data aligment
#pragma pack(push, 1)


/**
 * @brief Resource entry
 **/

typedef struct
{
   uint8_t type;
   uint32_t dataOffset;
   uint32_t dataLength;
   uint8_t nameLength;
   char name[];
} tResEntry;


/**
 * @brief Resource header
 **/

typedef struct
{
   uint32_t totalSize;
   tResEntry rootEntry;
} tResHeader;


//Restore previous settings for data aligment
#pragma pack(pop)


#ifndef _WIN32

/**
 * @brief Break a path name into components
 * @param[in] path Full path
 * @param[in] drv Drive letter, followed by a colon
 * @param[in] dir Directory path, including trailing slash
 * @param[in] name Base filename (no extension)
 * @param[in] ext Filename extension, including leading period
 **/

void _splitpath(const char *path, char *drv, char *dir, char *name, char *ext)
{
   const char *end;
   const char *p;
   const char *s;

   //Extract drive name
   if(path[0] != '\0' && path[1] == ':')
   {
      if(drv != NULL)
      {
         *drv++ = *path++;
         *drv++ = *path++;
         *drv = '\0';
      }
   }
   else if(drv != NULL)
   {
      *drv = '\0';
   }

   //Search for the end of the string
   for(end = path; *end != '\0' && *end != ':'; )
      end++;

   //Search for the begin of the file extension
   for(p = end; p > path && *--p != '\\' && *p != '/'; )
   {
      if(*p == '.')
      {
         end = p;
         break;
      }
   }

   if(ext != NULL)
   {
      for(s = end; (*ext = *s++); )
         ext++;
   }

   //Search for the end of the directory name
   for(p = end; p > path; )
   {
      if(*--p == '\\' || *p == '/')
      {
         p++;
         break;
      }
   }

   if(name != NULL)
   {
      for(s = p; s < end; )
         *name++ = *s++;

      *name = '\0';
   }

   if(dir != NULL)
   {
      for(s = path; s < p; )
         *dir++ = *s++;

      *dir = '\0';
   }
}

#endif


/**
 * @brief Add the contents of a file to the resource data
 * @param[in] filename Path to the filename
 * @param[in] data Pointer to the resource data
 * @param[in] maxSize Maximum size of the resulting resource file
 * @param[out] length Actual length of the file
 * @return Status code
 **/

int addFile(const char *filename, uint8_t *data, uint32_t maxSize, uint32_t *length)
{
   int error;
   uint32_t n;
   FILE *fp;

   //Point to the header of the resource data
   tResHeader *ResHeader = (tResHeader *) data;

   //Open the specified file
   fp = fopen(filename, "rb");
   //Cannot open file?
   if(!fp)
      return ERROR_OPEN_FAILED;

   //Start of exception handling block
   do
   {
      //Jump to the end of the file
      error = fseek(fp, 0, SEEK_END);
      //Any error to report?
      if(error)
      {
         error = ERROR_FAILURE;
         break;
      }

      //Get the length of the file
      *length = ftell(fp);

      //Check the length of the file
      if((ResHeader->totalSize + *length) > maxSize)
      {
         error = ERROR_FILE_TOO_LARGE;
         break;
      }

      //Back the beginning of the file
      error = fseek(fp, 0, SEEK_SET);
      //Any error to report?
      if(error)
      {
         error = ERROR_FAILURE;
         break;
      }

      //Read file contents
      n = fread(data + ResHeader->totalSize, 1, *length, fp);
      //Failed to read data?
      if(n != *length)
      {
         error = ERROR_FAILURE;
         break;
      }

      //Update the total length of the resource data
      ResHeader->totalSize += *length;

      //Successful processing
      error = NO_ERROR;

      //End of exception handling block
   } while(0);

   //Close file
   fclose(fp);
   //Return status code
   return error;
}


/**
 * @brief Add the contents of a directory to the resource data
 * @param[in] parentOffset Offset of the parent directory
 * @param[in] parentSize Size of the parent directory
 * @param[in] directory Path to the directory
 * @param[in] data Pointer to the resource data
 * @param[in] maxSize Maximum size of the resulting resource file
 * @param[out] length Actual length of the directory
 * @return Status code
 **/

int addDirectory(uint32_t parentOffset, uint32_t parentSize,
   const char *directory, uint8_t *data, uint32_t maxSize, uint32_t *length)
{
   int error;
   uint32_t i;
   uint32_t pos;
   char path[PATH_MAX];
   char filename[PATH_MAX];
   tResEntry *entry;

#ifdef _WIN32
   intptr_t hFind;
   struct _finddata_t findData;
#else
   DIR *dir;
   struct dirent *dirEntry;
#endif

   //Point to the header of the resource data
   tResHeader *ResHeader = (tResHeader *) data;
   //Point to the location where to write the directory contents
   pos = ResHeader->totalSize;
   //Initialize the length field
   *length = 0;

#ifdef _WIN32
   //Append "*" to the directory path
   _makepath(path, NULL, directory, "*", NULL);

   //Open directory
   hFind = _findfirst(path, &findData);
   //Unable to read directory?
   if(hFind < 0)
      return ERROR_OPEN_FAILED;
#else
   //Open directory
   dir = opendir(directory);
   //Unable to read directory?
   if(dir == NULL)
      return ERROR_OPEN_FAILED;
#endif

   //Point to the location where to write the directory contents
   i = pos;

   //Check the actual size of the resource data
   if((i + sizeof(tResEntry) + 1) >= maxSize)
   {
#ifdef _WIN32
      //Close find search handle
      _findclose(hFind);
#else
      //Close directory
      closedir(dir);
#endif
      //Report an error
      return ERROR_FILE_TOO_LARGE;
   }

   //Current directory
   entry = (tResEntry *) (data + i);
   entry->type = RES_TYPE_DIR;
   entry->dataOffset = 0;
   entry->dataLength = 0;
   entry->nameLength = 1;
   entry->name[0] = '.';

   //Jump to the following entry
   i += sizeof(tResEntry) + entry->nameLength;
   //Update the length of the directory
   *length += sizeof(tResEntry) + entry->nameLength;

   //Add a link to the parent directory?
   if(parentOffset && parentSize)
   {
      //Check the actual size of the resource data
      if((i + sizeof(tResEntry) + 2) >= maxSize)
      {
#ifdef _WIN32
         //Close find search handle
         _findclose(hFind);
#else
         //Close directory
         closedir(dir);
#endif
         //Report an error
         return ERROR_FILE_TOO_LARGE;
      }

      //Parent directory
      entry = (tResEntry *) (data + i);
      entry->type = RES_TYPE_DIR;
      entry->dataOffset = 0;
      entry->dataLength = 0;
      entry->nameLength = 2;
      entry->name[0] = '.';
      entry->name[1] = '.';

      //Jump to the following entry
      i += sizeof(tResEntry) + entry->nameLength;
      //Update the length of the directory
      *length += sizeof(tResEntry) + entry->nameLength;
   }

   //Loop through directory entries
   while(1)
   {
#ifdef _WIN32
      //Check file name
      if(!strcmp(findData.name, ".") || !strcmp(findData.name, ".."))
      {
         //Discard . and .. directories
      }
      //Check file attributes
      else if(findData.attrib & _A_HIDDEN)
      {
         //Discard hidden files
      }
      else
      {
         //Make sure the maximum size is not exceeded
         if((i + sizeof(tResEntry) + strlen(findData.name)) >= maxSize)
         {
            //Close find search handle
            _findclose(hFind);
            //Report an error
            return ERROR_FILE_TOO_LARGE;
         }

         //Add a new entry
         entry = (tResEntry *) (data + i);
         entry->type = (findData.attrib & _A_SUBDIR) ? RES_TYPE_DIR : RES_TYPE_FILE;
         entry->dataOffset = 0;
         entry->dataLength = 0;
         entry->nameLength = strlen(findData.name);
         strncpy(entry->name, findData.name, entry->nameLength);

         //Jump to the following entry
         i += sizeof(tResEntry) + entry->nameLength;
         //Update the length of the directory
         *length += sizeof(tResEntry) + entry->nameLength;
      }

      //Get next file
      if(_findnext(hFind, &findData) < 0)
         break;
#else
      //Read directory entry
      dirEntry = readdir(dir);
      //End of the directory stream?
      if(dirEntry == NULL)
         break;

      //Check file name
      if(!strcmp(dirEntry->d_name, ".") || !strcmp(dirEntry->d_name, ".."))
      {
         //Discard . and .. directories
      }
      //Check file type
      else if(dirEntry->d_type != DT_DIR && dirEntry->d_type != DT_REG)
      {
         //Discard unknown files
      }
      else
      {
         //Make sure the maximum size is not exceeded
         if((i + sizeof(tResEntry) + strlen(dirEntry->d_name)) >= maxSize)
         {
            //Close directory
            closedir(dir);
            //Report an error
            return ERROR_FILE_TOO_LARGE;
         }

         //Add a new entry
         entry = (tResEntry *) (data + i);
         entry->type = (dirEntry->d_type == DT_DIR) ? RES_TYPE_DIR : RES_TYPE_FILE;
         entry->dataOffset = 0;
         entry->dataLength = 0;
         entry->nameLength = strlen(dirEntry->d_name);
         strncpy(entry->name, dirEntry->d_name, entry->nameLength);

         //Jump to the following entry
         i += sizeof(tResEntry) + entry->nameLength;
         //Update the length of the directory
         *length += sizeof(tResEntry) + entry->nameLength;
      }
#endif
   }

   //Update the total size of the resource file
   ResHeader->totalSize += *length;

   //Parse the newly added entries
   for(i = 0; i < *length; i += sizeof(tResEntry) + entry->nameLength)
   {
      //Point to the current entry
      entry = (tResEntry *) (data + pos + i);

      //Special . directory?
      if(entry->nameLength == 1 && entry->name[0] == '.')
      {
         //Set data offset
         entry->dataOffset = pos;
         //Set directory size
         entry->dataLength = *length;
      }
      //Special .. directory?
      else if(entry->nameLength == 2 && entry->name[0] == '.' && entry->name[1] == '.')
      {
         //Set data offset
         entry->dataOffset = parentOffset;
         //Set directory size
         entry->dataLength = parentSize;
      }
      else
      {
         //Data must be aligned on 4-byte boundaries
         ResHeader->totalSize = (ResHeader->totalSize + 3) / 4 * 4;
         //Set data offset
         entry->dataOffset = ResHeader->totalSize;

         //Retrieve file name
         strncpy(filename, entry->name, entry->nameLength);
         filename[entry->nameLength] = '\0';

#ifdef _WIN32
         //Form the full path to the item
         _makepath(path, NULL, directory, filename, NULL);
#else
         //Form the full path to the item
         strcpy(path, directory);
         strcat(path, "/");
         strcat(path, filename);
#endif

         //Check entry type
         if(entry->type == RES_TYPE_DIR)
         {
            //Add the contents of the directory to the resource data
            error = addDirectory(pos, *length, path, data, maxSize, &entry->dataLength);
         }
         else
         {
            //Add the contents of the file to the resource data
            error = addFile(path, data, maxSize, &entry->dataLength);
         }

         //Any error to report?
         if(error)
         {
#ifdef _WIN32
            //Close find search handle
            _findclose(hFind);
#else
            //Close directory
            closedir(dir);
#endif
            //Report an error
            return error;
         }
      }
   }

#ifdef _WIN32
   //Close find search handle
   _findclose(hFind);
#else
   //Close directory
   closedir(dir);
#endif

   //Successful processing
   return 0;
}


/**
 * @brief Dump the contents of a directory
 * @param[in] directory Directory to dump
 * @param[in] parentSize Size of the parent directory
 * @param[in] level Recursion level
 * @return Status code
 **/

int dumpDirectory(const uint8_t *data, tResEntry *directory, unsigned int level)
{
   int error;
   uint32_t i;
   uint32_t n;
   tResEntry *entry;
   char filename[PATH_MAX];

   //Check entry type
   if(directory->type != RES_TYPE_DIR)
      return ERROR_FAILURE;

   //Retrieve the length of the directory
   n = directory->dataLength;
   //Point to the first entry
   entry = (tResEntry *) (data + directory->dataOffset);

   //Loop through the directory
   while(n > 0)
   {
      //Check the number of remaining bytes
      if(n < sizeof(tResEntry))
         return ERROR_INVALID_RESOURCE;
      //Make sure the entry is valid
      if(n < (sizeof(tResEntry) + entry->nameLength))
         return ERROR_INVALID_RESOURCE;

      //Retrieve filename
      strncpy(filename, entry->name, entry->nameLength);
      //Properly terminate the string with a NULL character
      filename[entry->nameLength] = '\0';

      //Indent the current line
      for(i = 0; i < level; i++) printf("  ");

      //Display filename
      if(entry->type == RES_TYPE_DIR)
         printf("[%s]\r\n", filename);
      else
         printf("%s (%u bytes)\r\n", filename, entry->dataLength);

      //Directory found?
      if(entry->type == RES_TYPE_DIR)
      {
         if(strcmp(filename, ".") && strcmp(filename, ".."))
         {
            //Recursively dump the contents of the directory
            error = dumpDirectory(data, entry, level + 1);
            //Any error to report?
            if(error) return error;
         }
      }

      //Remaining bytes to process
      n -= sizeof(tResEntry) + entry->nameLength;
      //Point to the next entry
      entry = (tResEntry *) ((uint8_t *) entry + sizeof(tResEntry) + entry->nameLength);
   }

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Main routine
 **/

int main(int argc, char *argv[])
{
   int error;
   unsigned int i;
   unsigned int maxSize;
   uint8_t *data;
   const char *srcDir;
   const char *destFile;
   char currentDir[PATH_MAX];
   char path[PATH_MAX];
   char drive[PATH_MAX];
   char filename[PATH_MAX];
   tResHeader *resHeader;
   FILE *fp;

   //Check parameters
   if(argc != 3 && argc != 4)
   {
      //Print command syntax
      printf("Usage: rc.exe input output [maxsize]\r\n");
      printf("  - input:   Source directory to include in resource file\r\n");
      printf("  - output:  Compiled resource file\r\n");
      printf("  - maxsize: Maximum size of the resource file\r\n");
      printf("Copyright (c) 2010-2017 Oryx Embedded\r\n");
      //Report an error
      return ERROR_FAILURE;
   }

   //Source directory
   srcDir = argv[1];
   //Destination resource file
   destFile = argv[2];

   //Maximum size of the resulting resource file
   maxSize = (argc == 4) ? atoi(argv[3]) : (1024 * 1024);

#ifdef _WIN32
   //Split source directory
   _splitpath(srcDir, drive, NULL, NULL, NULL);

   //Relative or absolute path?
   if(drive[0] == '\0')
   {
      //Get current working directory
      _getcwd(currentDir, _MAX_PATH);

      //Retrieve the full path
      _makepath(path, NULL, currentDir, srcDir, NULL);
   }
   else
   {
      //Copy the path to the source directory
      strcpy(path, srcDir);
   }
#else
   //Relative or absolute path?
   if(srcDir[0] != '/')
   {
      //Get current working directory
      getcwd(currentDir, PATH_MAX);

      //Retrieve the full path
      strcpy(path, currentDir);
      strcat(path, "/");
      strcat(path, srcDir);
   }
   else
   {
      //Copy the path to the source directory
      strcpy(path, srcDir);
   }
#endif

   //Allocate a memory buffer to hold the resulting data
   data = malloc(maxSize);
   //Failed to allocate memory?
   if(data == NULL)
   {
      printf("Error: Unable to allocate memory!\r\n");
      //Release previoulsy allocated memory
      free(data);
      return ERROR_FAILURE;
   }

   //Clear memory buffer
   memset(data, 0, maxSize);

   //Initialize resource data header
   resHeader = (tResHeader *) data;
   resHeader->totalSize = sizeof(tResHeader);
   resHeader->rootEntry.type = RES_TYPE_DIR;
   resHeader->rootEntry.dataOffset = sizeof(tResHeader);
   resHeader->rootEntry.dataLength = 0;
   resHeader->rootEntry.nameLength = 0;

   //Add the contents of the specified directory to the resource file
   error = addDirectory(0, 0, path, data, maxSize, &resHeader->rootEntry.dataLength);

   //Any error to report?
   if(error == ERROR_FILE_TOO_LARGE)
   {
      //User message
      printf("Error: Maximum size exceeded (%u bytes)!\r\n", maxSize);
      //Release previoulsy allocated memory
      free(data);
      //Report an error
      return ERROR_FAILURE;
   }
   else if(error)
   {
      //User message
      printf("Error: Unable to read source directory!\r\n");
      //Release previoulsy allocated memory
      free(data);
      //Report an error
      return ERROR_FAILURE;
   }

   //Open output file
   fp = fopen(destFile, "w+");

   //Cannot open output file
   if(!fp)
   {
      //User message
      printf("Error: Cannot open resource file!\r\n");
      //Release previoulsy allocated memory
      free(data);
      //Report an error
      return ERROR_FAILURE;
   }

   //Get filename
   _splitpath(destFile, NULL, NULL, filename, NULL);

   //Write header
   fprintf(fp, "const unsigned char %s[] =\n", filename);
   fprintf(fp, "{\n");

   //Write data
   for(i = 0; i < resHeader->totalSize; i++)
   {
      //Beginning of a new line?
      if((i % 16) == 0)
         fprintf(fp, "   ");

      //Display current data byte
      fprintf(fp, "0x%02X", data[i]);

      //Last byte?
      if(i == (resHeader->totalSize - 1))
         fprintf(fp, "\n");
      //End of current line?
      else if((i % 16) == 15)
         fprintf(fp, ",\n");
      else
         fprintf(fp, ", ");
   }

   //Properly terminate the array
   fprintf(fp, "};\n");
   //Close output file
   fclose(fp);

   //Dump the contents of the resource file
   dumpDirectory(data, &resHeader->rootEntry, 0);
   //User message
   printf("\r\n%u bytes successfully written !\r\n", resHeader->totalSize);

   //Release previoulsy allocated memory
   free(data);

   //Successful processing
   return NO_ERROR;
}
