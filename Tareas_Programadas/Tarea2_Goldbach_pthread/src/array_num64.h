// Goldbach_pthread program v1.3 Fabio Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Sanabria Valerin>
#ifndef ARRAY_NUM64_H
#define ARRAY_NUM64_H

#include <stddef.h>
#include <inttypes.h>

// Numeros que cumplen los requisitos de
// goldbach
typedef struct {
  int64_t num1; //num1
  int64_t num2;
  int64_t num3;
} sumas_value_t;

// Este arreglo va a contener el arreglo de sumas
// por lo que va a ser mas facil de agarrar
typedef struct {// array con todas las sumas de value
  size_t capacity;
  size_t count;
 sumas_value_t* sumas_value;
} array_sum_t;

// Array con numeros primos
typedef struct { // array de numeros primos
  size_t capacity;
  size_t count;
  int64_t* primo;
} array_primos_t;

/**
 * @brief constructor de sumas
 * @param array un puntero a un array,debe ser distinto a NULL
 * @return void
*/
void sumas_value_init(array_sum_t* array);

/**
 * @brief destructor de sumas
 * @param array un puntero a un array,debe ser distinto a NULL
 * @return void
*/
void sumas_value_destroy(array_sum_t* array);

/**
 * @brief Coloca un numero dentro del array sumas correspondiente
 * @param array un puntero a un array,debe ser distinto a NULL
 * @param num1 el primer numero de las sumas
 * @param num2 el segundo numero de las sumas
 * @param num3 el tercer numero de las sumas
 * @return EXIT_SUCCESS
*/
int sumas_value_append(array_sum_t* array, int64_t num1, int64_t num2, int64_t num3);

/**
 * @brief constructor de primos
 * @param array un puntero a un array,debe ser distinto a NULL
 * @return void
*/
void array_primos_init(array_primos_t* array);

/**
 * @brief Destructor
 * @param array un puntero a un array,debe ser distinto a NULL
 * @return void
*/
void array_primos_destroy(array_primos_t* array);

/**
 * @brief Coloca un numero dentro del array sumas correspondiente
 * @param array un puntero a un array,debe ser distinto a NULL
 * @param num el numero que se desea ingresar en el array
 * @return EXIT_SUCCESS
*/
int array_primos_append(array_primos_t* array, int64_t num);


#endif  // ARRAY_NUM64_H