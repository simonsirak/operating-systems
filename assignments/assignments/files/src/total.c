#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <assert.h>

/* to use stat() */
#include <sys/stat.h>
#include <unistd.h>

/* to use strcmp() */
#include <string.h>
 
/*

struct dirent {
    ino_t          d_ino;       inode number
    off_t          d_off;        offset to the next dirent 
    unsigned short d_reclen;    length of this record 
    unsigned char  d_type;      type of file; not supported by all file system types 
    char           d_name[256]; filename 
};

*/

/*

#define st_mtime st_mtim.tv_sec
#define st_ctime st_ctim.tv_sec

struct stat {
  dev_t     st_dev;          ID of device containing file 
  ino_t     st_ino;          inode number 
  mode_t    st_mode;         protection 
  nlink_t   st_nlink;        number of hard links 
  uid_t     st_uid;          user ID of owner
  gid_t     st_gid;          group ID of owner 
  dev_t     st_rdev;         device ID (if special file) 
  off_t     st_size;         total size, in bytes 
  blksize_t st_blksize;      blocksize for filesystem I/O
  blkcnt_t  st_blocks;       number of 512B blocks allocated 

  struct timespec st_atim;   time of last access 
  struct timespec st_mtim;   time of last modification 
  struct timespec st_ctim;   time of last status change 
};
*/


unsigned long count(char *path) {

  unsigned long total = 0;
  
  DIR *dirp = opendir(path);

  if(dirp == NULL) {
    printf("not able to open %s\n", path);
    return 0;
  }
  
  char subdir[1024];  
  
  struct dirent *entry;
  
  while((entry = readdir(dirp)) != NULL) {
    switch( entry->d_type) {

    case DT_DIR:  //This is a directory.
      if((strcmp(entry->d_name, ".") == 0)  | (strcmp(entry->d_name, "..") == 0)) {
	break;
      };

      sprintf(subdir, "%s/%s", path, entry->d_name);
      total += count(subdir);
      break;

    case DT_REG:  //This is a regular file.
      // printf("f: %s\n", entry->d_name);
      total++;
      break;

    default:
      break;
    }

  }
  closedir(dirp);  
  return total;
}  



int main(int argc, char *argv[]) {

  if( argc < 2 ) {
    perror("usage: freq <dir>\n");
    return -1;
  }

  char *path = argv[1];

  unsigned long total =  count(path);

  printf("The directory %s contains %lu files\n", path, total);
    
}



  
