// Goldbach_optimizado program v1.3 Fabio Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Sanabria Valerin>
#ifndef GOLDBACH_H
#define GOLDBACH_H

#include "array_int64.h"

/**
 * @brief crea memoria privada
 * estructructura para crear la memoria
 * privada de los hilos
*/
typedef struct private private_data_t;

/**
 * @brief crea memoria compartida
 * estructructura para crear la memoria
 * compartida de los hilos
*/
typedef struct shared shared_data_t;

/**
 * @brief Corre las funciones para realizar las sumas
 * de goldbach e imprimirlas segun se solicite
 * @param goldbach puntero a objeto de tipo goldbach, debe ser distinto a NULL
 * @param thread_count Cantidad de hilos que desea el usuario
 * @param argc cantidad de argumentos ingresados enentrada estandar
 * @param argv un arreglo con los datos ingresados de entrada estandar
 * @param process_number Numero de proceso que esta ejecutando el programa
 * en un momento determinado
 * @param process_count Cantidad de procesos que van a ejecutar el programa
 * @return un codigo de error
 * EXIT_SUCCESS si se analizaron correctamente los datos
 * EXIT_FAILURE si no se analizan los datos correctamente
*/
void goldbach_run(array_int64_t* goldbach, size_t thread_count,
 int argc, char* argv[], int process_number, int process_count);

#endif  // GOLDBACH_H
