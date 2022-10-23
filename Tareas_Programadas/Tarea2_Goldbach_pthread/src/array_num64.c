// Goldbach_optimizado program v1.3 Fabio Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Sanabria Valerin>
#include <assert.h>
#include <stdlib.h>
#include <inttypes.h>

#include "array_int64.h"
#include "array_num64.h"


void sumas_value_init(array_sum_t *array) {
  array->capacity = 0;
  array->count = 0;
  array->sumas_value = NULL;
}

void sumas_value_destroy(array_sum_t *array) {
  array->capacity = 0;
  array->count = 0;
  free(array->sumas_value);
  array->sumas_value = NULL;
}

/**
 * @brief Incrementa la capacidad del arreglo dinamicos
 * @param array un puntero a un array,debe ser distinto a NULL
 * @return un codigo de error
 * EXIT_SUCCESS si se analizaron correctamente los datos
 * EXIT_FAILURE si no se analizan los datos correctamente
*/
int sumas_value_increase_capacity(array_sum_t *array) {
  size_t new_capacity = 10 * (array->capacity ? array->capacity : 1);
  sumas_value_t *new_array =
  (sumas_value_t *)realloc(array->sumas_value,
  new_capacity * sizeof(sumas_value_t));

  if (new_array) {
    array->capacity = new_capacity;
    array->sumas_value = new_array;
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}

int sumas_value_append
(array_sum_t *array, int64_t num1, int64_t num2, int64_t num3) {
  int error = EXIT_SUCCESS;
  if (array->count == array->capacity) {
    error = sumas_value_increase_capacity(array);
  }
  if (error == EXIT_SUCCESS) {
    array->sumas_value[array->count].num1 = num1;
    array->sumas_value[array->count].num2 = num2;
    array->sumas_value[array->count].num3 = num3;
    array->count++;
  }
  return error;
}

void array_primos_init(array_primos_t* array) {
  array->capacity = 0;
  array->count = 0;
  array->primo = NULL;
}

void array_primos_destroy(array_primos_t* array) {
  array->capacity = 0;
  array->count = 0;
  free(array->primo);
  array->primo = NULL;
}

/**
 * @brief Incrementa la capacidad del arreglo dinamicos
 * @param array un puntero a un array,debe ser distinto a NULL
 * @return un codigo de error
 * EXIT_SUCCESS si se analizaron correctamente los datos
 * EXIT_FAILURE si no se analizan los datos correctamente
*/
int array_primos_increase_capacity(array_primos_t* array) {
  size_t new_capacity = 10 * (array->capacity ? array->capacity : 1);
  int64_t* new_array = (int64_t*)
    realloc(array->primo, new_capacity * sizeof(int64_t));
  if (new_array) {
    array->capacity = new_capacity;
    array->primo = new_array;
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}

int array_primos_append(array_primos_t*array, int64_t num) {
  int error = EXIT_SUCCESS;
  if (array->count == array->capacity) {
    error = array_primos_increase_capacity(array);
  }
  if (error == EXIT_SUCCESS) {
    array->primo[array->count] = num;
    array->count++;
  }
  return error;
}

void array_booleans_init(array_booleans_t* array) {
  array->capacity = 0;
  array->count = 0;
  array->boolean = NULL;
}

void array_booleans_destroy(array_booleans_t* array) {
  array->capacity = 0;
  array->count = 0;
  free(array->boolean);
  array->boolean = NULL;
}

/**
 * @brief Incrementa la capacidad del arreglo dinamicos
 * @param array un puntero a un array,debe ser distinto a NULL
 * @return un codigo de error
 * EXIT_SUCCESS si se analizaron correctamente los datos
 * EXIT_FAILURE si no se analizan los datos correctamente
*/
int array_booleans_increase_capacity(array_booleans_t* array) {
  size_t new_capacity = 10 * (array->capacity ? array->capacity : 1);
  int64_t* new_array = (int64_t*)
    realloc(array->boolean, new_capacity * sizeof(int64_t));
  if (new_array) {
    array->capacity = new_capacity;
    array->boolean = new_array;
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}

int array_booleans_append(array_booleans_t*array, int64_t num) {
  int error = EXIT_SUCCESS;
  if (array->count == array->capacity) {
    error = array_booleans_increase_capacity(array);
  }
  if (error == EXIT_SUCCESS) {
    array->boolean[array->count] = num;
    array->count++;
  }
  return error;
}
