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
 * @return int cantidad de primos que tiene el array
*/
int calcular_primos(array_primos_t* array_primos, int64_t num);

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
  int64_t num = labs(elements->value);

  int prime_count = calcular_primos
    (&elements->array_primos, num);
  if (num > 5) {
    if (elements->value % 2 == 0) {
      calcular_pares(elements, num, prime_count);
    } else {
      calcular_impares(elements, num, prime_count);
    }
  }
}

void calcular_pares
  (goldbach_t *elements, int64_t num, int prime_count) {
  bool array_sum_required = false;
  if (elements->value < 0) {
    array_sum_required = true;
  }
  for (int i = 0; i < prime_count; i++) {
    for (int j = i; j < prime_count; j++) {
      if (elements->array_primos.primo[i] +
      elements->array_primos.primo[j] == num) {
        elements->cant_sum++;
        if (array_sum_required) {
          sumas_value_append(&elements->array_sum,
          elements->array_primos.primo[i],
          elements->array_primos.primo[j], 0);
        }
      }
    }
  }
}

void calcular_impares(goldbach_t *elements, int64_t
num, int prime_count) {
  bool array_sum_required = false;
  if (elements->value < 0) {
    array_sum_required = true;
  }
  for (int i = 0; i < prime_count; i++) {
    for (int j = i; j < prime_count; j++) {
      for (int k = j; k < prime_count; k++) {
        if (elements->array_primos.primo[i] +
        elements->array_primos.primo[j] +
        elements->array_primos.primo[k] == num) {
          elements->cant_sum++;
          if (array_sum_required) {
            sumas_value_append(&elements->array_sum,
            elements->array_primos.primo[i],
            elements->array_primos.primo[j],
            elements->array_primos.primo[k]);
          }
        }
      }
    }
  }
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
  assert(array);
  assert(file);
  if (argc < 0 && argv[1]) {
    printf("no hay argumentos");
  }
  // printf("Ingrese los numeros que desea procesar\n");
  int64_t value = 0;
  int contador = 0;
  while (fscanf(file, "%" PRId64, &value) == 1) {
    ++contador;
    array_int64_append(array, value);
  }
  return contador;
}

void* asignar_thread(void* data) {
  private_data_t* private_data = (private_data_t*) data;
  shared_data_t* shared_data = private_data->shared_data;

  for ( size_t i = private_data->start; i < private_data->finish; i++ ) {
    calcular_sumas(&shared_data->array->elements[i]);  // Llama a calcular
  }
  return NULL;
}

void print_cant_sumas_numeros(const array_int64_t* array,
const uint64_t array_size) {
  int64_t cant_sumas = 0;
  int64_t cant_numeros = (int64_t)array_size;
  for (uint64_t i = 0; i < array_size; i++) {
    if (array->elements[i].value == 4 || array->elements[i].value == -4) {
      ++cant_sumas;
    } else {
      cant_sumas += array->elements[i].cant_sum;
    }
  }
  printf("Total " "%"PRId64 " numbers " "%"PRId64 " sums\n\n",
  cant_numeros, cant_sumas);
}

void goldbach_print(const array_int64_t* array, const uint64_t array_size) {
  print_cant_sumas_numeros(array, array_size);
  for (uint64_t i = 0; i < array_size; i++) {
    int64_t num = labs(array->elements[i].value);

    printf("%" PRId64 ": ", array->elements[i].value);
    if (0 <= num && num <= 5) {
      if (array->elements[i].value == 4) {
        printf("1 sums");
      } else {
        if (array->elements[i].value == -4) {
          printf("1 sums: 2 + 2");
        } else {
          printf("NA");
        }
      }
    } else {
      printf("%" PRId64 " sums: ", array->elements[i].cant_sum);
      if (array->elements[i].value < 0) {
        if (array->elements[i].value % 2 == 0) {
          print_par(array, i);
        } else {
          print_impar(array, i);
        }
      }
    }
    printf("\n");
  }
}

void print_par(const array_int64_t* array, int i) {
  for (int j = 0; j < array->elements[i].cant_sum; j++) {
    if ( j ==  array->elements[i].cant_sum - 1 ) {
      printf("%" PRId64 " + %" PRId64, array->elements[i].array_sum.
      sumas_value[j].num1, array->elements[i].array_sum.sumas_value
      [j].num2);
    } else {
      printf("%" PRId64 " + %" PRId64 ", ", array->elements[i].
      array_sum.sumas_value[j].num1, array->elements[i].array_sum.
      sumas_value[j].num2);
    }
  }
}

void print_impar(const array_int64_t* array, int i) {
  for (int j = 0; j < array->elements[i].cant_sum; j++) {
    if ( j == array->elements[i].cant_sum - 1 ) {
      printf("%" PRId64 " + %" PRId64 " + %" PRId64, array->elements[i]
      .array_sum.sumas_value[j].num1, array->elements[i].array_sum.
      sumas_value[j].num2, array->elements[i].array_sum.sumas_value
      [j].num3);
    } else {
      printf("%" PRId64 " + %" PRId64 " + %" PRId64 ", ",
      array->elements[i].array_sum.sumas_value[j].num1,
      array->elements[i].array_sum.sumas_value[j].num2,
      array->elements[i].array_sum.sumas_value[j].num3);
    }
  }
}
