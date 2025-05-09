#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_ROWS 100
#define MAX_LINE_LENGTH 256
#define POLYNOMIAL_DEGREE 3
#define BASE_YEAR 1960


typedef struct {
    int year;
    double internet_percentage;
    double population;
} DataRow;


typedef struct {
    double coeffs[POLYNOMIAL_DEGREE + 1]; // +1 untuk konstan
    double r_squared;
} PolynomialModel;


int readCSV(const char* filename, DataRow* data) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    int row_count = 0;

    // skip header
    fgets(line, MAX_LINE_LENGTH, file);

    while (fgets(line, MAX_LINE_LENGTH, file) && row_count < MAX_ROWS) {
        char* token = strtok(line, ",");
        if (token != NULL) {
            data[row_count].year = atoi(token);

            token = strtok(NULL, ",");
            if (token != NULL) {
                data[row_count].internet_percentage = atof(token);

                token = strtok(NULL, ",");
                if (token != NULL) {
                    data[row_count].population = atof(token);
                    row_count++;
                }
            }
        }
    }

    fclose(file);
    return row_count;
}

// kalkulasi rata-rata
double mean(double* arr, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum / n;
}

// kalkulasi korelasi
double calculateRSquared(double* y_actual, double* y_pred, int n) {
    double y_mean = mean(y_actual, n);
    double ss_total = 0.0, ss_residual = 0.0;

    for (int i = 0; i < n; i++) {
        ss_total += pow(y_actual[i] - y_mean, 2);
        ss_residual += pow(y_actual[i] - y_pred[i], 2);
    }

    return 1.0 - (ss_residual / ss_total);
}

PolynomialModel fitPolynomialRegression(double* x, double* y, int n, int degree) {
    PolynomialModel model;

    // set koefisien awal dulu ke 0
    for (int i = 0; i <= degree; i++) {
        model.coeffs[i] = 0.0;
    }

    double X[MAX_ROWS][POLYNOMIAL_DEGREE + 1];
    double X_T[POLYNOMIAL_DEGREE + 1][MAX_ROWS];
    double X_T_X[POLYNOMIAL_DEGREE + 1][POLYNOMIAL_DEGREE + 1];
    double X_T_y[POLYNOMIAL_DEGREE + 1];
    double augmented[POLYNOMIAL_DEGREE + 1][POLYNOMIAL_DEGREE + 2];
    double y_pred[MAX_ROWS];

    // isi X pakai polinomial term
    for (int i = 0; i < n; i++) {
        X[i][0] = 1.0;  
        for (int j = 1; j <= degree; j++) {
            X[i][j] = pow(x[i], j);
        }
    }

    // Transpose of X
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= degree; j++) {
            X_T[j][i] = X[i][j];
        }
    }

    // X_T * X
    for (int i = 0; i <= degree; i++) {
        for (int j = 0; j <= degree; j++) {
            X_T_X[i][j] = 0.0;
            for (int k = 0; k < n; k++) {
                X_T_X[i][j] += X_T[i][k] * X[k][j];
            }
        }
    }

    // X_T * y
    for (int i = 0; i <= degree; i++) {
        X_T_y[i] = 0.0;
        for (int j = 0; j < n; j++) {
            X_T_y[i] += X_T[i][j] * y[j];
        }
    }

    // metode gauss
    // augmented matrix [X_T_X | X_T_y]
    for (int i = 0; i <= degree; i++) {
        for (int j = 0; j <= degree; j++) {
            augmented[i][j] = X_T_X[i][j];
        }
        augmented[i][degree + 1] = X_T_y[i];
    }

    // kita pakai eliminasi gauss (Kode ini pernah dipakai untuk pengerjaan tugas pemrograman KOMNUM pre-UTS)
    for (int i = 0; i <= degree; i++) {
        // cari pivot
        int max_row = i;
        double max_val = fabs(augmented[i][i]);
        for (int j = i + 1; j <= degree; j++) {
            if (fabs(augmented[j][i]) > max_val) {
                max_val = fabs(augmented[j][i]);
                max_row = j;
            }
        }

        // tukar baris jika perlu
        if (max_row != i) {
            for (int j = 0; j <= degree + 1; j++) {
                double temp = augmented[i][j];
                augmented[i][j] = augmented[max_row][j];
                augmented[max_row][j] = temp;
            }
        }

        // eliminasi yang bawah
        for (int j = i + 1; j <= degree; j++) {
            double factor = augmented[j][i] / augmented[i][i];
            for (int k = i; k <= degree + 1; k++) {
                augmented[j][k] -= factor * augmented[i][k];
            }
        }
    }

    // back substitution
    for (int i = degree; i >= 0; i--) {
        model.coeffs[i] = augmented[i][degree + 1];
        for (int j = i + 1; j <= degree; j++) {
            model.coeffs[i] -= augmented[i][j] * model.coeffs[j];
        }
        model.coeffs[i] /= augmented[i][i];
    }

    // kalkulasi korelasi
    for (int i = 0; i < n; i++) {
        y_pred[i] = model.coeffs[0];
        for (int j = 1; j <= degree; j++) {
            y_pred[i] += model.coeffs[j] * pow(x[i], j);
        }
    }

    model.r_squared = calculateRSquared(y, y_pred, n);

    return model;
}

