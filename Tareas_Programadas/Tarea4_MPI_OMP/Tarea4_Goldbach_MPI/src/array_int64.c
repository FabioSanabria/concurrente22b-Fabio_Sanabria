// Goldbach_optimizado program v1.3 Fabio Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Sanabria Valerin>
#include <assert.h>
#include <stdlib.h>
#include <inttypes.h>

#include "array_int64.h"
#include "array_num64.h"

/**
 * @brief Incrementa la capacidad del arreglo dinamicos
 * @param array un puntero a un array,debe ser distinto a NULL
 * @return un codigo de error
 * EXIT_SUCCESS si se analizaron correctamente los datos
 * EXIT_FAILURE si no se analizan los datos correctamente
*/
int goldbach_increase_capacity(array_int64_t *array);

void array_int64_init(array_int64_t *array) {
  array->capacity = 0;
  array->count = 0;
  array->elements = NULL;
}

void array_int64_destroy(array_int64_t *array) {
  for (size_t i = 0; i < array->count; i++) {
    sumas_value_destroy(&array->elements[i].array_sum);
    array_primos_destroy(&array->elements[i].array_primos);
    array_booleans_destroy(&array->elements[i].array_booleans);
  }
  // se libera memoria
  array->capacity = 0;
  array->count = 0;
  free(array->elements);
  array->elements = NULL;
}

int array_int64_append(array_int64_t *array, int64_t num) {
  int error = EXIT_SUCCESS;
  if (array->count == array->capacity) {
    error = goldbach_increase_capacity(array);
  }
  if (error == EXIT_SUCCESS) {
    array->elements[array->count].value = num;
    array->elements[array->count].cant_sum = 0;
    sumas_value_init(&array->elements[array->count].array_sum);
    array_primos_init(&array->elements[array->count].
    array_primos);
    array_booleans_init(&array->elements[array->count].
    array_booleans);
    array->count++;
  }
  return error;
}

/**
 * @brief Incrementa la capacidad del arreglo dinamicos
 * @param array un puntero a un array,debe ser distinto a NULL
 * @return un codigo de error
 * EXIT_SUCCESS si se analizaron correctamente los datos
 * EXIT_FAILURE si no se analizan los datos correctamente
*/
int goldbach_increase_capacity(array_int64_t *array) {
  size_t new_capacity = 10 * (array->capacity ? array->capacity : 1);
  goldbach_t *new_array = (goldbach_t *)realloc
  (array->elements, new_capacity * sizeof(goldbach_t));

  if (new_array) {
    array->capacity = new_capacity;
    array->elements = new_array;
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}
