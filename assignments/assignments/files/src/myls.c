#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <assert.h>

/* to use stat() */
#include <sys/stat.h>
#include <unistd.h>


#include <errno.h>

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




int main(int argc, char *argv[]) {

  if( argc < 2 ) {
    perror("usage: myls <dir>\n");
    return -1;
  }

  char *path = argv[1];

  struct stat file_st;
  stat(path, &file_st);  

  printf("The directory %s with inode %8lu\n", path,  file_st.st_ino);
  
  DIR *dirp = opendir(path);

  struct dirent *entry;

  errno = 0;
  
  while((entry = readdir(dirp)) != NULL) {
    switch( entry->d_type) {
    case DT_BLK  :  // This is a block device. 
      printf("b:");
      break;      
    case DT_CHR  :  //This is a character device.
      printf("c:");
      break;      
    case DT_DIR  :  //This is a directory.
        printf("d:");
        break;
    case DT_FIFO :  //This is a named pipe (FIFO).
      printf("p:");
      break;
    case DT_LNK  :  //This is a symbolic link.
      printf("l:");
      break;
    case DT_REG  :  //This is a regular file.
      printf("f:");
      break;
    case DT_SOCK :  //This is a UNIX domain socket.
      printf("s:");
      break;
    case DT_UNKNOWN :  // The file type is unknown.
      printf("u:");
      break;
    }

    printf("\tinode: %8lu",  entry->d_ino);
    printf("\tname: %s\n", entry->d_name);      

  }    
    
  if(errno != 0) {
    perror("something happened");
  }
}



  
