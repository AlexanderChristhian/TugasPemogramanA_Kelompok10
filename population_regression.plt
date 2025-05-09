set terminal png size 800,600
set output 'population_regression.png'
set title 'Pertumbuhan Populasi Indonesia'
set xlabel 'Tahun'
set ylabel 'Jumlah Penduduk'
set grid
set key outside
plot 'population_regression.dat' index 0 using 1:2 with points pointtype 7 pointsize 1 title 'Data Asli', \
     'population_regression.dat' index 1 using 1:2 with lines lw 2 title 'Kurva Regresi', \
     2005 notitle with lines lt 0 lw 1 dashtype 2, \
     2006 notitle with lines lt 0 lw 1 dashtype 2, \
     2015 notitle with lines lt 0 lw 1 dashtype 2, \
     2016 notitle with lines lt 0 lw 1 dashtype 2
