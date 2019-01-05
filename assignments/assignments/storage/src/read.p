#
# plot of read time 
#
#
# candlestick opening:low:high:closing
#
set terminal png
set output "read.png"

unset key

set yrange [0:2800]  
set xrange [-30:500]

set xtics 64

set xlabel "index in block"
set ylabel "time in ns"

set boxwidth 10

plot 'read.dat' using 1:3:2:6:5 with candlesticks

