// Goldbach_optimizado program v1.3 Fabio Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Sanabria Valerin>

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "array_int64.h"

#ifndef SALIDA_H
#define SALIDA_H

/** @brief Subrutina que imprime la cantidad total de las
 * sumas que hay en el arreglo de goldbach y las imprime en 
 * el inicio de todo
 * @param array Array goldbach que contiene todas las sumas y
 * elementos de cada numero ingresado
 * @param array_size tamanio del arreglo goldbach
 * @return void
*/
void print_cant_sumas_numeros(const array_int64_t* array,
const uint64_t array_size);

/** @brief Subrutina que imprime las sumas de goldbach
 * ya sea que soliciten las sumas o solo la cantidad de sumas,
 * en cualquiera de los 2 casos va a calcular la cantidad total
 * de sumas y la cantidad de numeros ingresados
 * @param array Array goldbach que contiene todas las sumas y
 * elementos de cada numero ingresado
 * @param array_size tamanio del arreglo goldbach
 * @return void
*/
void goldbach_print_salida(const array_int64_t* array, const uint64_t array_size);

/** @brief Subrutina que imprime las sumas de goldbach
 * en caso de que el numero sea par, si el numero es negativo
 * tendria que imprimir 2 numeros como minimo para simular 
 * la suma goldbach
 * @param array Array goldbach que contiene todas las sumas y
 * elementos de cada numero ingresado
 * @param i Posicion del elemento que se quiere imprimir
 * @return void
*/
void print_par(const array_int64_t* array, int i);

/** @brief Subrutina que imprime las sumas de goldbach
 * en caso de que el numero sea impar, si el numero es negativo
 * tendria que imprimir 3 numeros como minimo para simular 
 * la suma goldbach
 * @param array Array goldbach que contiene todas las sumas y
 * elementos de cada numero ingresado
 * @param i Posicion del elemento que se quiere imprimir
 * @return void
*/
void print_impar(const array_int64_t* array, int i);

#endif