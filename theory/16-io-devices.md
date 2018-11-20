# IO Devices 
IO devices are anything that receive input and present some output. Displays, GPUs, mouse, keyboard, hard drive are all examples 
of IO devices. Certain devices require faster transmission of data, data that is transmitted through some bus. There are different 
protocols (busses); faster protocols like the memory bus is reserved for things like the main memory. The I/O bus is for 
display, GPU and other performance-based I/O, whereas slower protocols (peripheral buses) like USB are used for 
devices where some lag does no harm (like keyboard, mouse, hard drive). 

IO devices are either accessed via explicit operations in the CPU architecture (which are priviliged operations), 
or via a memory mapped IO (probably also priviliged). We can receive and send data to them via polling (which waits for a 
condition, e.g that the device has received some data successfully). However this is inefficient. One other alternative is to 
let devices send hardware interrupts upon some event. This is much more efficient as it frees the CPU of spinning from polling, 
however large amounts of interrupts can essentially starve the CPU from ever getting any user-processes to actually run (e.g if a
data center was queried with a disgusting amount of writes/reads). Thus, polling actually prevents starvation. A mix of these 
could be used as well, since polling (if the wait is not that long) can actually prevent the need to perform excessive context 
switches from processes that sleep when waiting for some input but then immediately wake up again. And also mitigates the context 
switches due to interrupts.

A further optimization is to remove the need for programmed IO (CPU-based IO using polling/interrupts) through the DMA: 
Direct Memory Access device. This device exploits the simple nature of load/store calls. If we need to send a large amount 
of data to a device, using the CPU is still insufficient since we would need to send like 8 bytes at a time, wait for confirmation,
and then proceed. For 1MB of data, that would be horrible. Thus, we use a dedicated devices for handling large data/command transfers:
The CPU tells the DMA where and what data to store in a chosen device (also chosen by the data store), and the DMA handles the rest. 
This frees the CPU from explicitly waiting for the common operation of storing data/sending commands to a device.

In order for the OS to handle various kinds of devices, each device is required to have a device driver that, for each OS-related 
operation on a device of a certain kind, the driver has implemented the operation based on the devices own hardware. An interesting 
thing to note is that the device drivers make up a large amount of the OS itself, if counted as part of the OS.
