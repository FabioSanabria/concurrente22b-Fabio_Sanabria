// Goldbach_pthread program v1.3 Fabio Sanabria Valerin
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
 */
typedef struct shared {
  array_int64_t* array;
  uint64_t thread_count;
} shared_data_t;

/** @struct private_data_t
 *  @brief Este struct tiene la funcionalidad
 *  de guardar los datos privados de los hilos, como
 * lo es el inicio y final del mapeo de cada hilo, su 
 * shared data y el numero de hilo
 *  @b thread_number Numero de hilo que tiene alguno en
 * especifico
 *  @b shared_Data registro compartido de todos los hilos
 * 
 *  @b start Indica el inicio de los elementos con los que
 * tiene que trabajar el hilo (mapeo). Ej [0, 3[
 * 
 * @b finish Indica el final de los elementos con los que 
 * tiene que trabajar un hilo (mapeo). Ej [0, 3[
 */
typedef struct private {
  uint64_t thread_number;
  shared_data_t* shared_data;
  size_t start;
  size_t finish;
} private_data_t;

/**
 * @brief Subrutina que calcula los numeros primos
 * para cada numero ingresado para luego guardarlos
 * en un arreglo
 * @param array_primos arreglo de primos para cada elemento,
 * alli es donde se va a guardar los respectivos numeros
 * primos
 * @param num Numero ingresado por el usuario
 * @param array_booleans Array de booleanos para poder
 * obtener los numeros primos
 * @return int cantidad de primos que tiene el array
*/
int calcular_primos(array_primos_t* array_primos,
array_booleans_t* array_booleans, int64_t num);
/**
 * @brief Lee los datos ingresados en entrada estandar dados por el usuario,
 * ademas puede agarrar numeros dados por algun archivo de texto
 * @param array Array goldbach, contiene todos los elementos y herramientas nece-
 * sarias para calcular sus sumas
 * @param file un puntero a un archivo de texto, es el stdin
 * @param argc Cantidad de argumentos ingresados
 * @param argv[] Array de argumentos ingresados
 * @return un codigo de error
 * EXIT_SUCCESS si se analizaron correctamente los datos
 * EXIT_FAILURE si no se analizan los datos correctamente
*/
int goldbach_recibir_datos(array_int64_t *array, FILE *file, int argc,
char* argv[]);

/**
 * @brief Subrutina que calcula las sumas de goldbach
 * Dentro de esta subrutina se calculan los numeros primos
 * para cada numero ingresado y revisa si es numero par o impar
 * @param elements puntero a objeto de tipo goldbach, debe ser distinto a NULL
 * @return void
*/
void calcular_sumas(goldbach_t* elements);

/**
 * @brief Subrutina que calcula las sumas de goldbach
 * de los numeros pares que ingrese el usuario
 * @param elements puntero a objeto de tipo goldbach que
 * contiene las sumas, primos, etc, debe ser distinto a NULL
 * @param num Numero al que se le quiere calcular las sumas
 * @param prime_count Cantidad de numeros primos, este valor
 * es retornado por el calculo de primos
 * @return void
*/
void calcular_pares
(goldbach_t* elements, int64_t num, int prime_count);

/**
 * @brief Subrutina que calcula las sumas de goldbach
 * de los numeros impares que ingrese el usuario
 * @param elements puntero a objeto de tipo goldbach que
 * contiene las sumas, primos, etc, debe ser distinto a NULL
 * @param num Numero al que se le quiere calcular las sumas
 * @param prime_count Cantidad de numeros primos, este valor
 * es retornado por el calculo de primos
 * @return void
*/
void calcular_impares(goldbach_t* elements, int64_t
num, int prime_count);

/**
 * @brief Subrutina encargada de crear los hilos que el
 * usuario desea o los puestos por defecto en el main
 * @param shared_data Registro compartido entre hilos
 * @param task_amount Cantidad de numeros que queremos
 * @return un codigo de error
 * EXIT_SUCCESS si se analizaron correctamente los datos
 * EXIT_FAILURE si no se analizan los datos correctamente
*/
int create_threads(shared_data_t *shared_data, size_t task_amount);

/**
 * @brief Subrutina encargada de asignarle a cada thread
 * una tarea en especifico
 * @param data Datos privados que tiene cada hilo
 * (Dentro del metodo se hace parse)
 * @return un codigo de error
 * EXIT_SUCCESS si se analizaron correctamente los datos
 * EXIT_FAILURE si no se analizan los datos correctamente
*/
void* asignar_thread(void *data);

