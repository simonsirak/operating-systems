# Hard Disk Drives
A hard disk consists of a number of disks/surfaces, each of which has a number of tracks (essentially circles) around each disk. each track is divided into sectors. We write and read from a HDD by using a sort of pin "disk head". There is 1 head per surface, and it can rotate to change track. Each disk can also spin around in *one* direction. 

Keeping this in mind, we can evaluate the time of writing/reading to a disk by adding together the delay from spinning a surface, as well as the delay for switching track (seeking), and the delay for transferring the data. The transfer itself isn't that bad, it is usually the spinning/seeking that are the worst offenders timewise. But I guess that can vary.

One should also consider the write-back/write-through nature of the cache of a HDD. (Maybe fill this in)

There are a few different protocols for HDD reads/writes. Since we can assume/approximate the time of each read/write pretty well, we would want to employ a SJF-like scheduling principle (see summary of scheduling policies) of I/O operations on the HDD since it has been proven to be optimal under this assumption. There are a few poor approximations of this policy, and one good. We will go 
through them.

Please do note that the OS usually just tries to pick a few requests that are "good enough". The HDD itself is sophisticated and 
usually implements its own scheduler, like SPTF, to optimize the sent requests.

## Shortest Seek Time First (SSTF)
The first approximation is SSTF. This policy schedules IO requests such that they are executed "by the closest track". We prefer the 
closer tracks. This is not very good though, since that may lead to starvation in the sense that certain IO requests are never done 
if they are always "too far away". An OS can implement this precisely by implementing Nearest Block First (like SSTF but nearest block 
instead, since the OS only knows the block-interface). These are effectively the same if we are using a HDD.

## Elevator (Circular-SCAN)
The C-SCAN is a bit better. It essentially does all the work on the current track, then moves up 1 track, etc until it reaches the last 
track, in which it loops back to the innermost track. The more primitive SCAN is biased towards the middle tracks. This avoids starvation.

## Shortest Positioning Time First (SPTF)
SPTF picks the request with the shortest positioning time, i.e rotation+seek. This is faster since it takes both rotation and seek into 
account. However, like with SSTF, this is pretty much impossible to implement on the OS.

## Extra from Lecture
Sectors are usually 4KB today, used to be 512 bytes. It is less relevant to find the first sector. Usually we want to measure a large amount of consecutive reads.

How is an SSD built? It has erased blocks, each containing a number of pages. We can only write to erased blocks, but we can read in constant time to any page. So if you wanna change 1 byte, you would need to copy a whole block, with that 1 byte changed, and then mark the old block as erasable. So reads are usually faster than writes. Also, a "more empty" SSD is much faster than a "more full" SSD due to having to swap around blocks. Stressing the SSD too much will wear it out. HDD lasts 
longer.

The tmp folder is used in order to avoid unneccessary tear of the SSD, since the tmp is mapped to the RAM and deleted upon shutdown. SSDs can have certain blocks destroyed, but has a hard time recovering. HDD is more resilient, and can work around destroyed sectors and stuff.

High performance HDD:s are now replaced by SSHD, hybrid of SSD and HDD.
