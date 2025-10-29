#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MATRIX_DATA_RANGE 256

void swap(int *a, int *b);
void generateRandomMatrix(int **grid, int dimension);
void printMatrix(int **grid, int dimension);
void rotateMatrix90degClockwise(int **grid, int dimension);
void applySmoothingFilter(int **grid, int dimension);

int main() {
  int matrixDimension;

  printf("Enter matrix size (2-10): ");
  scanf("%d", &matrixDimension);
  if (matrixDimension < 2 || matrixDimension > 10) {
    printf("Matrix size must be within the range (2-10).");
    return 1;
  }

  srand(time(NULL));
  int **matrixData = (int **)malloc(matrixDimension * sizeof(int *));
  if (matrixData == NULL) {
    printf("Memory allocation failed.");
    return 1;
  }


  for (int i = 0; i < matrixDimension; i++) {
    *(matrixData + i) = (int *)malloc(matrixDimension * sizeof(int));
  }

  generateRandomMatrix(matrixData, matrixDimension);

  printf("Original Randomly Generated Matrix:\n");
  printMatrix(matrixData, matrixDimension);

  rotateMatrix90degClockwise(matrixData, matrixDimension);
  printf("Matrix after 90° Clockwise Rotation:\n");
  printMatrix(matrixData, matrixDimension);

  applySmoothingFilter(matrixData, matrixDimension);
  printf("Matrix after Applying 3x3 Smoothing Filter:\n");
  printMatrix(matrixData, matrixDimension);

  for (int i = 0; i < matrixDimension; i++) {
    free(*(matrixData + i));
  }
  free(matrixData);

  return 0;
}

void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

void generateRandomMatrix(int **grid, int dimension) {
  for (int row = 0; row < dimension; row++) {
    for (int col = 0; col < dimension; col++) {
      *(*(grid + row) + col) = rand() % MATRIX_DATA_RANGE;
    }
  }
}

void printMatrix(int **grid, int dimension) {
  for (int row = 0; row < dimension; row++) {
    for (int col = 0; col < dimension; col++) {
      printf("%3d  ", *(*(grid + row) + col));
    }
    printf("\n");
  }
  printf("\n");
}

void rotateMatrix90degClockwise(int **grid, int dimension) {
  for (int i = 0; i < dimension; i++) {
    for (int j = i + 1; j < dimension; j++) {
      swap(*(grid + i) + j, *(grid + j) + i);
    }
  }

  for (int row = 0; row < dimension; row++) {
    for (int col = 0; col < dimension / 2; col++) {
      swap(*(grid + row) + col, *(grid + row) + dimension - 1 - col);
    }
  }
}


void applySmoothingFilter(int **grid, int dimension) {
  for (int row = 0; row < dimension; row++) {
    for (int col = 0; col < dimension; col++) {
      int sum = 0, count = 0;

      for (int k_row = row - 1; k_row <= row + 1; k_row++) {
        for (int k_col = col - 1; k_col <= col + 1; k_col++) {
          if (k_row >= 0 && k_row < dimension && k_col >= 0 && k_col < dimension) {
            sum += *(*(grid + k_row) + k_col) % MATRIX_DATA_RANGE;
            count++;
          }
        }
      }

      *(*(grid + row) + col) |= (sum / count << 8);
    }
  }

  for (int row = 0; row < dimension; row++) {
    for (int col = 0; col < dimension; col++) {
      *(*(grid + row) + col) >>= 8;
    }
  }
}
