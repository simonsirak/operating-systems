# Hard Disk Drives
A hard disk consists of a number of disks/surfaces, each of which has a number of tracks (essentially circles) around each disk. 
each track is divided into sectors. We write and read from a HDD by using a sort of pin "disk head". There is 1 head per surface, 
and it can rotate to change track. Each disk can also spin around in *one* direction. 

Keeping this in mind, we can evaluate the time of writing/reading to a disk by adding together the delay from spinning a surface,
as well as the delay for switching track (seeking), and the delay for transferring the data. The transfer itself isn't that bad, 
it is usually the spinning/seeking that are the worst offenders timewise. But I guess that can vary.

One should also consider the write-back/write-through nature of the cache of a HDD. (Maybe fill this in)

There are a few different protocols for HDD reads/writes. Since we can assume/approximate the time of each read/write pretty well, we would want to employ a SJF-like scheduling principle (see summary of scheduling policies) of I/O operations on the HDD since it has been proven to be optimal under this assumption. There are a few poor approximations of this policy, and one good. We will go 
through them.

## Shortest Seek Time First (SSTF)
-- TODO -- 

## Elevator (Circular-SCAN)
-- TODO -- 

## Shortest Positioning Time First (SPTF)
-- TODO -- 
