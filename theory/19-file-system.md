# File Systems
These notes cover the basics of a file system and how one can optimize it.

## Basic File System
First, we need some way of storing everything in the file system. Let us think of persistent memory in terms of blocks (**note that 
these blocks are not necessarily the same size as the sectors of the corresponding disk**). A simple 
file system needs to store the information that makes up a file (the "inode" of a file, it usually contains an identifying integer, timestamp information,
pointers to data blocks, size of the file, corresponding "human-readable" name string, and similar information), as well as the data
for the file itself (contained in data blocks). One inode takes up typically about 128 or 256 bytes. Using that and the block size,
one can deduce the number of inodes that fit into an inode block.

In order to know what data blocks/inodes are taken, we reserve two blocks for two bitmaps: An inode bitmap and a data block bitmap. 
Each bit in, say, the inode bitmap is 1 if the corresponding inode is taken, and 0 if it is free.

To top it off, we reserve a super block that stores information about how many data blocks and inode blocks exist, where the different 
sections described above begin (like the inode table), and some number that identifies the type of this file system.

We now have a basis for storage of the file system. Knowing the start of the inode region as well as the block size, one can use the 
identifying integer of an inode to find the inode block in memory. Since persistent memory is not byte addressable, we would need to 
calculate the block in which the inode is in (`block = inumber * sizeof(inode_t) / blockSize`) and then use that information to calculate 
in what sector the inode is in (`((block * blockSize) + startOfInodeRegion) / sectorSize`).
