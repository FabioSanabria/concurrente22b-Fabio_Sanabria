// Goldbach_pthread program v1.3 Fabio Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Sanabria Valerin>
#ifndef ARRAY_INT64_H
#define ARRAY_INT64_H

#include <stddef.h>
#include <inttypes.h>

#include "array_num64.h"


/**
 * @brief Estructura de datos goldbach
 * contiene los datos que pertenecen
 * a cada valor ingresado
 * @b value Numero ingresado por el usuario
 * @b cant_sum Cantidad de sumas que tiene el numero
 * @b array_sum Array de sumas que tiene el numero
 * @b array_primos Array de primos que tiene un numero,
 * todos los primos deben de ser menores que el numero
 * @b thread_count cantidad de hilos que puso el usuario
 * 
*/
typedef struct goldbach_array {
  int64_t value;
  int64_t cant_sum;
  array_sum_t array_sum;
  array_primos_t array_primos;
} goldbach_t;

/**
 * @brief Estructura de datos array
 *  array que contiene goldbach
 * y sus elementos
 * @b capacity capacidad que puede tener el array en
 * determinado tiempo
 * @b count cantidad de elementos dentro del arreglo
 * @b elements array goldbach, contiene todos los calculos
 * hechos en goldbach.c, 
*/
typedef struct {
  size_t capacity;
  size_t count;
  goldbach_t* elements;
} array_int64_t;

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
