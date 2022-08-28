// Goldbach Serial assignment version 1.0 
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Andrés Sanabria Valerin>
//Credits to Jeisson Hidalgo to provide a lot of this code in his list of videos "Taller de C++ a C" 
#ifndef ARRAY_INT64_H
#define ARRAY_INT64_H

#include <stddef.h> //Defines various variable types and macros
#include <inttypes.h> //Provide a set of integer types

/**
 * @brief Struct of an array used for store different types of variables
*/
typedef struct {
  size_t capacity; // Capacity of the array
  size_t count; // The number of elements in the array
  int64_t* elements; // The elements that can be stored in this array
} array_int64_t;

/**
 * @brief Constructor
 * @param array Pointer to an array to simulate the dinamic arrays in c++
 * @return An error code, EXIT_SUCCESS if the code run correctly or 
 * EXIT_FAILURE if the code fails in this method
*/
int array_int64_init(array_int64_t* array);

/**
 * @brief Destructor
 * @param array Pointer to an array to simulate the dinamic arrays in c++
 * @return An error code, EXIT_SUCCESS if the code run correctly or 
 * EXIT_FAILURE if the code fails in this method
*/
void array_int64_destroy(array_int64_t* array);

/**
 * @brief Append elements in the array 
 * @param array Pointer to an array to simulate the dinamic arrays in c++
 * @param element Element that the user wants to store in the array
 * @return An error code, EXIT_SUCCESS if the code run correctly or 
 * EXIT_FAILURE if the code fails in this method
*/
int array_int64_append(array_int64_t* array, int64_t element);

#endif  // ARRAY_INT64_H
