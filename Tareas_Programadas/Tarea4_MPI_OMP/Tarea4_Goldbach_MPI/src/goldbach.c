// Goldbach_optimizado program v1.3 Fabio Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Sanabria Valerin>
#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>
#include <mpi.h>
#include "array_int64.h"
#include "array_num64.h"
#include "goldbach.h"
#include "calculadora.h"
#include "salida.h"
#include "entrada.h"


// thread_share_data_t
typedef struct shared {
  array_int64_t* array;
  uint64_t thread_count;
  int64_t pos;  // posicion del hilo en el arreglo
  // pthread_mutex_t can_acces;  // verifica si el hilo puede accesar
} shared_data_t;

// thread_private_data_t
typedef struct private {
  shared_data_t* shared_data;
} private_data_t;

/**
 * @brief Subrutina que calcula los numeros primos
 * para cada numero ingresado
 * @param array_primos puntero a objeto de tipo array_primos_t,
 * debe ser distinto a NULL
 * @param num numero al que se le quiere sacar los numeros primos
 * anteriores
 * @return int cantidad de primos
*/
int calcular_primos(array_primos_t* array_primos, int64_t num);

/**
 * @brief Lee los datos ingresados en entrada estandar
 * @param array puntero a objeto de tipo array_int64_t, debe ser distinto a NULL
 * @param file un puntero a un archivo de texto
 * @param argc cantidad de argumentos ingresados enentrada estandar
 * @param argv un arreglo con los datos ingresados de entrada estandar
 * @return un codigo de error
 * EXIT_SUCCESS si se analizaron correctamente los datos
 * EXIT_FAILURE si no se analizan los datos correctamente
*/
int goldbach_recibir_datos(array_int64_t *array, FILE *file, int argc,
char* argv[]);

/**
 * @brief Subrutina que calcula las sumas de goldbach
 * @param elements puntero a objeto de tipo goldbach, debe ser distinto a NULL
 * @return void
*/
void calcular_sumas(goldbach_t* elements);

/** @brief Subrutina encargada de crear los hilos de concurrencia mediante
 * Open MP.
 * Se podra escoger la distribucion a utilizar (dynamic,static,guided)
 * @param array arreglo con los valores a ser procesados
 * @param thread_count cantidad de hilos a ser creados
 * @param task_amount cantidad de numeros a ser procesados
 * @return codigo de error
 * 0 si lo logro
 * 1 si fallo
*/
int create_threads(const array_int64_t* array, size_t thread_count,
size_t task_amount);

/** @brief Subrutina que imprime las sumas de goldbach
 * ya sea que soliciten las sumas o solo la cantidad de sumas
 * @param array puntero a objeto de tipo goldbach, debe ser distinto a NULL
 * @param array_size tamanio que tiene el arreglo de sumas
 * @return void
*/
void goldbach_print(const array_int64_t* array, const uint64_t array_size);

int calcular_primos(array_primos_t* array_primos, int64_t num) {
  int cont2 = 0;
  // inicio subrutina que busca los primos y los mete en un array
      for (int m = 1; m < num; m++) {  // crea los numeros a ser probados
        int contador = 0;
        for (int j = m; j > 0; j--) {  // revisa si es un numero primo
          if (m % j == 0) {  // si es primo entra e incrementa el contador
            contador++;
          }
        }
        if (contador == 2) {
          array_primos_append(array_primos, m);
          cont2++;
          // introduce el numero primo
          // en el array de numeros primos del hilo
        }
      }
      // fin de subrutina
  return cont2;
}

void calcular_sumas(goldbach_t* elements) {
  calcular_sumas_calculadora(elements);
}

void goldbach_run(array_int64_t* goldbach, size_t thread_count, int argc,
 char* argv[], int process_number, int process_count) {
  size_t task_amount = goldbach_recibir_datos(goldbach, stdin, argc, argv);
  double time_spent = 0;
  clock_t begin = clock();  // para medir tiempo
    create_threads(goldbach, thread_count, task_amount);
    if (process_number != 0) {  // process number == 1
      size_t message_size = task_amount;
      if (MPI_Send(&message_size, /*count*/ 1, MPI_INTEGER, 0,
      /*tag*/ process_number, MPI_COMM_WORLD) != MPI_SUCCESS) {
        fprintf(stderr, "error : could not send message\n");
      }
      if (MPI_Send(goldbach, task_amount, MPI_CHAR,
            /*target*/ 0, /*tag*/ process_number,
            MPI_COMM_WORLD) != MPI_SUCCESS) {
                fprintf(stderr, "error : could not send message\n");
      }
  }
  if (process_number == 0) {
    for (int i = 1; i < process_count; i++) {
      size_t counter = i;
      while (counter < task_amount) {
        if (MPI_Recv(&task_amount, 1, MPI_INTEGER, i
            , /*tag*/ i, MPI_COMM_WORLD, MPI_STATUS_IGNORE)
            != MPI_SUCCESS ) {
            fprintf(stderr, "error : could not receive message\n");
        }
        if (MPI_Recv(&goldbach->elements[0], task_amount, MPI_CHAR, i
            , /*tag*/ i, MPI_COMM_WORLD, MPI_STATUS_IGNORE)
            != MPI_SUCCESS ) {
            fprintf(stderr, "error : could not receive message\n");
        }
        counter += process_count;
      }
    }
    goldbach_print(goldbach, task_amount);
    clock_t end = clock();  // fin de la medicion
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Tiempo: %f segundos\n", time_spent);
  }
}

int create_threads(const array_int64_t* array, size_t thread_count,
 size_t task_amount) {
  int error = EXIT_SUCCESS;
  #pragma omp parallel for num_threads(thread_count)\
  schedule(dynamic) shared(array, task_amount)
    for (size_t i = 0; i < task_amount; i++) {
      calcular_sumas(&array->elements[i]);
    }
    return error;
}

int goldbach_recibir_datos(array_int64_t *array, FILE *file, int argc,
char* argv[]) {
  return goldbach_recibir_datos_entrada(array, file, argc, argv);
}

void goldbach_print(const array_int64_t* array, const uint64_t array_size) {
  goldbach_print_salida(array, array_size);
}
