// Goldbach Serial assignment version 1.0
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Andrés Sanabria Valerin>
// Credits to Jeisson Hidalgo
#ifndef GOLDBACH_H
#define GOLDBACH_H

#include "array_int64.h"

// Struct de datos privados para los hilos
// Esto es por asi decirlo la firma del struct
typedef struct private private_data_t;


// Struct de datos compartidos para los hilos
// Esto es por asi decirlo la firma del struct
typedef struct shared shared_data_t;

/**
 * @brief Run all the necesary process to generate the goldbach sums 
 * @param goldbach Pointer to a goldbach object
 * @param argc Arguments entered on standard input
 * @param argv Array entered on standard input
 * @return An error code, EXIT_SUCCESS if the code run correctly or 
 * EXIT_FAILURE if the code fails in this method
*/
void goldbach_run(array_int64_t* goldbach, size_t thread_count,
 int argc, char* argv[]);

#endif  // GOLDBACH_H
