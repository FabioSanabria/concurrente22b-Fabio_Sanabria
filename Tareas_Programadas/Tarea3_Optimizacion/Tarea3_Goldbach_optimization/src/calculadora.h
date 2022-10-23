// Goldbach_optimizado program v1.3 Fabio Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Sanabria Valerin>

#include <ctype.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "array_int64.h"
#include "array_num64.h"

#ifndef CALCULADORA_H
#define CALCULADORA_H

/**
 * @brief Subrutina que calcula los primos de un numero en
 * espercifico, esto con ayuda de la Criba de Erastostenes
 * @param array_primos arreglo de primos para cada elemento,
 * alli es donde se va a guardar los respectivos numeros
 * primos
 * @param array_booleans arreglo de booleanos que ayuda a
 * que los numeros primos se logren guardar en las Criba
 * @param num Numero ingresado por el usuario
 * @return int cantidad de primos que tiene el array
*/
int calcular_primos_calculadora(array_primos_t* array_primos,
array_booleans_t* array_booleans, int64_t num);

/**
 * @brief Subrutina que calcula las sumas de goldbach pares
 * @param elements puntero a objeto de tipo goldbach, debe ser distinto a NULL
 * @param num numero par a calcular
 * @param prime_count tamanio del arreglo de numeros primos
 * @return void
*/
void calcular_pares_calculadora
(goldbach_t* elements, int64_t num, int prime_count);

/**
 * @brief Subrutina que calcula las sumas de goldbach impares
 * @param elements puntero a objeto de tipo goldbach, debe ser distinto a NULL
 * @param num numero impar a calcular
 * @param prime_count tamanio del arreglo de numeros primos
 * @return void
*/
void calcular_impares_calculadora(goldbach_t* elements, int64_t
num, int prime_count);

/**
 * @brief Subrutina que calcula las sumas de goldbach
 * Dentro de esta subrutina se calculan los numeros primos
 * para cada numero ingresado y revisa si es numero par o impar
 * @param elements puntero a objeto de tipo goldbach, debe ser distinto a NULL
 * @return void
*/
void calcular_sumas_calculadora(goldbach_t* elements);

/**
 * @brief Subrutina que revisa si el numero que es enviado es un numero primo
 * o no
 * @param element numero el cual se quiere saber si es primo o no
 * @return bool true si es primo y false si no es primo
*/
bool esPrimo(int64_t element);
#endif
