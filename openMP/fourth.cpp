#include <iostream>
#include <omp.h>
#include <chrono>

using namespace std;

// Функция для умножения двух матриц с использованием OpenMP
void multiplyMatrices(int **A, int **B, int **C, int rowsA, int colsA, int colsB) {
  #pragma omp parallel for
  for (int i = 0; i < rowsA; i++) {
    for (int j = 0; j < colsB; j++) {
      C[i][j] = 0;
      for (int k = 0; k < colsA; k++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

int main() {
  // Размер матриц
  int rowsA = 1000;
  int colsA = 1000;
  int colsB = 1000;

  // Выделение памяти для матриц
  int **A = new int*[rowsA];
  int **B = new int*[colsA];
  int **C = new int*[rowsA];

  for (int i = 0; i < rowsA; i++) {
    A[i] = new int[colsA];
  }

  for (int i = 0; i < colsA; i++) {
    B[i] = new int[colsB];
  }

  for (int i = 0; i < rowsA; i++) {
    C[i] = new int[colsB];
  }

  // Инициализация матриц случайными значениями
  srand(time(0));
  for (int i = 0; i < rowsA; i++) {
    for (int j = 0; j < colsA; j++) {
      A[i][j] = rand() % 10;
    }
  }

  for (int i = 0; i < colsA; i++) {
    for (int j = 0; j < colsB; j++) {
      B[i][j] = rand() % 10;
    }
  }

  auto start_time = std::chrono::high_resolution_clock::now();

  // Вычисление произведения матриц
  multiplyMatrices(A, B, C, rowsA, colsA, colsB);

  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

  std::cout << "Время работы программы: " << duration.count() << " микросекунд" << std::endl;

  // Вывод результата
//   cout << "Матрица A:" << endl;
//   for (int i = 0; i < rowsA; i++) {
//     for (int j = 0; j < colsA; j++) {
//       cout << A[i][j] << " ";
//     }
//     cout << endl;
//   }

//   cout << "Матрица B:" << endl;
//   for (int i = 0; i < colsA; i++) {
//     for (int j = 0; j < colsB; j++) {
//       cout << B[i][j] << " ";
//     }
//     cout << endl;
//   }

//   cout << "Матрица C (A * B):" << endl;
//   for (int i = 0; i < rowsA; i++) {
//     for (int j = 0; j < colsB; j++) {
//       cout << C[i][j] << " ";
//     }
//     cout << endl;
//   }

  // Освобождение памяти
  for (int i = 0; i < rowsA; i++) {
    delete[] A[i];
  }
  delete[] A;

  for (int i = 0; i < colsA; i++) {
    delete[] B[i];
  }
  delete[] B;

  for (int i = 0; i < rowsA; i++) {
    delete[] C[i];
  }
  delete[] C;

  return 0;
}