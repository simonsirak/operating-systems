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
is only necessary because this is a context switch, so the interrupt has to save the kernel stack to the process structure. I don't
really get WHY you need to save the context to the process structure if you already have the kernel stack saved in the process structure.
