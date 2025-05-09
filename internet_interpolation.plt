set terminal png size 800,600
set output 'internet_interpolation.png'
set title 'Interpolasi Persentase Pengguna Internet Indonesia'
set xlabel 'Tahun'
set ylabel 'Persentase (%)'
set grid
set key outside
plot 'internet_interpolation.dat' index 0 using 1:2 with linespoints pointtype 7 pointsize 1 title 'Data Asli', \
     'internet_interpolation.dat' index 1 using 1:2 with points pointtype 9 pointsize 1.5 title 'Nilai Interpolasi'
