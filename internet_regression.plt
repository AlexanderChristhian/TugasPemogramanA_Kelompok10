set terminal png size 800,600
set output 'internet_regression.png'
set title 'Persentase Pengguna Internet Indonesia'
set xlabel 'Tahun'
set ylabel 'Persentase (%)'
set grid
set key outside
plot 'internet_regression.dat' index 0 using 1:2 with points pointtype 7 pointsize 1 title 'Data Asli', \
     'internet_regression.dat' index 1 using 1:2 with lines lw 2 title 'Kurva Regresi', \
     2005 notitle with lines lt 0 lw 1 dashtype 2, \
     2006 notitle with lines lt 0 lw 1 dashtype 2, \
     2015 notitle with lines lt 0 lw 1 dashtype 2, \
     2016 notitle with lines lt 0 lw 1 dashtype 2
