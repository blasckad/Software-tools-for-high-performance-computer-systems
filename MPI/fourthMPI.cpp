#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Функция для выделения памяти для матрицы
int* allocateMatrix(int rows, int cols) {
    return (int*)malloc(rows * cols * sizeof(int));
}

// Функция для освобождения памяти матрицы
void freeMatrix(int* matrix) {
    free(matrix);
}

// Функция для вывода матрицы
void printMatrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%d ", matrix[i * cols + j]);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Размеры матриц
    const int rowsA = 1000;
    const int colsA = 1000;
    const int rowsB = colsA;
    const int colsB = 1000;

    // Инициализация матриц
    int* matrixA = allocateMatrix(rowsA, colsA);
    int* matrixB = allocateMatrix(rowsB, colsB);
    int* local_result = allocateMatrix(rowsA / size, colsB);
    int* result = allocateMatrix(rowsA, colsB);

    // Инициализация генератора случайных чисел
    srand(time(NULL) + rank); // Инициализация генератора случайных чисел с разными seed'ами для каждого процесса

    // Заполнение матриц случайными числами
    if (rank == 0) {
        // Заполнение матрицы A и B случайными числами
        for (int i = 0; i < rowsA * colsA; ++i) {
            matrixA[i] = rand() % 10; // Заполнение числами от 0 до 9
        }
        for (int i = 0; i < rowsB * colsB; ++i) {
            matrixB[i] = rand() % 10; // Заполнение числами от 0 до 9
        }
    }

    // Распространение матриц A и B
    MPI_Bcast(matrixA, rowsA * colsA, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(matrixB, rowsB * colsB, MPI_INT, 0, MPI_COMM_WORLD);

    double start_time = MPI_Wtime();

    // Вычисление локальной части результирующей матрицы
    for (int i = 0; i < rowsA / size; ++i) {
        for (int j = 0; j < colsB; ++j) {
            local_result[i * colsB + j] = 0;
            for (int k = 0; k < colsA; ++k) {
                local_result[i * colsB + j] += matrixA[(i * size + rank) * colsA + k] * matrixB[k * colsB + j];
            }
        }
    }

    // Объединение локальных результатов на процессе с рангом 0
    MPI_Gather(local_result, (rowsA / size) * colsB, MPI_INT,
               result, (rowsA / size) * colsB, MPI_INT,
               0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    // Вывод результата на процессе с рангом 0
    if (rank == 0) {
        // printf("Result matrix:\n");
        // printMatrix(result, rowsA, colsB);
        printf("Time taken to compute the sum: %f seconds\n", end_time - start_time);
        freeMatrix(result);
    }

    // Освобождение памяти
    freeMatrix(matrixA);
    freeMatrix(matrixB);
    freeMatrix(local_result);

    MPI_Finalize();
    return 0;
}