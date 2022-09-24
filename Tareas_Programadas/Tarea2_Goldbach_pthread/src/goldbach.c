// Goldbach Serial assignment version 1.5
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Andrés Sanabria Valerin>
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

// Structs utilizados para los hilos
// Idea principal es un array de arrays
// Dentro de el array va a tener varios 
// arrays de sumas, primos, etc. Esto por
// recomendacion del profe Alberto
typedef struct shared {
  array_int64_t* array;
  uint64_t thread_count;
} shared_data_t;

typedef struct private {
  uint64_t thread_number;
  shared_data_t* shared_data;
  size_t start;
  size_t finish;
} private_data_t;

/**
 * @brief Calculates the prime numbers that are before the entered number
 *  and adds them in an array
 * @param goldbach Ponter to a goldbach object
 * @param num Number that the user wants to see his goldbach sum
 * @return void
*/
int calcular_primos(array_primos_t* array_primos, int64_t num);

/**
 * @brief Take the arguments of the terminal and analyze if argc and argv are valid
 * @param goldbach Pointer to a goldbach object
 * @param argc NUmber of arguments
 * @param argv Array with the arguments 
 * @return An error code, EXIT_SUCCESS if the code run correctly or 
 * EXIT_FAILURE if the code fails in this method
*/
int goldbach_recibir_datos(array_int64_t *array, FILE *file, int argc,
char* argv[]);

/**
 * @brief Calculates all the goldbach sums, save the amount of sums in a variable 
 * and saves the prime numbers needed into the appropriate array
 * Dentro de esta subrutina se calculan los numeros primos
 * para cada numero ingresado
 * @param goldbach Pointer to a goldbach object
 * @return void
*/
void calcular_sumas(goldbach_t* elements);
void calcular_pares 
(goldbach_t* elements, int64_t num, int prime_count);
void calcular_impares(goldbach_t* elements, int64_t
num, int prime_count);

int create_threads(shared_data_t *shared_data, size_t task_amount);
void* asignar_thread(void *data);
size_t formula_bloque(int i, int D, int w);

void goldbach_print(const array_int64_t* array, const uint64_t array_size);
void print_par(const array_int64_t* array, int i);
void print_impar(const array_int64_t* array, int i);

// Primer subrutina cambiada, se le pasa un array de primos
// y el numero
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
          cont2++;  // La cantidad de primos
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
  // Sees if the argumets are correct to use
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
      // D = 10 sumas, w = 4
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
  char* argv[]) { // analize the arguments

  assert(array);
  assert(file);
  if (argc < 0 && argv[1]) {  // Revisa si hay argumentos
    printf("no hay argumentos");
  }
  // Le pide amablemente al usuario que meta numeros :3
  printf("Ingrese los numeros que desea procesar\n");
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
    // Metodo de calcular sumas para cada elemento, implementar
  }
  return NULL;
}
void goldbach_print(const array_int64_t* array, const uint64_t array_size) {
  for (uint64_t i = 0; i < array_size; i++) {
    int64_t num = labs(array->elements[i].value);

    printf("%" PRId64 " cantidad sumas: ", array->elements[i].value);
    if (0 <= num && num <= 5) {
      printf("NA");
    } else {
      printf("%" PRId64 " ", array->elements[i].cant_sum);
      if (array->elements[i].value < 0) {
        printf(" -> ");
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
