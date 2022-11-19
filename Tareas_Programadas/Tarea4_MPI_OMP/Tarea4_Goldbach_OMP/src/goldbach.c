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
#include "array_int64.h"
#include "array_num64.h"
#include "goldbach.h"
#include "calculadora.h"
#include "salida.h"
#include "entrada.h"


/** @struct shared_data_t
 *  @brief Este struct tiene la funcionalidad
 *  de guardar los datos que se requieran compartir
 * entre los hilos como lo es la cantidad de hilos
 *  @b array guarda el arreglo que contiene el goldbach,
 * es decir, todos los valores, sumas, primos, etc
 *  @b thread_count cantidad de hilos que puso el usuario
 *  @b pos posicion en la que se encuentra el hilo en el
 *  arreglo
 *  @b can_acces Control de concurrencia que evita que los
 *  hilos modifiquen
 */
typedef struct shared {
  array_int64_t* array;
  uint64_t thread_count;
  int64_t pos;  // posicion del hilo en el arreglo
  pthread_mutex_t can_acces;  // verifica si el hilo puede accesar
} shared_data_t;


/** @struct private_data_t
 *  @brief Este struct tiene la funcionalidad
 *  de guardar los datos privados de los hilos, como
 * lo es el inicio y final del mapeo de cada hilo, su 
 * shared data y el numero de hilo
 *  @b thread_number Numero de hilo que tiene alguno en
 * especifico
 *  @b shared_data registro compartido de todos los hilos
 */
typedef struct private {
  uint64_t thread_number;
  shared_data_t* shared_data;
} private_data_t;

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
 * @param array puntero a objeto de tipo goldbach, debe ser distinto a NULL
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
 * @param shared_data Registro compartido entre hilos
 * @return un codigo de error
 * EXIT_SUCCESS si se analizaron correctamente los datos
 * EXIT_FAILURE si no se analizan los datos correctamente
*/
int create_threads(shared_data_t *shared_data);

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
  shared_data_t *shared_data = (shared_data_t *)
  calloc(1, sizeof(shared_data_t));
  if (shared_data) {
    shared_data->thread_count = thread_count;
    shared_data->array = goldbach;
    shared_data->pos = 0;  // se inicializa la posicion

    pthread_mutex_init(&shared_data->can_acces, NULL);
    create_threads(shared_data);

    pthread_mutex_destroy(&shared_data->can_acces);
    free(shared_data);
  }
  goldbach_print(goldbach, task_amount);
  clock_t end = clock();  // fin de la medicion
  time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Tiempo: %f segundos\n", time_spent);
}

int create_threads(shared_data_t *shared_data) {
  int error = EXIT_SUCCESS;
  pthread_t *threads = (pthread_t *)
    malloc(shared_data->thread_count * sizeof(pthread_t));
  private_data_t *private_data = (private_data_t *)
    calloc(shared_data->thread_count, sizeof(private_data_t));

  if (threads && private_data) {
    for (uint64_t thread_number = 0; thread_number <
    shared_data->thread_count; ++thread_number) {

      private_data[thread_number].shared_data = shared_data;
      error = pthread_create(&threads[thread_number], /*attr*/ NULL,
      asignar_thread, /*arg*/ &private_data[thread_number]);
    }

    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count;
    ++thread_number) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
    }

    free(private_data);
    free(threads);
  }
  return error;
}

int goldbach_recibir_datos(array_int64_t *array, FILE *file, int argc,
char* argv[]) {
  return goldbach_recibir_datos_entrada(array, file, argc, argv);
}

void* asignar_thread(void* data) {
  private_data_t* private_data = (private_data_t*) data;
  shared_data_t* shared_data = private_data->shared_data;

  int64_t pos = 0;

  while (true) {
    pthread_mutex_lock(&shared_data->can_acces);
    pos = shared_data->pos;
    shared_data->pos++;

    if ((size_t)shared_data->pos > shared_data->array->count) {
      pthread_mutex_unlock(&shared_data->can_acces);
      break;
    }
    pthread_mutex_unlock(&shared_data->can_acces);
    calcular_sumas(&shared_data->array->elements[pos]);
  }
  return NULL;
}

void goldbach_print(const array_int64_t* array, const uint64_t array_size) {
  goldbach_print_salida(array, array_size);
}