// prediksi menggunakan polinomial
double predictPolynomial(PolynomialModel model, double x, int degree) {
    double result = model.coeffs[0];
    for (int i = 1; i <= degree; i++) {
        result += model.coeffs[i] * pow(x, i);
    }
    return result;
}

double linearInterpolate(double x, double x0, double y0, double x1, double y1) {
    if (x1 == x0) {
        return (y0 + y1) / 2; // handling agar tidak dibagi oleh 0
    }
    return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}

// untuk mencari titik terdekat agar dapat melakukan interpolasi
void findNearestPoints(double target_year, double* years, double* values, int n,
                      double* x0, double* y0, double* x1, double* y1) {
    // inisiasi pakai nilai yang sangat jauh
    double min_dist_before = 1e9;
    double min_dist_after = 1e9;
    int idx_before = -1;
    int idx_after = -1;

    // cari titik terdekat sebelum dan sesudah tahun target
    for (int i = 0; i < n; i++) {
        if (years[i] < target_year) {
            double dist = target_year - years[i];
            if (dist < min_dist_before) {
                min_dist_before = dist;
                idx_before = i;
            }
        } else if (years[i] > target_year) {
            double dist = years[i] - target_year;
            if (dist < min_dist_after) {
                min_dist_after = dist;
                idx_after = i;
            }
        }
    }

    // jika kita menemukan titik sebelum dan sesudah
    if (idx_before != -1 && idx_after != -1) {
        *x0 = years[idx_before];
        *y0 = values[idx_before];
        *x1 = years[idx_after];
        *y1 = values[idx_after];
    }
    // jika kita hanya menemukan titik sesudah
    else if (idx_before == -1 && idx_after != -1) {
        // cari titik terdekat kedua setelah tahun target
        double second_min_dist = 1e9;
        int second_idx = -1;
        for (int i = 0; i < n; i++) {
            if (i != idx_after && years[i] > target_year) {
                double dist = years[i] - target_year;
                if (dist < second_min_dist) {
                    second_min_dist = dist;
                    second_idx = i;
                }
            }
        }
        // jika kita menemukan titik kedua setelah tahun target
        if (second_idx != -1) {
            // pakai titik setelah dan titik kedua setelah
            *x0 = years[idx_after];
            *y0 = values[idx_after];
            *x1 = years[second_idx];
            *y1 = values[second_idx];
        } else {
            // jika kita tidak menemukan titik kedua setelah tahun target, maka kita pakai titik setelah dua kali
            *x0 = years[idx_after];
            *y0 = values[idx_after];
            *x1 = years[idx_after];
            *y1 = values[idx_after];
        }
    }
    // jika kita hanya menemukan titik sebelum
    else if (idx_before != -1 && idx_after == -1) {
        // cari titik terdekat kedua sebelum tahun target
        double second_min_dist = 1e9;
        int second_idx = -1;
        for (int i = 0; i < n; i++) {
            if (i != idx_before && years[i] < target_year) {
                double dist = target_year - years[i];
                if (dist < second_min_dist) {
                    second_min_dist = dist;
                    second_idx = i;
                }
            }
        }

        if (second_idx != -1) {
            // pakai titik sebelum dan titik kedua sebelum
            *x0 = years[idx_before];
            *y0 = values[idx_before];
            *x1 = years[second_idx];
            *y1 = values[second_idx];
        } else {
            // jika kita tidak menemukan titik kedua sebelum tahun target, maka kita pakai titik sebelum dua kali
            *x0 = years[idx_before];
            *y0 = values[idx_before];
            *x1 = years[idx_before];
            *y1 = values[idx_before];
        }
    }
    // jika kita tidak menemukan titik sebelum maupun sesudah, maka kita pakai titik target sebagai error handler
    else {
        *x0 = target_year;
        *y0 = 0;
        *x1 = target_year;
        *y1 = 0;
    }
}

