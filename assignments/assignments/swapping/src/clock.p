# 
set terminal png
set output "clock.png"

set terminal png linewidth 4 size 800,600  font verdana 24

# This is to set the color 
set style line 1 lc rgb "black" lw 1 pt 1
set style line 2 lc rgb "red" lw 1 pt 1
set style line 3 lc rgb "blue" lw 1 pt 1
set style line 4 lc rgb "green" lw 1 pt 1
set style line 5 lc rgb "gray" lw 1 pt 1

set title ""

set key right center
    
set xlabel "frames in memory"
set ylabel "hit ratio"    

set xrange [0:100]
set yrange [0:1]


plot "random.dat" u 1:2 w linespoints ls 2 title "random",  "best.dat" u 1:2 w linespoints ls 3 title "optimal",  "lru.dat" u 1:2 w linespoints ls 4 title "LRU",   "clock.dat" u 1:2 w linespoints ls 5 title "clock", 
