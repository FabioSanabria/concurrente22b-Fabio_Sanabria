// Goldbach Serial assignment version 1.0 
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

typedef struct goldbach {
  array_int64_t values;  // input values
  array_int64_t cant_sum;  // total of goldbach sums
  array_int64_t sums;  // numbers of the goldabch sums if the number is negative
  array_int64_t primes_num;  // prime numbers before an especific number used to the sums
} goldbach_t;


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
int goldbach_analyze_arguments(goldbach_t* goldbach, int argc
  , char* argv[]);

/**
 * @brief Calculates the prime numbers that are before the entered number
 *  and adds them in an array
 * @param goldbach Ponter to a goldbach object
 * @return void
*/
void calculate_primes(goldbach_t* goldbach, int num);

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
    array_int64_init(&goldbach->values);  // Inicialize the values array 
    array_int64_init(&goldbach->cant_sum);  // inicialize the amount of sums array
    array_int64_init(&goldbach->sums);  // inicialize the sums array
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

// Calculate_primes Idea:(Puedo usar for, un metodo de comparacion y devuelvo un codigo de error)
// Calculate_sums Idea: (Iterar todas las combinaciones dde primos hasta que salgan los numeros que quiero, buscar una manera mas efectiva y facil)
//Print Idea: Veo si es negativo o positivo y dependiendo de la ocasion imprimir todo lo que tienen los array 
//Analyze arguments: Adaptar el codigo de Jeisson para que funcione con este
// Out of ideas...