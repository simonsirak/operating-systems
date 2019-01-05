set terminal png
set output "tlb.png"

set title "TLB benchmark (10 Mi operations)"

set key right center
    
set xlabel "number of pages"
    
set ylabel "time in s"

set logscale x 2    

plot "tlb4K.dat" u 1:2 w linespoints  title "page size 4K bytes",  \
     "tlb64.dat" u 1:2 w linespoints  title "page size 64 bytes", \
     "dummy.dat" u 1:2 w linespoints  title "dummy"
