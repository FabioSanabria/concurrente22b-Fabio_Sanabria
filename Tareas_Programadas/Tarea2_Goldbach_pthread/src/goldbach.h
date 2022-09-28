// Goldbach_pthread program v1.3 Fabio Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Sanabria Valerin>
#ifndef GOLDBACH_H
#define GOLDBACH_H

#include "array_int64.h"

/**
 * @struct private_data_t
 * @brief crea memoria privada
 * estructura para crear la memoria
 * privada de los hilos
*/
typedef struct private private_data_t;

/**
 * @brief @struct shared_data_t
 * @brief crea memoria compartida
 * estructructura para crear la memoria
 * compartida de los hilos
*/
typedef struct shared shared_data_t;

/**
 * @brief Corre las funciones para realizar las sumas
 * de goldbach e imprimirlas segun se solicite, es decir,
 * llama a todos los procesos necesarios para realizar todas las
 * sumas de manera eficaz y eficiente
 * @param goldbach puntero a objeto de tipo goldbach, debe ser distinto a NULL
 * @param thread_count Cantidad de hilos que maneja el programa, el usuario
 * los elige o es la cantidad de procesadores que cuenta la maquina
 * @param argc cantidad de argumentos ingresados enentrada estandar
 * @param argv un arreglo con los datos ingresados de entrada estandar
 * @return un codigo de error
 * EXIT_SUCCESS si se analizaron correctamente los datos
 * EXIT_FAILURE si no se analizan los datos correctamente
*/
void goldbach_run(array_int64_t* goldbach, size_t thread_count,
 int argc, char* argv[]);

#endif  // GOLDBACH_H