// GENERATE OLEH AI :
// Function to generate data points for plotting
void generatePlotData(double* x, double* y, int n, PolynomialModel model, int degree,
                     const char* filename, int start_year, int end_year) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error creating file: %s\n", filename);
        return;
    }

    // Write original data points
    fprintf(file, "# Original data points\n");
    for (int i = 0; i < n; i++) {
        fprintf(file, "%f %f\n", x[i] + BASE_YEAR, y[i]);
    }

    fprintf(file, "\n\n# Regression curve\n");
    // Generate points for the regression curve
    for (int year = start_year; year <= end_year; year++) {
        double x_norm = year - BASE_YEAR;
        double y_pred = predictPolynomial(model, x_norm, degree);
        fprintf(file, "%d %f\n", year, y_pred);
    }

    fclose(file);
}

// GENERATE OLEH AI:
// Function to generate gnuplot script
void generateGnuplotScript(const char* data_file, const char* script_file, const char* output_file,
                          const char* title, const char* xlabel, const char* ylabel,
                          int missing_years[], int n_missing) {
    FILE* file = fopen(script_file, "w");
    if (file == NULL) {
        printf("Error creating gnuplot script: %s\n", script_file);
        return;
    }

    fprintf(file, "set terminal png size 800,600\n");
    fprintf(file, "set output '%s'\n", output_file);
    fprintf(file, "set title '%s'\n", title);
    fprintf(file, "set xlabel '%s'\n", xlabel);
    fprintf(file, "set ylabel '%s'\n", ylabel);
    fprintf(file, "set grid\n");
    fprintf(file, "set key outside\n");

    // Plot regression curve and original data
    fprintf(file, "plot '%s' index 0 using 1:2 with points pointtype 7 pointsize 1 title 'Data Asli', \\\n", data_file);
    fprintf(file, "     '%s' index 1 using 1:2 with lines lw 2 title 'Kurva Regresi'", data_file);

    // Add vertical lines for missing years
    for (int i = 0; i < n_missing; i++) {
        fprintf(file, ", \\\n     %d notitle with lines lt 0 lw 1 dashtype 2", missing_years[i]);
    }

    fprintf(file, "\n");
    fclose(file);
}

// GENERATE OLEH AI
// Function to generate interpolation plot data
void generateInterpolationPlotData(double* years, double* values, int n,
                                 int missing_years[], double interpolated_values[], int n_missing,
                                 const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error creating file: %s\n", filename);
        return;
    }

    // Write original data points
    fprintf(file, "# Original data points\n");
    for (int i = 0; i < n; i++) {
        fprintf(file, "%f %f\n", years[i], values[i]);
    }

    fprintf(file, "\n\n# Interpolated points\n");
    // Write interpolated points
    for (int i = 0; i < n_missing; i++) {
        fprintf(file, "%d %f\n", missing_years[i], interpolated_values[i]);
    }

    fclose(file);
}

