// Goldbach_optimizado program v1.3 Fabio Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Sanabria Valerin>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "array_int64.h"
#include "array_num64.h"

/**
 * @brief Lee los datos ingresados en entrada estandar dados por el usuario,
 * ademas puede agarrar numeros dados por algun archivo de texto
 * @param array Array goldbach, contiene todos los elementos y herramientas nece-
 * sarias para calcular sus sumas
 * @param file un puntero a un archivo de texto, es el stdin
 * @param argc Cantidad de argumentos ingresados
 * @param argv[] Array de argumentos ingresados
 * @return un codigo de error
 * EXIT_SUCCESS si se analizaron correctamente los datos
 * EXIT_FAILURE si no se analizan los datos correctamente
*/
int goldbach_recibir_datos_entrada(array_int64_t *array, FILE *file, int argc,
char* argv[]);
