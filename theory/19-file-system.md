# File Systems
These notes cover the basics of a file system and how one can optimize it.

## Basic File System
First, we need some way of storing everything in the file system. Let us think of persistent memory in terms of blocks (**note that 
these blocks are not necessarily the same size as the sectors of the corresponding disk**). A simple 
file system needs to store the information that makes up a file (the "inode" of a file, it usually contains an identifying integer, the type of file, timestamp information, pointers to data blocks, size of the file, corresponding "human-readable" name string, and similar information), as well as the data for the file itself (contained in data blocks). One inode takes up typically about 128 or 256 bytes. Using that and the block size, one can deduce the number of inodes that fit into an inode block.

In order to know what data blocks/inodes are taken, we reserve two blocks for two bitmaps: An inode bitmap and a data block bitmap. 
Each bit in, say, the inode bitmap is 1 if the corresponding inode is taken, and 0 if it is free.

To top it off, we reserve a super block that stores information about how many data blocks and inode blocks exist, where the different 
sections described above begin (like the inode table), and some number that identifies the type of this file system.

We now have a basis for storage of the file system. Knowing the start of the inode region as well as the block size, one can use the 
identifying integer of an inode to find the inode block in memory. Since persistent memory is not byte addressable, we would need to 
calculate the block in which the inode is in (`block = inumber * sizeof(inode_t) / blockSize`) and then use that information to calculate 
in what sector the inode is in (`((block * blockSize) + startOfInodeRegion) / sectorSize`).

>**Connecting inodes to Data Blocks**
>
> One fast way is to, as mentioned above, have direct pointers to data blocks. This is fast, but static and files are thus constrained  in file size. A better way is to store some direct pointers, followed by some indirect pointers. Indirect pointers point to data blocks that store direct pointers, and as such you can fit many more data blocks in a file. This is only a two-level index however. In fact, you can 
even allow the data block pointed to by an indirect pointer to store indirect pointers, which would allow an incredible amount of data blocks. You usually want to mix it up in an inode: Store a few direct pointers, a couple of indirect pointers, a double indirect pointer and a triple indirect pointer. The reason is because many files never break certain size barriers, and thus might not need an indirect pointer or a double indirect pointer. Therefore, we have direct pointers so that accesses to small files require less disk accesses in total for instance.

Directories are simply files with a special type. For each file/directory in the directory, they store the inumber, record length, string length, and a string name. This is stored as a linear list. The record name is stored in order to show how much data the file entry (not the size of the file) actually takes up in the directory. If a file entry takes place in a part of the directory that was held by a file with a longer string name, then the record length will be the size of the previous file entry. 

Creating files is as simple as checking the bitmap for a free inode, allocate it, update the bitmap and initialize the inode metadata that is feasible to initialize. A pre-allocaiton policy can be used to boost performance (usually boost performance), by allocating 8 blocks of contiguous memory when creating a new file. 

To read a file you would need to do the following (given a path starting at root): First, read the inode block for the root directory. Then, read the data block(s) until you find the entry for the next directory in the path. Then, read the inode block for that directory, and read the data block(s) until you find the file entry in the given directory you are in. Then, read the inode block for the file, and lastly scan through the data blocks of the files. After each read you would need to write to the inode to mark the new access timestamp.

