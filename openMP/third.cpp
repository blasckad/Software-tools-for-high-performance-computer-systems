#include <iostream>
#include <cmath>
#include <omp.h>
#include <chrono>

using namespace std;

// Определение функции f(x, y)
double f(double x, double y) {
  return sin(x) * cos(y); // Пример нетривиальной функции
}

// Функция для вычисления производной по x
double dfdx(double x, double y, double h) {
  return (f(x + h, y) - f(x, y)) / h;
}

// Функция для вычисления производной по y
double dfdy(double x, double y, double h) {
  return (f(x, y + h) - f(x, y)) / h;
}

int main() {
  // Размер сетки
  const int N = 10;

  // Шаг сетки
  const double h = 0.1;

  // Двумерный массив для функции f(x, y)
  double A[N][N];

  // Двумерный массив для производной df(x, y)/dx
  double B[N][N];

  auto start_time = std::chrono::high_resolution_clock::now();

  // Заполнение массива A значениями функции f(x, y)
  #pragma omp parallel for collapse(2)
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      A[i][j] = f(i * h, j * h);
    }
  }

  // Вычисление производной df(x, y)/dx с использованием OpenMP
  #pragma omp parallel for collapse(2)
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      B[i][j] = dfdx(i * h, j * h, h);
    }
  }

  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

  std::cout << "Время работы программы: " << duration.count() << " микросекунд" << std::endl;

//   Вывод результатов
//   cout << "Массив A (f(x, y)):" << endl;
//   for (int i = 0; i < N; i++) {
//     for (int j = 0; j < N; j++) {
//       cout << A[i][j] << " ";
//     }
//     cout << endl;
//   }

//   cout << "Массив B (df(x, y)/dx):" << endl;
//   for (int i = 0; i < N; i++) {
//     for (int j = 0; j < N; j++) {
//       cout << B[i][j] << " ";
//     }
//     cout << endl;
//   }

  return 0;
}