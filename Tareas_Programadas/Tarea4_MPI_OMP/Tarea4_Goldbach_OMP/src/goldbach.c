// Goldbach_optimizado program v1.3 Fabio Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Sanabria Valerin>
#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>
#include "array_int64.h"
#include "array_num64.h"
#include "goldbach.h"
#include "calculadora.h"
#include "salida.h"
#include "entrada.h"
/**
 * @brief Subrutina llama a una de las subrutinas de calculadora.h para
 * calcular las sumas
 * @param array_primos arreglo de primos para cada elemento,
 * alli es donde se va a guardar los respectivos numeros
 * primos
 * @param array_booleans arreglo de booleanos que ayuda a
 * que los numeros primos se logren guardar en las Criba
 * @param num Numero ingresado por el usuario
 * @return int cantidad de primos que tiene el array
*/
int calcular_primos(array_primos_t* array_primos,
array_booleans_t* array_booleans, int64_t num);

/**
 * @brief Subrutina que llama a otra subrutina que se encuentra en entrada.h
 * para lograr recibir los valores metidos por el usuario
 * @param array puntero a objeto de tipo array_int64_t,
 * debe ser distinto a NULL
 * @param file un puntero a un archivo de texto
 * @param argc Cantidad de argumento que ha metido el usuario
 * @param argv Los argumentos que metio el usuario para usar en el
 * programa
 * @return un codigo de error
 * EXIT_SUCCESS si se analizaron correctamente los datos
 * EXIT_FAILURE si no se analizan los datos correctamente
*/
int goldbach_recibir_datos(array_int64_t *array, FILE *file, int argc,
char* argv[]);

/**
 * @brief Subrutina que llama a otra subrutina que se encuentra en calculadora.h
 * que calcula las sumas de goldbach
 * @param elements puntero a objeto de tipo goldbach, debe ser distinto a NULL
 * @return void
*/
void calcular_sumas(goldbach_t* elements);

/**
 * @brief Subrutina encargada de crear los hilos que el
 * usuario desea o los puestos por defecto en el main
 * @param array Arreglo con los numeros dados por el usuario
 * @param thread_count Cantidad de hilos que desea usar
 * el usuario
 * @param task_amount Cantidad de numeros que el usuario ha
 * agregado en la entrada estandar
 * @return un codigo de error
 * EXIT_SUCCESS si se analizaron correctamente los datos
 * EXIT_FAILURE si no se analizan los datos correctamente
*/
int create_threads(const array_int64_t* array, size_t thread_count,
 size_t task_amount);

/**
 * @brief Subrutina encargada de asignarle a cada thread
 * una tarea en especifico, en este caso se va a utilizar
 * el mapeo dinamico para aumentar el desempeño del
 * algoritmo
 * @param data Datos privados que tiene cada hilo
 * (Dentro del metodo se hace parse)
 * @return un codigo de error
 * EXIT_SUCCESS si se analizaron correctamente los datos
 * EXIT_FAILURE si no se analizan los datos correctamente
*/
void* asignar_thread(void *data);

/** @brief Subrutina que llama a otra subrutina que vive en salida.h
 *  que imprime las sumas de goldbach de todos los numeros ingresados
 * ya sea que soliciten las sumas o solo la cantidad de sumas
 * @param array puntero a objeto de tipo goldbach, debe ser distinto a NULL
 * @param array_size Tamano del arreglo goldbach
 * @return void
*/
void goldbach_print(const array_int64_t* array, const uint64_t array_size);

int calcular_primos(array_primos_t* array_primos,
array_booleans_t* array_booleans, int64_t num) {
  return calcular_primos_calculadora(array_primos, array_booleans,
  num);
}

void calcular_sumas(goldbach_t* elements) {
  calcular_sumas_calculadora(elements);
}

void goldbach_run(array_int64_t* goldbach, size_t thread_count, int argc,
 char* argv[]) {
  size_t task_amount = goldbach_recibir_datos(goldbach, stdin, argc, argv);
  double time_spent = 0;
  clock_t begin = clock();  // para medir tiempo
  create_threads(goldbach, thread_count, task_amount);

  goldbach_print(goldbach, task_amount);
  clock_t end = clock();  // fin de la medicion
  time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Tiempo: %f segundos\n", time_spent);
}

int create_threads(const array_int64_t* array, size_t thread_count,
 size_t task_amount) {
  int error = EXIT_SUCCESS;
  #pragma omp parallel for num_threads(thread_count)\
  schedule(dynamic)shared(array, task_amount)
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
