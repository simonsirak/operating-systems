# Advanced Scheduling Policies
There are two more advanced policies worth noting: The Multi Level Feedback Queue (MLFQ) and the 
Propotional Share scheduling policy. These are rather different, but both have their advantages and disadvantages.

# MLFQ
The MLFQ basically works as follows. The processes are partitioned into several different queues based on their current 
priority. The process with highest priority is run first, and priority is modified by demoting/promoting a process to a 
different queue based on previous diagnostics. Usually we keep track of how long a process has been running, and if it has been 
running longer than some period then it is demoted. This prevents long jobs from dominating. Similarly, we promote all 
processes to highest priority after some other period/number of timer interrupts. This prevents short jobs from dominating.

More specifically, here are the ground rules, where A and B are processes and P(X) is the priority of X:

1. Processes start at the highest priority.
2. If P(A) > P(B) then run A.
3. If P(A) = P(B) then run them in a RR manner.
4. If a process has run longer than some period D, then it is demoted to a lower priority.
5. After some period R, reset all process priorities to the highest priority.
