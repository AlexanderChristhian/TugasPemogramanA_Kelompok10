# Interpolasi dan Regresi Data Penduduk dan Pengguna Internet Indonesia

Program ini menganalisis data penduduk dan persentase pengguna internet di Indonesia dari tahun 1960 hingga 2023. Program melakukan interpolasi untuk mengestimasi nilai yang hilang dan regresi polinomial untuk memprediksi nilai di masa depan.

## Hasil Analisis

==================================================
HASIL ESTIMASI NILAI YANG HILANG (INTERPOLASI)
==================================================

1. Estimasi Jumlah Penduduk Indonesia:
   2005: 230971878 jiwa (interpolasi antara 2004 dan 2007)
   2006: 234017108 jiwa (interpolasi antara 2004 dan 2007)
   2015: 261700485 jiwa (interpolasi antara 2014 dan 2017)
   2016: 264523572 jiwa (interpolasi antara 2014 dan 2017)

2. Estimasi Persentase Pengguna Internet Indonesia:
   2005: 3.6623% (interpolasi antara 2004 dan 2007)
   2006: 4.7243% (interpolasi antara 2004 dan 2007)
   2015: 22.2074% (interpolasi antara 2014 dan 2017)
   2016: 27.2716% (interpolasi antara 2014 dan 2017)

==================================================
PERSAMAAN POLINOMIAL (UNTUK ESTIMASI MASA DEPAN)
==================================================

1. Persamaan untuk Pertumbuhan Populasi Indonesia:
   y = 87172094.457997 + 2716701.135408(Year - 1960)^1 + 24669.141827(Year - 1960)^2 - 299.811689(Year - 1960)^3
   R^2 = 0.999879

2. Persamaan untuk Persentase Pengguna Internet Indonesia:
   y = -263.972543 + 20.457447(Year - 1960)^1 - 0.530709(Year - 1960)^2 + 0.004622(Year - 1960)^3
   R^2 = 0.993636

==================================================
ESTIMASI MASA DEPAN
==================================================

1. Estimasi Jumlah Penduduk Indonesia tahun 2030:
   295384560 jiwa

2. Estimasi Jumlah Pengguna Internet Indonesia tahun 2035:
   Persentase (uncapped): 235.0873%
   Persentase (capped): 100.0000%
   Populasi: 303205546 jiwa
   Jumlah Pengguna Internet: 303205546 jiwa

## Visualisasi

### Interpolasi Populasi Indonesia
![Interpolasi Populasi Indonesia](population_interpolation.png)

### Interpolasi Persentase Pengguna Internet Indonesia
![Interpolasi Persentase Pengguna Internet Indonesia](internet_interpolation.png)

### Regresi Pertumbuhan Populasi Indonesia
![Pertumbuhan Populasi Indonesia](population_regression.png)

### Regresi Persentase Pengguna Internet Indonesia
![Persentase Pengguna Internet Indonesia](internet_regression.png)

## Metode

1. **Interpolasi Linear**: Digunakan untuk mengestimasi nilai yang hilang di antara dua titik data yang diketahui.
   - Formula: y = y₁ + (x - x₁) * (y₂ - y₁) / (x₂ - x₁)
   - Digunakan untuk mengestimasi nilai pada tahun 2005, 2006, 2015, dan 2016.

2. **Regresi Polinomial**: Digunakan untuk memodelkan tren data dan memprediksi nilai di masa depan.
   - Polinomial derajat 3 (kubik) digunakan untuk kedua dataset.
   - Metode eliminasi Gauss digunakan untuk menyelesaikan sistem persamaan linear.
   - Koefisien determinasi (R²) yang tinggi menunjukkan model yang sangat baik.

## Kesimpulan

1. **Pertumbuhan Populasi**: Indonesia menunjukkan pertumbuhan populasi yang stabil dan dapat diprediksi dengan baik menggunakan model polinomial (R² = 0.999879).

2. **Adopsi Internet**: Persentase pengguna internet di Indonesia menunjukkan pertumbuhan eksponensial, terutama setelah tahun 2010. Model prediksi menunjukkan bahwa pada tahun 2035, seluruh populasi Indonesia akan menjadi pengguna internet.

3. **Keterbatasan Model**: Prediksi persentase pengguna internet melebihi 100% pada tahun 2035, yang secara logis tidak mungkin. Ini menunjukkan keterbatasan model polinomial untuk prediksi jangka panjang dan perlunya model yang lebih kompleks atau penerapan batasan logis.
