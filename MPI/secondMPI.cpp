#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Инициализация массива чисел
    const int array_size = 10000000; // Большой массив для замера времени
    int *numbers = (int *)malloc(array_size * sizeof(int));
    int local_sum = 0;
    int global_sum = 0;

    // Заполнение массива
    for (int i = 0; i < array_size; ++i) {
        numbers[i] = i + 1;
    }

    // Засекаем время перед распределением работы
    double start_time = MPI_Wtime();

    // Распределение работы между процессами
    int elements_per_process = array_size / size;
    int start_index = rank * elements_per_process;
    int end_index = (rank + 1) * elements_per_process;

    // Вычисление суммы локальной части массива
    for (int i = start_index; i < end_index; ++i) {
        local_sum += numbers[i];
    }

    // Обмен результатами между процессами
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Засекаем время после вычисления суммы
    double end_time = MPI_Wtime();

    // Освобождение памяти
    free(numbers);

    // Вывод результата на экран только на главном процессе
    if (rank == 0) {
        printf("The sum of the array elements is: %d\n", global_sum);
        printf("Time taken to compute the sum: %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}