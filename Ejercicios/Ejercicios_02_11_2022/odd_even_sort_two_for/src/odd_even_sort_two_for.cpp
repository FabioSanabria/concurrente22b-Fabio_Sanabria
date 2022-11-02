// Copyright 2022 Fabio Sanabria <fabio.sanabria@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <bits/stdc++.h>
#include <random>
#include <iostream>

// using namespace std;

// A function to sort the algorithm using Odd Even sort
void serial_odd_even_sort(size_t n, double arr[],
int64_t thread_count);

double fRand(double fMin, double fMax);

void serial_odd_even_sort(size_t n, double arr[],
int64_t thread_team) {
    int64_t thread_count = thread_team;
    size_t i;
    size_t phase;
    #pragma omp parallel num_threads(thread_count) \
    default(none) shared(arr, n) private(i, phase)
    for (phase = 0; phase < n; ++phase) {
        if (phase % 2 == 0) {
            #pragma omp for
            // Perform Bubble sort on odd indexed element
        for (i = 1; i <= n - 2; i = i + 2) {
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
            }
        }
        } else {
            #pragma omp for
            // Perform Bubble sort on even indexed element
            for (i = 0; i <= n - 2; i = i + 2) {
                if (arr[i] > arr[i + 1]) {
                    std::swap(arr[i], arr[i + 1]);
                }
            }
        }
    }
}

// A utility function to print an array of size n
void printArray(double arr[], int n) {
    for (int i = 0; i < n; i++) {
        if (i == n - 1) {
          std::cout << arr[i] << "\n";
        } else {
          std::cout << arr[i] << ", ";
        }
    }
}

double fRand(double fMin, double fMax) {
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

// Driver program to test above functions.
int main(int argc, char* argv[]) {
  srand(time(NULL));
  size_t n = 0;
  int64_t thread_count = 0;
  double time_spent = 0;
  clock_t begin = clock();  // para medir tiempo
  if (argc == 1) {
    std::cout << "argv[2] = numeros, argv[3] = threads" << std::endl;
  } else {
    if (argc == 3) {
        if (sscanf(argv[1], "%" SCNu64, &n) == 1) {
            // nothing
        }
        if (sscanf(argv[2], "%" SCNu64, &thread_count) == 1) {
            // nothing
        } else {
            fprintf(stderr, "Error: invalid size of array\n");
            return EXIT_FAILURE;  // Error code
        }
    } else {
        if (sscanf(argv[1], "%" SCNu64, &n) == 1) {
            // nothing
        }
    }
  }
    double array[n];

    for (size_t i = 0; i < n; i++) {
        array[i] = fRand(0, 100);
    }
    // size_t n = sizeof(arr) / sizeof(arr[0]);
    serial_odd_even_sort(n, array, thread_count);
    printArray(array, n);
    clock_t end = clock();  // fin de la medicion
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Tiempo: %f segundos\n", time_spent);
    return EXIT_SUCCESS;
}
