set terminal png size 800,600
set output 'population_interpolation.png'
set title 'Interpolasi Populasi Indonesia'
set xlabel 'Tahun'
set ylabel 'Jumlah Penduduk'
set grid
set key outside
plot 'population_interpolation.dat' index 0 using 1:2 with linespoints pointtype 7 pointsize 1 title 'Data Asli', \
     'population_interpolation.dat' index 1 using 1:2 with points pointtype 9 pointsize 1.5 title 'Nilai Interpolasi'
