# Simple Scheduling Policies
There are a few simple scheduling policies that are useful when you can make certain assumptions about the nature 
of the processes to be ran on the computer. The performance of a scheduling policy is usually measured in terms of turnaround
time (time of completion minus time of arrival) and response time (time of first scheduling minus time of arrival). These
measure efficiency and fairness respectively.

Here are some common policies. Assumptions will be made in the first one and then incrementally be peeled off.

## FIFO
If you know the length of each process and theyre the same length, if they all start at the same time, if they always need to run 
to completion in one go and they require no I/O. Then, any choice of policy will do, really. However, in this case a simple 
FIFO-scheduling policy will do. Reordering makes no difference to response time.

## SJF
If they are not the same size, reordering does matter. In this case, Shortest Job First is a sufficient policy: Run the
shortest one first. This is optimal under the above conditions mentioned in FIFO and above sentence.

## PSJF
If they also do not arrive at the same time and do not need to run to completion, SJF is no longer optimal 
(just think if a short process arrives after a long one). In this case, a preemptive SJF is optimal; 
each timer interrupt, you perform a context switch to the currently shortest job (shortest meaning the 
remaining length, i presume).

## RR
Round Robin (RR) prioritizes fairness over efficiency. It switches context periodically, such that each of the n processes 
are run exactly once for each set of n context switches.

## I/O Incorporation
Incorporating I/O is simple: Whenever an I/O-request is made, you switch context using whatever policy you have. In order 
to do this, it is useful to think of any one process as "its partition", where the parts are separated by I/O operations. 
Since these take time, we may look at each part as a separate process when applying any other scheduling policy.

