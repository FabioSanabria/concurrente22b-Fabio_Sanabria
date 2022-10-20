// Goldbach_optimization program v1.4 Fabio Sanabria
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Sanabria>
#ifndef ARRAY_INT64_H
#define ARRAY_INT64_H

#include <stddef.h>
#include <inttypes.h>

#include "array_num64.h"


/**
 * @brief Estructura de datos goldbach
 *  contiene los datos que pertenecen
 * a cada valor ingresado
*/
typedef struct goldbach_array {
  int64_t value;
  int64_t cant_sum;
  array_sum_t array_sum;
  array_primos_t array_primos;
  array_booleans_t array_booleans;
}goldbach_t;

/**
 * @brief Estructura de datos array
 *  array que contiene goldbach
 * y sus elementos
*/
typedef struct {
  size_t capacity;
  size_t count;
  goldbach_t* elements;
}array_int64_t;

/**
 * @brief Constructor
 * @param array un puntero a un array,debe ser distinto a NULL
 * @return void
*/
void array_int64_init(array_int64_t* array);

/**
 * @brief Destructor
 * @param array un puntero a un array,debe ser distinto a NULL
 * @return void
*/
void array_int64_destroy(array_int64_t* array);

/**
 * @brief Coloca un numero dentro del array
 * @param array un puntero a un array,debe ser distinto a NULL
 * @param element el numero que se desea ingresar en el array
 * @return un codigo de error
 * EXIT_SUCCESS si se analizaron correctamente los datos
 * EXIT_FAILURE si no se analizan los datos correctamente
*/
int array_int64_append(array_int64_t* array, int64_t element);



#endif  // ARRAY_INT64_H
