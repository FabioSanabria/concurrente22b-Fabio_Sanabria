// Goldbach Serial assignment version 1.5
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Andrés Sanabria Valerin>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include "array_int64.h"
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
 * @brief Get the numbers of the standard input to realize the goldbach sums 
 * and take the test cases of the file (< test/test-small or < test/test-medium)
 * (Pruebas de Caja Negra vistas en el video de Jeisson)
 * @param goldbach Pointer to a goldbach object
 * @param file Pointer to the test cases (Files)
 * @return An error code, EXIT_SUCCESS if the code run correctly or 
 * EXIT_FAILURE if the code fails in this method
*/
int get_numbers(goldbach_t* goldbach, FILE* file);

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
 * @brief Calculates the prime numbers that are before the entered number
 *  and adds them in an array
 * @param goldbach Ponter to a goldbach object
 * @param num Number that the user wants to see his goldbach sum
 * @return void
*/
int calcular_primos(array_primos_t* array_primos, int64_t num);

/**
 * @brief Calculates all the goldbach sums, save the amount of sums in a variable 
 * and saves the prime numbers needed into the appropriate array
 * Dentro de esta subrutina se calculan los numeros primos
 * para cada numero ingresado
 * @param goldbach Pointer to a goldbach object
 * @return void
*/
void calculate_sums(goldbach_t* goldbach);

/**
 * @brief Print the goldbach sums of a number, if the
 * number is negative prints the amount of sums and the numbers used, if 
 * the number is positive only prints the amount of sums
 * @param goldbach Pointer to a goldbach object
 * @return void
*/
void goldbach_print(const goldbach_t* goldbach);

goldbach_t* goldbach_create(void) {
  goldbach_t* goldbach = (goldbach_t*)
    calloc(1, sizeof(goldbach_t));  // request memory
  if (goldbach) {
    // Inicialize the values array
    array_int64_init(&goldbach->values);
     // inicialize the amount of sums array
    array_int64_init(&goldbach->cant_sum);
    // inicialize the sums array
    array_int64_init(&goldbach->sums);
  }
  return goldbach;
}

void goldbach_destroy(goldbach_t* goldbach) {
  assert(goldbach);
  // Free all the memory used in the program
  array_int64_destroy(&goldbach->values);
  array_int64_destroy(&goldbach->cant_sum);
  array_int64_destroy(&goldbach->sums);
  free(goldbach);
}

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

void calculate_sums(goldbach_t* goldbach) {
  assert(goldbach);  // Defensive programming
  for (size_t i = 0; i < goldbach->values.count; i++) {  // array lenght
    int64_t cant_sum = 0;  // Variable that saves the cant_sums
    // Change the value to positive if it is negative
    int64_t num = labs(goldbach->values.elements[i]);
    calculate_primes(goldbach, num);
    if (num > 5) {  // Condition of goldbach
      // it has to be higher than 5

      // Calling calculate_primes
      // (1,3,5,7) y (6)
      // Search if this can be better
      if (num % 2 == 0) {  // if this is a pair
        for (size_t j = 0; j < goldbach->primes_nums.count
         && (goldbach->primes_nums.elements[j]) <=
          num / 2; j++) {  // as long as j
          // is less than the size of the array
          // and the prime number is less than or
          // equal to the num/2, continue the
          // cycle
          for (size_t k = j; k < goldbach->primes_nums.count
           && (goldbach->primes_nums.elements[k]) <
            num; k++ ) {
            if (goldbach->primes_nums.elements[j] +
             goldbach->primes_nums.elements[k] == num) {
              cant_sum++;
              // if the sum of each elements is the number
              // we save this nums in the array sums
              array_int64_append(&goldbach->sums,
               goldbach->primes_nums.elements[j]);
              array_int64_append(&goldbach->sums,
               goldbach->primes_nums.elements[k]);
            }
          }
        }
        // And then we pass the cant of sums into the array of sums
        array_int64_append(&goldbach->cant_sum, cant_sum);
        cant_sum = 0;  // reset to be used again
      } else {  // case of odd numbers
          // while a is less than the array of primes lenght
          // and see if the element is less than the element / 2
          // this is for optimize something
          for (size_t a = 0; a < goldbach->primes_nums.count
           && (goldbach->primes_nums.elements[a]) <
            (num-1) / 2; a++) {  // sum "1" + 3 + 5
            for (size_t b = a; b < goldbach->primes_nums.count
             && (goldbach->primes_nums.elements[b])
             < num; b++) {  // sum 1 + "3" + 5
              for (size_t c = b; c < goldbach->primes_nums.count &&
               (goldbach->primes_nums.elements[c]) < num; c++) {
                // sum 1 + 3 + "5"
                if (goldbach->primes_nums.elements[a] +
                 goldbach->primes_nums.elements[b] +
                  goldbach->primes_nums.elements[c] == num) {
                  cant_sum++;
                // Sees if the sum of the 3 elements is the number
                // if it is correct append to the sums 3 nums
                  array_int64_append(&goldbach->sums,
                   goldbach->primes_nums.elements[a]);
                  array_int64_append(&goldbach->sums,
                   goldbach->primes_nums.elements[b]);
                  array_int64_append(&goldbach->sums,
                   goldbach->primes_nums.elements[c]);
                }
                // array_int64_destroy(&goldbach->primes_nums);
              }
            }
          }
        // Append the count of sums and then go back to 0
          array_int64_append(&goldbach->cant_sum, cant_sum);
          cant_sum = 0;  // reinicia la cantidad de sums
        }
    } else {  // Case if the number is less than 5
      array_int64_append(&goldbach->cant_sum, 0);
    }
      array_int64_destroy(&goldbach->primes_nums);  // Destroy the primes array
  }
}

void goldbach_run(array_int64_t* goldbach, size_t thread_count, int argc,
 char* argv[]) {
  // Sees if the argumets are correct to use
  int error = goldbach_recibir_datos(goldbach, stdin, argc, argv);
  if (error == EXIT_SUCCESS) {
    get_numbers(goldbach, stdin);
  }
  double time_spent = 0;
  clock_t begin = clock();  // get the time
  if (error == EXIT_SUCCESS) {
    calculate_sums(goldbach);  // Calling all the subrutines
    goldbach_print(goldbach);
  }
  clock_t end = clock();  // To prove how inefficent is the algorithm :(
  time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Tiempo: %f segundos\n", time_spent);
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

int get_numbers(goldbach_t* goldbach, FILE* file) {
  assert(goldbach);
  assert(file);
  int error = EXIT_SUCCESS;

  int64_t value = 0;  // data entered by the user
  while (fscanf(file, "%"SCNd64, &value) == 1) {  // If the data isn't a number,
  // exit the program
    // add all the data to an array of values
    error = array_int64_append(&goldbach->values, value);
    if (error) {  // if there is an error reading
      printf("There was an error in reading");
      break;
    }
  }
  return error;
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
