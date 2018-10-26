# Limited Direct Execution
What is LDE? Well, it is a mechanism for how a process is to be executed on the computer. the "direct" aspect of it describes 
the fact that the process gains direct control of the processor while it is running. The "limited" aspect is the interesting part. 

In general, we like to keep certain critical operations restricted/priviliged (I/O and other interrupt/trap-related peripherals). As such, we introduce two processor modes: kernel mode and user mode. Kernel mode is turned on by the hardware whenever an interrupt occurs, and when the computer first boots. In fact, when you first boot, you actually set up a trap-table where each trap is related to a trap handler. For instance, the trap handler for an I/O request would be to perform the specified I/O operation. Each entry in the trap table is related to an integer, and that number is usually what defines the specific system call. Now, how can we use this to control the execution of processes?

In a cooperative system, you let a process do whatever it wants, and it will hand over control to the OS only when it 
performs a priviliged operation. Only then can the OS regain control and switch process. However it is more common to use the non-cooperative approach. Here, we configure a special time interrupt in our trap table. The interrupt is handled by letting the OS investigate whether a context switch is necessary.

# Context Switches
When a context switch from process A to process B occurs, it goes as follows:

1. The hardware stores all registers for A onto A:s kernel stack. The kernel stack is a fixed-size stack with room for the 
registers that constitute the current state of the CPU for a process, and is used generally for all types of traps. 
The hardware then jumps to the trap handler, which is essentially
a procedure in the OS. If this was any interrupt, then this ensures that the current state of A is preserved. Saving to the
kernel stack is necessary because the routine performed via the trap handler uses those registers.
2. The OS performs the switch. It saves the registers of A onto its process structure (using the kernel stack of A saved in the 
previous step). Note that if the trap was NOT a context switch, then the trap would do its business, return-from-trap, and step 3 would be carried out on the process A --
no need to save the state (i.e the context, which was originally saved to the kernel stack) to the process structure. That 
is only necessary because this is a context switch, so the interrupt has to save the kernel stack to the process structure. I don't really get WHY you need to save the context to the process structure if you already have the kernel stack saved in the process structure.
