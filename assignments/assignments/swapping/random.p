 # Gnuplot script file for plotting data in file "random.dat"
set terminal png
set output "random.png"

set terminal png linewidth 4 size 800,600  font verdana 24


# This is to set the color 
set style line 1 lc rgb "black" lw 1 pt 1
set style line 2 lc rgb "red" lw 1 pt 1

set title ""

set key right center
    
set xlabel "frames in memory"
set ylabel "hit ratio"    

set xrange [0:100]
set yrange [0:1]


plot "random.dat" u 1:2 w linespoints ls 2 title "random"
