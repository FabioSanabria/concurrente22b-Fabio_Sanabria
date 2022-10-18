// Goldbach_pthread program v1.3 Fabio Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Sanabria Valerin>
#ifndef ARRAY_NUM64_H
#define ARRAY_NUM64_H

#include <stddef.h>
#include <inttypes.h>

/**
 * @brief Estructura de datos de sumas
 *  estructura que contiene los 3 valores
 * necesarios para las sumas, en caso de 
 * conjetura debil se usan solo 2
*/
typedef struct {
  int64_t num1;  // num1
  int64_t num2;
  int64_t num3;
}sumas_value_t;

/**
 * @brief Estructura de datos con las sumas de value
 *  array que contiene las sumas procesadas del
 * valor ingresado y su cantidad de sumas
 *  @b capacity contiene la cantidad
 * de elementos que le caben al array
 *  @b count contiene la cantidad de sumas
 * en especifico que tiene el array
 * @b sumas_value contiene el array de
 * sumas de un elemento en especifico
*/
typedef struct {  // array con todas las sumas de value
  size_t capacity;  // Capacidad del array
  size_t count;  // Cantidad de sumas
  sumas_value_t* sumas_value;  // Array de sumas
} array_sum_t;

/**
 * @brief Estructura de datos de numeros primos
 *  array que contiene la informacion de 
 * los numeros primos
*/
typedef struct {  // array de numeros primos
  size_t capacity;
  size_t count;
  int64_t* primo;
} array_primos_t;

/**
 * @brief Constructor de sumas
 * @param array un puntero a un array de sumas,
 * debe ser distinto a NULL
 * @return void
*/
void sumas_value_init(array_sum_t* array);

/**
 * @brief Destructor de sumas
 * @param array un puntero a un array de sumas,
 * debe ser distinto a NULL
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
int sumas_value_append(array_sum_t* array, int64_t num1,
 int64_t num2, int64_t num3);

/**
 * @brief Constructor de primos
 * @param array un puntero a un array,debe ser distinto a NULL
 * @return void
*/
void array_primos_init(array_primos_t* array);

/**
 * @brief Destructor de primos
 * @param array un puntero a un array de primos,
 * debe ser distinto a NULL
 * @return void
*/
void array_primos_destroy(array_primos_t* array);

/**
 * @brief Coloca un numero dentro del array de primos correspondiente
 * @param array un puntero a un array de primos,
 * debe ser distinto a NULL
 * @param num el numero que se desea ingresar en el array
 * @return EXIT_SUCCESS
*/
int array_primos_append(array_primos_t* array, int64_t num);


#endif  // ARRAY_NUM64_H
