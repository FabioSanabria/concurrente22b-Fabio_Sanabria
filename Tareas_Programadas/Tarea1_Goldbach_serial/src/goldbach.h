// Goldbach Serial assignment version 1.0
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Andrés Sanabria Valerin>
// Credits to Jeisson Hidalgo
#ifndef GOLDBACH_H
#define GOLDBACH_H

/**
 * @brief Struct of an array used for store different types of variables
*/
typedef struct goldbach goldbach_t;  // "Registro opaco"

/**
 * @brief Constructor
 * @return pointer to a goldbach_t object
*/
goldbach_t* goldbach_create(void);  // Implemented in the .c "Registro opaco"

/**
 * @brief Run all the necesary process to generate the goldbach sums 
 * @param goldbach Pointer to a goldbach object
 * @param argc Arguments entered on standard input
 * @param argv Array entered on standard input
 * @return An error code, EXIT_SUCCESS if the code run correctly or 
 * EXIT_FAILURE if the code fails in this method
*/
int goldbach_run(goldbach_t* goldbach, int argc, char* argv[]);

/**
 * @brief Destructor
 * @param goldbach Pointer to a goldbach object
 * @return void
*/
void goldbach_destroy(goldbach_t* goldbach);

#endif  // GOLDBACH_H
