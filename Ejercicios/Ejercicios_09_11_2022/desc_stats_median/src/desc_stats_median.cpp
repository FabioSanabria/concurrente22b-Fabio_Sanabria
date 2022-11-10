// Copyright 2022 Fabio Sanabria <fabio.sanabria@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <bits/stdc++.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "math.h"

double maximo(std::vector<double> array, size_t n,
int64_t thread_count);

double minimo(std::vector<double> array, size_t n,
int64_t thread_count);

void imprimir(std::vector<double> array, size_t n);

double average(std::vector<double> array,
int64_t thread_count);

double desviacionS(std::vector<double> array,
int64_t thread_count, double promedio);

void ordenamiento(size_t n, double array[],
int64_t thread_count);

int main(int argc, char* argv[]) {
  int max, min;
  double promedio = 0.0;
  double desviacion = 0.0;
  int64_t thread_count = 0;
  double value = 0;  // data entered by the user
  // If the data isn't a number,
  std::vector<double> array;

    if (argc == 2) {
        if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {
            // nothing
        }
    }
  std::cout << "Dele ctrl + d para detener el pedido de numeros\n";
  while (fscanf(stdin, "%lf" , &value) == 1) {
    // add all the data to an array of array
    array.push_back(value);
  }
    if (array.empty()) {
        return 0;
    }

    double arr[array.size()];
    for (size_t index = 0; index < array.size(); index++) {
        arr[index] = array[index];
    }

    max = maximo(array, array.size(), thread_count);
    min = minimo(array, array.size(), thread_count);
    promedio = average(array, thread_count);
    desviacion = desviacionS(array, thread_count, promedio);
    ordenamiento(array.size(), arr, thread_count);

    size_t index = (array.size() / 2) - 1;

    double mediana = arr[index];
    std::cout << "\nVector: "; imprimir(array, array.size());
    std::cout << "\nMaximo: " << max;
    std::cout << "\nminimo: " << min;
    std::cout << "\nPromedio: " << promedio;
    std::cout << "\nMediana: " << mediana;
    std::cout << "\nDesviacion: " << desviacion << std::endl;
    return 0;
}

double maximo(std::vector<double> array, size_t n,
int64_t thread_count) {
    double max = array[0];
    size_t i;
    #pragma omp parallel num_threads(thread_count) \
    default(none) shared(array, n, max) private(i)
    {
        #pragma omp for
        for (i = 1; i < n; i++) {
            if (max < array[i]) {
                #pragma omp critical(can_change)
                max = array[i];
            }
        }
    }
    return max;
}

double minimo(std::vector<double> array, size_t n,
int64_t thread_count) {
    double min = array[0];
    size_t index;
    #pragma omp parallel num_threads(thread_count) \
    default(none) shared(array, n, min) private(index)
    {
        #pragma omp for
        for (index = 1; index < n; ++index) {
            if (min > array[index]) {
                #pragma omp critical(can_min)
                min = array[index];
             }
        }
    }
    return min;
}

double average(std::vector<double> array,
int64_t thread_count) {
    double sum = 0;
  #pragma omp parallel for num_threads(thread_count) schedule(runtime) \
    default(none) shared(array) reduction(+:sum)
  // Aqui va sumando todos los valores ingresados por el
  // usuario para luego sacar el promedio, como todos los
  // hilos estan tocando una region compartida se debe de
  // colocar un mutex para poder proteger el codigo de condiciones
  // de carrera
  for (size_t index = 0; index < array.size(); ++index) {
    // Mutex que protege de race_condition
    sum += array[index];
  }

  const double average = array.size() ? sum / array.size() : 0.0;
    return average;
}

double desviacionS(std::vector<double> array,
int64_t thread_count, double mean) {
    if (array.size() == 0) {
        return 0;
    }
    double sum = 0;
    #pragma omp parallel for num_threads(thread_count) schedule(static) \
    default(none) shared(array, mean) reduction(+:sum)
  for (size_t index = 0; index < array.size(); ++index) {
    // Mutex que protege de race_condition
    sum += (array[index]-mean) * (array[index]-mean);
  }
    double desviacion = sum/ (array.size() - 1);
    desviacion = sqrt(desviacion);
    return desviacion;
}

void ordenamiento(size_t n, double arr[],
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

void imprimir(std::vector<double> array, size_t n) {
    for (size_t i = 0; i < n; i++) {
        std::cout << array[i] << " ";
    }
}
