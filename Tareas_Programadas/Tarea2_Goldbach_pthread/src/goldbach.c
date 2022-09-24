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

//------------------------------------------------------------
// Ya no lo vamos a utilizar por no sirve para este diseno
// typedef struct goldbach {
//   array_int64_t values;  // input values
//   array_int64_t cant_sum;  // total of goldbach sums
//   array_int64_t sums;  // numbers of the goldabch sums if the number is negative
//   array_int64_t primes_nums;  // prime numbers before
//   // an especific number used to the sums
// } goldbach_t;


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


int create_threads(shared_data_t *shared_data, size_t task_amount);

/**
 * @brief Calculates all the goldbach sums, save the amount of sums in a variable 
 * and saves the prime numbers needed into the appropriate array
 * Dentro de esta subrutina se calculan los numeros primos
 * para cada numero ingresado
 * @param goldbach Pointer to a goldbach object
 * @return void
*/
void calcular_sumas(goldbach_t* goldbach);

/**
 * @brief Print the goldbach sums of a number, if the
 * number is negative prints the amount of sums and the numbers used, if 
 * the number is positive only prints the amount of sums
 * @param goldbach Pointer to a goldbach object
 * @return void
*/
void goldbach_print(const goldbach_t* goldbach);

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

void calcular_sumas(goldbach_t* goldbach) {
  // Implementar
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

  // Aqui va la parte pa imprimir, pensar en como se hace
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
void goldbach_print(const goldbach_t* goldbach) {
  assert(goldbach);
  int pointer = 0;  // Amount of spaces to move in sums
  for (size_t index = 0; index < goldbach->values.count; ++index) {
    const int64_t num = goldbach->values.elements[index];  // numero original
    int64_t vabs = labs(goldbach->values.elements[index]);  // valor absoluto
    if (vabs < 6) {  // Convert to positive
      printf("%"PRId64 ": NA\n", num);
    } else if (num > 0) {  // if is positive
      if (num % 2 == 0) {  // pair
        int sums_index = 0;  // Index that marks the limit
        // of the sums of a determinated element
        sums_index = 2 * goldbach->cant_sum.elements[index];
        // print the cant_sums pair
        printf("%"PRId64 ": %"PRId64 " sums\n", num,  // cant
         goldbach->cant_sum.elements[index]);
        pointer = pointer + sums_index;
      } else {  // odd
        int sums_index = 0;  // week conjeture needs to
        // multiply by 3 because of the formula
        sums_index = 3 * goldbach->cant_sum.elements[index];
        // print the cant_sums pair
        printf("%"PRId64 ": %"PRId64 " sums\n", num,  // cant
         goldbach->cant_sum.elements[index]);
        pointer = pointer + sums_index;
      }
    } else {  // negative
      if (num % 2 == 0) {  // pair
      // print the cant_sums pair
        printf("%"PRId64 ": %"PRId64 " sums: ", num,
          goldbach->cant_sum.elements[index]);
        for (int i = 0; i < goldbach->cant_sum.elements[index]; i++) {
          printf("%" PRId64 " + %" PRId64,
          // Moving in all the array of sums
            goldbach->sums.elements[pointer],
            goldbach->sums.elements[pointer + 1]);
            pointer = pointer + 2;
          if (i < (goldbach->cant_sum.elements[index])-1) {
            // This is to make the code more readable
            printf(", ");
          } else {
            printf("\n");
          }
        }
      } else {  // odd
      // print the cant_sums odd
        printf("%"PRId64 ": %"PRId64 " sums: ", num,  // cant 
        goldbach->cant_sum.elements[index]);
        for (int i = 0; i < goldbach->cant_sum.elements[index]; i++) {
          printf("%" PRId64 " + %" PRId64 " + %" PRId64 ,
            goldbach->sums.elements[pointer],
            goldbach->sums.elements[pointer + 1],
            goldbach->sums.elements[pointer + 2]);
          pointer = pointer + 3;
          if (i < (goldbach->cant_sum.elements[index])-1) {
            // This is to make the code more readable
            printf(", ");
          } else {
            printf("\n");
          }
        }
      }
    }
  }
}
