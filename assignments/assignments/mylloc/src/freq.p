
set terminal png

set output 'freq.png'

set logscale y
plot "freq.dat" u 1
