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