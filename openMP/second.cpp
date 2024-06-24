#include <iostream>
#include <omp.h>
#include <chrono>

int main() {
  // Размер массива
  const int N = 1000;

  // Создание массива из целых чисел
  int* arr = new int[N];

  // Инициализация массива случайными числами
  for (int i = 0; i < N; ++i) {
    arr[i] = rand() % 1000; // Случайные числа от 0 до 999
  }

  // Замер времени работы
  auto start_time = std::chrono::high_resolution_clock::now();

  // Вычисление суммы с помощью OpenMP
  int sum = 0;
  #pragma omp parallel for reduction(+:sum)
  for (int i = 0; i < N; ++i) {
    sum += arr[i];
  }

  // Замер времени работы
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

  // Вывод результата
  std::cout << "Сумма элементов массива: " << sum << std::endl;
  std::cout << "Время работы программы: " << duration.count() << " микросекунд" << std::endl;

  // Освобождение памяти
  delete[] arr;

  return 0;
}