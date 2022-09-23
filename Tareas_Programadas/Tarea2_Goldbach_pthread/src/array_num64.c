// Goldbach_pthread program v1.0 Fabio Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Sanabria Valerin>
#include <assert.h>
#include <stdlib.h>
#include <inttypes.h>

#include "array_int64.h"
#include "array_num64.h"

// Metodo privado que inicializa el arreglo de sumas
// Doxygen
void sumas_value_init(array_sum_t *array) {
  array->capacity = 0;
  array->count = 0;
  array->sumas_value = NULL;
}

// Metodo privado que destruye el arreglo de sumas
// Doxygen
void sumas_value_destroy(array_sum_t *array) {
  array->capacity = 0;
  array->count = 0;
  free(array->sumas_value);
  array->sumas_value = NULL;
}

// Resize del array de sumas
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

// Append de los numeros dentro del array
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