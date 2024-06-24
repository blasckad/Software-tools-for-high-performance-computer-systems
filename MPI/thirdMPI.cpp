#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Функция f(x, y)
double f(double x, double y) {
    // Замените эту функцию на необходимую для вашей задачи
    return sin(x) * cos(y);
}

// Функция для вычисления производной по x
double dfdx(double x, double y, double h) {
    return (f(x + h, y) - f(x - h, y)) / (2 * h);
}

// Функция для вычисления производной по y
double dfdy(double x, double y, double h) {
    return (f(x, y + h) - f(x, y - h)) / (2 * h);
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Размеры сетки
    const int N = 1000; // Количество точек по x
    const int M = 1000; // Количество точек по y
    const double h = 0.001; // Шаг сетки

    // Инициализация сетки
    double *A = (double *)malloc(N * M * sizeof(double));
    double *Bx = (double *)malloc(N * M * sizeof(double));
    double *By = (double *)malloc(N * M * sizeof(double));

    double start_time = MPI_Wtime();

    // Заполнение массива A значениями функции f(x, y)
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            double x = i * h;
            double y = j * h;
            A[i * M + j] = f(x, y);
        }
    }

    // Распределение работы между процессами
    int rows_per_process = N / size;
    int start_row = rank * rows_per_process;
    int end_row = (rank + 1) * rows_per_process;
    if (rank == size - 1) {
        end_row = N; // Последний процесс должен обработать все оставшиеся строки
    }

    // Вычисление производных для каждой строки назначенной данному процессу
    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < M; ++j) {
            double x = i * h;
            double y = j * h;
            Bx[i * M + j] = dfdx(x, y, h);
            By[i * M + j] = dfdy(x, y, h);
        }
    }

    // Сбор результатов с разных процессов
    double *global_Bx = NULL;
    double *global_By = NULL;
    if (rank == 0) {
        global_Bx = (double *)malloc(N * M * sizeof(double));
        global_By = (double *)malloc(N * M * sizeof(double));
    }
    MPI_Gather(Bx, rows_per_process * M, MPI_DOUBLE, global_Bx, rows_per_process * M, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Gather(By, rows_per_process * M, MPI_DOUBLE, global_By, rows_per_process * M, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    // Вывод результата на экран только на главном процессе
    if (rank == 0) {
        // Вывод значений производных для всех точек функции
        // for (int i = 0; i < N; ++i) {
        //     for (int j = 0; j < M; ++j) {
        //         double x = i * h;
        //         double y = j * h;
        //         printf("df/dx at (%f, %f) = %f\n", x, y, global_Bx[i * M + j]);
        //         printf("df/dy at (%f, %f) = %f\n", x, y, global_By[i * M + j]);
        //     }
        // }
        printf("Time taken to compute the sum: %f seconds\n", end_time - start_time);
        // Освобождение памяти, выделенной для глобальных массивов
        free(global_Bx);
        free(global_By);
    }

    // Освобождение памяти, выделенной для локальных массивов
    free(A);
    free(Bx);
    free(By);

    MPI_Finalize();
    return 0;
}