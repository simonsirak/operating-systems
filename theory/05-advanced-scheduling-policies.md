# Advanced Scheduling Policies
There are two more advanced policies worth noting: The Multi Level Feedback Queue (MLFQ) and the 
Propotional Share scheduling policy. These are rather different, but both have their advantages and disadvantages.

## MLFQ
The MLFQ basically works as follows. The processes are partitioned into several different queues based on their current 
priority. The process with highest priority is run first, and priority is modified by demoting/promoting a process to a 
different queue based on previous diagnostics. Usually we keep track of how long a process has been running, and if it has been 
running longer than some period then it is demoted. This prevents long jobs from dominating. Similarly, we promote all 
processes to highest priority after some other period/number of timer interrupts. This prevents short jobs from dominating.

More specifically, here are the ground rules, where A and B are processes and P(X) is the priority of X:

1. Processes start at the highest priority. We assume each new job is short to give all jobs a fair shot.
2. If P(A) > P(B) then run A. 
3. If P(A) = P(B) then run them in a RR manner.
4. If a process has been in a certain queue longer than some period D *in total since its arrival to the queue*, then it is demoted to a lower priority. The part in italics renders the process unable to game the OS by performing an IO operation right before D time units have passed (otherwise, the process could reset the timer using the IO operation).
5. After some period R, reset all process priorities to the highest priority. This gives processes that change behaviour (from long-running, number-crunching job to interactive job) a fair shot. It also helps avoid starvation by letting long-running jobs back to a high-priority queue, so they get some CPU time.

## Proportional Share
This scheduling policy bases itself on randomization. The general idea is that each process is assigned some number of tickets. The order and which tickets do not matter, just the number of tickets, since everything is random anyway. Then, 
before each context switch, a random number in the range 0-MAXTICKET is drawn. Then, simply traverse the list of processes 
and keep track of a counter that adds the number of tickets each process has until the counter exceeds the drawn ticket. 
If the drawn ticket was larger than the ticket numbers for all the previous processes, but not for the current process, 
then (if we label the encountered tickets 0 and upwards) the drawn ticket is held by the current process. Simply choose 
that process and you are done.

Each user may have its own ticket currency for its processes. As long as there is a bijection that converts the currency to the global currency used by the ticket generator, then we are good. Also, one can also modify this scheduler by allowing 
processes to lend their tickets to other processes (ticket transfer). For instance if a client requests something from a server that is on the same computer. A process can even be able to increase its number of tickets temporarily to boost performance, but that is usually not good for computers with apps competing for resources.

A deterministic variant of proportional share is to introduce the concept of strides. Each process can be viewed to take strides based on the number of tickets it owns; more tickets means smaller stride. Then, the process with the smallest distance traveled will be the process that is switched to. This pretty much ensures a proportional amount of processor time for each process by the end of their life cycles. However, this also means you need to save extra data for each process, corresponding to how much they've traveled.

## Usages 
MLFQ is used by one "half" of the "Unix-like family" (FreeBSD, macOS), and proportional share is used by the "other half" (at least the Linux-kernel).