/**
 * @brief Formula de mapeo de bloque, esta formula ayuda 
 * a que los hilos sepan con que valores van a trabajar
 * gracias el tipo de mapeo de bloque que es el mas
 * sencillo de hacer, un hilo de se encarga de una lista
 * de numero y otro hilo se encarga de otra lista de numeros
 * dividiendo asi las tareas entre todos lo mas equitativa
 * posible
 * @param i numero de hilo
 * @param D Cantidad de numeros que queremos
 * @param w Cantidad total de hilos
 * @return Un intervalo de numeros donde el hilo va a 
 * realizar sus tareas
*/
size_t formula_bloque(int i, int D, int w);

/** @brief Subrutina que imprime las sumas de goldbach
 * ya sea que soliciten las sumas o solo la cantidad de sumas,
 * en cualquiera de los 2 casos va a calcular la cantidad total
 * de sumas y la cantidad de numeros ingresados
 * @param array Array goldbach que contiene todas las sumas y
 * elementos de cada numero ingresado
 * @param array_size tamanio del arreglo goldbach
 * @return void
*/
void goldbach_print(const array_int64_t* array, const uint64_t array_size);

/** @brief Subrutina que imprime las sumas de goldbach
 * en caso de que el numero sea par, si el numero es negativo
 * tendria que imprimir 2 numeros como minimo para simular 
 * la suma goldbach
 * @param array Array goldbach que contiene todas las sumas y
 * elementos de cada numero ingresado
 * @param i Posicion del elemento que se quiere imprimir
 * @return void
*/
void print_par(const array_int64_t* array, int i);

/** @brief Subrutina que imprime las sumas de goldbach
 * en caso de que el numero sea impar, si el numero es negativo
 * tendria que imprimir 3 numeros como minimo para simular 
 * la suma goldbach
 * @param array Array goldbach que contiene todas las sumas y
 * elementos de cada numero ingresado
 * @param i Posicion del elemento que se quiere imprimir
 * @return void
*/
void print_impar(const array_int64_t* array, int i);

/** @brief Subrutina que imprime la cantidad total de las
 * sumas que hay en el arreglo de goldbach y las imprime en 
 * el inicio de todo
 * @param array Array goldbach que contiene todas las sumas y
 * elementos de cada numero ingresado
 * @param array_size tamanio del arreglo goldbach
 * @return void
*/
void print_cant_sumas_numeros(const array_int64_t* array,
const uint64_t array_size);

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
  size_t cant_nums = goldbach_recibir_datos(goldbach, stdin, argc, argv);
  double time_spent = 0;
  clock_t begin = clock();
  shared_data_t *shared_data = (shared_data_t *)
  calloc(1, sizeof(shared_data_t));
  if (shared_data) {
    shared_data->thread_count = thread_count;
    shared_data->array = goldbach;
    // Llama a todo
    create_threads(shared_data, cant_nums);
    free(shared_data);  // LIbera datos
  }
  goldbach_print(goldbach, cant_nums);
  clock_t end = clock();
  time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Tiempo: %f segundos\n", time_spent);
}

uint64_t minimo(uint64_t a, uint64_t b) {
  return ( ( ( a) < ( b)) ? ( a) : ( b));
}

size_t formula_bloque(int i, int D, int w) {
  // i = thread_number, D = cant_sums, w = thread_count
  return i*(int)floor(D/w)+minimo(i, D%w);
}

int create_threads(shared_data_t *shared_data, size_t task_amount) {
  int error = EXIT_SUCCESS;
  pthread_t *threads = (pthread_t *)
    malloc(shared_data->thread_count * sizeof(pthread_t));
  private_data_t *private_data = (private_data_t *)
    calloc(shared_data->thread_count, sizeof(private_data_t));

  if (threads && private_data) {
    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count;
    ++thread_number) {
      private_data[thread_number].thread_number = thread_number;
      private_data[thread_number].shared_data = shared_data;
      private_data[thread_number].start = formula_bloque(thread_number,
      task_amount, shared_data->thread_count);
      private_data[thread_number].finish = formula_bloque(thread_number+1,
      task_amount, shared_data->thread_count);
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

  for ( size_t i = private_data->start; i < private_data->finish; i++ ) {
    calcular_sumas(&shared_data->array->elements[i]);  // Llama a calcular
  }
  return NULL;
}

void goldbach_print(const array_int64_t* array, const uint64_t array_size) {
  goldbach_print_salida(array, array_size);
}