// GENERATE OLEH AI
// Function to generate gnuplot script for interpolation
void generateInterpolationGnuplotScript(const char* data_file, const char* script_file, const char* output_file,
                                      const char* title, const char* xlabel, const char* ylabel) {
    FILE* file = fopen(script_file, "w");
    if (file == NULL) {
        printf("Error creating gnuplot script: %s\n", script_file);
        return;
    }

    fprintf(file, "set terminal png size 800,600\n");
    fprintf(file, "set output '%s'\n", output_file);
    fprintf(file, "set title '%s'\n", title);
    fprintf(file, "set xlabel '%s'\n", xlabel);
    fprintf(file, "set ylabel '%s'\n", ylabel);
    fprintf(file, "set grid\n");
    fprintf(file, "set key outside\n");

    // Plot original data and interpolated points
    fprintf(file, "plot '%s' index 0 using 1:2 with linespoints pointtype 7 pointsize 1 title 'Data Asli', \\\n", data_file);
    fprintf(file, "     '%s' index 1 using 1:2 with points pointtype 9 pointsize 1.5 title 'Nilai Interpolasi'\n", data_file);
    fclose(file);
}

int main() {
    DataRow data[MAX_ROWS];
    int row_count = readCSV("Data Tugas Pemrograman A.csv", data);
    if (row_count == 0) {
        printf("No data read from file.\n");
        return 1;
    }
    double x_pop[MAX_ROWS], y_pop[MAX_ROWS];
    int n_pop = 0;
    for (int i = 0; i < row_count; i++) {
        if (data[i].population > 0) {
            x_pop[n_pop] = data[i].year - BASE_YEAR;
            y_pop[n_pop] = data[i].population;
            n_pop++;
        }
    }
    // inisiasi fit population model menggunakan data yang sudah terbaca dari .csv tadi
    PolynomialModel pop_model = fitPolynomialRegression(x_pop, y_pop, n_pop, POLYNOMIAL_DEGREE);
    double x_internet[MAX_ROWS], y_internet[MAX_ROWS];
    int n_internet = 0;
    for (int i = 0; i < row_count; i++) {
        if (data[i].year >= 1994 && data[i].internet_percentage >= 0) {  // karena data internet mulai ada di tahun 1994, selebihnya 0
            x_internet[n_internet] = data[i].year - BASE_YEAR;
            y_internet[n_internet] = data[i].internet_percentage;
            n_internet++;
        }
    }
    // inisiasi fit internet model menggunakan data yang sudah terbaca dari .csv tadi
    PolynomialModel internet_model = fitPolynomialRegression(x_internet, y_internet, n_internet, POLYNOMIAL_DEGREE);
    double years_pop[MAX_ROWS], values_pop[MAX_ROWS];
    int n_pop_interp = 0;
    for (int i = 0; i < row_count; i++) {
        if (data[i].population > 0) {
            // skip tahun yang ada di soal, karena memang tidak ada
            if (data[i].year != 2005 && data[i].year != 2006 &&
                data[i].year != 2015 && data[i].year != 2016) {
                years_pop[n_pop_interp] = data[i].year;
                values_pop[n_pop_interp] = data[i].population;
                n_pop_interp++;
            }
        }
    }
    double years_internet[MAX_ROWS], values_internet[MAX_ROWS];
    int n_internet_interp = 0;
    for (int i = 0; i < row_count; i++) {
        if (data[i].year >= 1994 && data[i].internet_percentage >= 0) {
            // skip tahun yang ada di soal, karena memang tidak ada
            if (data[i].year != 2005 && data[i].year != 2006 &&
                data[i].year != 2015 && data[i].year != 2016) {
                years_internet[n_internet_interp] = data[i].year;
                values_internet[n_internet_interp] = data[i].internet_percentage;
                n_internet_interp++;
            }
        }
    }
    printf("==================================================\n");
    printf("HASIL ESTIMASI NILAI YANG HILANG (INTERPOLASI)\n");
    printf("==================================================\n\n");
    printf("1. Estimasi Jumlah Penduduk Indonesia:\n");
    int missing_years_pop[] = {2005, 2006, 2015, 2016};
    for (int i = 0; i < 4; i++) {
        double x0, y0, x1, y1;
        findNearestPoints(missing_years_pop[i], years_pop, values_pop, n_pop_interp, &x0, &y0, &x1, &y1);
        double interpolated = linearInterpolate(missing_years_pop[i], x0, y0, x1, y1);

        printf("   %d: %.0f jiwa (interpolasi antara %.0f dan %.0f)\n",
               missing_years_pop[i], interpolated, x0, x1);
    }
    printf("\n2. Estimasi Persentase Pengguna Internet Indonesia:\n");
    int missing_years_internet[] = {2005, 2006, 2015, 2016};
    for (int i = 0; i < 4; i++) {
        double x0, y0, x1, y1;
        findNearestPoints(missing_years_internet[i], years_internet, values_internet, n_internet_interp, &x0, &y0, &x1, &y1);
        double interpolated = linearInterpolate(missing_years_internet[i], x0, y0, x1, y1);

        printf("   %d: %.4f%% (interpolasi antara %.0f dan %.0f)\n",
               missing_years_internet[i], interpolated, x0, x1);
    }
    printf("\n==================================================\n");
    printf("PERSAMAAN POLINOMIAL (UNTUK ESTIMASI MASA DEPAN)\n");
    printf("==================================================\n\n");
    printf("1. Persamaan untuk Pertumbuhan Populasi Indonesia:\n");
    printf("   y = %.6f", pop_model.coeffs[0]);
    for (int i = 1; i <= POLYNOMIAL_DEGREE; i++) {
        if (pop_model.coeffs[i] >= 0) {
            printf(" + %.6f(Year - 1960)^%d", pop_model.coeffs[i], i);
        } else {
            printf(" - %.6f(Year - 1960)^%d", fabs(pop_model.coeffs[i]), i);
        }
    }
    printf("\n   R^2 = %.6f\n", pop_model.r_squared);
    printf("\n2. Persamaan untuk Persentase Pengguna Internet Indonesia:\n");
    printf("   y = %.6f", internet_model.coeffs[0]);
    for (int i = 1; i <= POLYNOMIAL_DEGREE; i++) {
        if (internet_model.coeffs[i] >= 0) {
            printf(" + %.6f(Year - 1960)^%d", internet_model.coeffs[i], i);
        } else {
            printf(" - %.6f(Year - 1960)^%d", fabs(internet_model.coeffs[i]), i);
        }
    }
    printf("\n   R^2 = %.6f\n", internet_model.r_squared);
    printf("\n==================================================\n");
    printf("ESTIMASI MASA DEPAN\n");
    printf("==================================================\n\n");
    double pop_2030 = predictPolynomial(pop_model, 2030 - BASE_YEAR, POLYNOMIAL_DEGREE);
    printf("1. Estimasi Jumlah Penduduk Indonesia tahun 2030:\n");
    printf("   %.0f jiwa\n", pop_2030);
    double pop_2035 = predictPolynomial(pop_model, 2035 - BASE_YEAR, POLYNOMIAL_DEGREE);
    double internet_percentage_2035 = predictPolynomial(internet_model, 2035 - BASE_YEAR, POLYNOMIAL_DEGREE);
    printf("\n2. Estimasi Jumlah Pengguna Internet Indonesia tahun 2035:\n");
    if (internet_percentage_2035 > 100) {
        printf("   Persentase (uncapped): %.4f%%\n", internet_percentage_2035);
        internet_percentage_2035 = 100;
        printf("   Persentase (capped): %.4f%%\n", internet_percentage_2035);
    } else {
        printf("   Persentase: %.4f%%\n", internet_percentage_2035);
    }
    double internet_users_2035 = (internet_percentage_2035 / 100) * pop_2035;
    printf("   Populasi: %.0f jiwa\n", pop_2035);
    printf("   Jumlah Pengguna Internet: %.0f jiwa\n", internet_users_2035);
    printf("\n==================================================\n");
    printf("MEMBUAT GRAFIK\n");
    printf("==================================================\n");
    generatePlotData(x_pop, y_pop, n_pop, pop_model, POLYNOMIAL_DEGREE,
                    "population_regression.dat", 1960, 2035);

    generateGnuplotScript("population_regression.dat", "population_regression.plt", "population_regression.png",
                         "Pertumbuhan Populasi Indonesia", "Tahun", "Jumlah Penduduk",
                         missing_years_pop, 4);
    printf("\nData regresi populasi disimpan di: population_regression.dat\n");
    printf("Script gnuplot disimpan di: population_regression.plt\n");
    printf("Untuk menghasilkan grafik, jalankan: gnuplot population_regression.plt\n");
    generatePlotData(x_internet, y_internet, n_internet, internet_model, POLYNOMIAL_DEGREE,
                    "internet_regression.dat", 1994, 2035);

    generateGnuplotScript("internet_regression.dat", "internet_regression.plt", "internet_regression.png",
                         "Persentase Pengguna Internet Indonesia", "Tahun", "Persentase (%)",
                         missing_years_internet, 4);
    printf("\nData regresi internet disimpan di: internet_regression.dat\n");
    printf("Script gnuplot disimpan di: internet_regression.plt\n");
    printf("Untuk menghasilkan grafik, jalankan: gnuplot internet_regression.plt\n");
    double pop_interpolated[4];
    for (int i = 0; i < 4; i++) {
        double x0, y0, x1, y1;
        findNearestPoints(missing_years_pop[i], years_pop, values_pop, n_pop_interp, &x0, &y0, &x1, &y1);
        pop_interpolated[i] = linearInterpolate(missing_years_pop[i], x0, y0, x1, y1);
    }
    generateInterpolationPlotData(years_pop, values_pop, n_pop_interp,
                                missing_years_pop, pop_interpolated, 4,
                                "population_interpolation.dat");
    generateInterpolationGnuplotScript("population_interpolation.dat", "population_interpolation.plt",
                                     "population_interpolation.png",
                                     "Interpolasi Populasi Indonesia", "Tahun", "Jumlah Penduduk");
    printf("\nData interpolasi populasi disimpan di: population_interpolation.dat\n");
    printf("Script gnuplot disimpan di: population_interpolation.plt\n");
    printf("Untuk menghasilkan grafik, jalankan: gnuplot population_interpolation.plt\n");
    double internet_interpolated[4];
    for (int i = 0; i < 4; i++) {
        double x0, y0, x1, y1;
        findNearestPoints(missing_years_internet[i], years_internet, values_internet, n_internet_interp, &x0, &y0, &x1, &y1);
        internet_interpolated[i] = linearInterpolate(missing_years_internet[i], x0, y0, x1, y1);
    }
    generateInterpolationPlotData(years_internet, values_internet, n_internet_interp,
                                missing_years_internet, internet_interpolated, 4,
                                "internet_interpolation.dat");
    generateInterpolationGnuplotScript("internet_interpolation.dat", "internet_interpolation.plt",
                                     "internet_interpolation.png",
                                     "Interpolasi Persentase Pengguna Internet Indonesia", "Tahun", "Persentase (%)");
    printf("\nData interpolasi internet disimpan di: internet_interpolation.dat\n");
    printf("Script gnuplot disimpan di: internet_interpolation.plt\n");
    printf("Untuk menghasilkan grafik, jalankan: gnuplot internet_interpolation.plt\n");

        
    FILE* batch_file = fopen("generate_plots.bat", "w");
    if (batch_file != NULL) {
        fprintf(batch_file, "gnuplot population_regression.plt\n");
        fprintf(batch_file, "gnuplot internet_regression.plt\n");
        fprintf(batch_file, "gnuplot population_interpolation.plt\n");
        fprintf(batch_file, "gnuplot internet_interpolation.plt\n");
        fclose(batch_file);

        printf("\nUntuk menghasilkan semua grafik sekaligus, jalankan: generate_plots.bat\n");
    }

    return 0;
}
