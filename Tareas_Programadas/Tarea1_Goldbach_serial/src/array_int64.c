// Goldbach Serial assignment version 1.0
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Andrés Sanabria Valerin>
// Credits to Jeisson Hidalgo to provide a lot of
// this code in his list of videos "Taller de C++ a C"
#include <assert.h>
#include <stdlib.h>

#include "array_int64.h"  // Include .h of this .c program

/**
 * @brief Increase the array's capacity
 * @param array Pointer to an array to simulate the dinamic arrays in c++
 * @return An error code, EXIT_SUCCESS if the code run correctly or 
 * EXIT_FAILURE if the code fails in this method
*/
int array_int64_increase_capacity(array_int64_t* array);

int array_int64_init(array_int64_t* array) {
  assert(array);
  array->capacity = 0;
  array->count = 0;
  array->elements = NULL;
  return EXIT_SUCCESS;
}

void array_int64_destroy(array_int64_t* array) {
  // Free memory to avoid memory leaks
  assert(array);
  array->capacity = 0;
  array->count = 0;
  free(array->elements);
}

int array_int64_append(array_int64_t* array, int64_t element) {
  assert(array);
  if (array->count == array->capacity) {
    if (array_int64_increase_capacity(array) != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }
  }
  array->elements[array->count++] = element;  // New value in the array
  return EXIT_SUCCESS;
}

int array_int64_increase_capacity(array_int64_t* array) {
  size_t new_capacity = 10 * (array->capacity ? array->capacity : 1);
  int64_t* new_elements = (int64_t*)
    realloc(array->elements, new_capacity * sizeof(int64_t));
    // We do a realloc to create a new node
// At this point we change the capacity and the elements for the new ones
  if (new_elements) {
    array->capacity = new_capacity;
    array->elements = new_elements;
    return EXIT_SUCCESS;
  } else {
    return EXIT_FAILURE;
  }
}
