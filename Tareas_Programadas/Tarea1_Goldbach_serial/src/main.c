// Goldbach Serial assignment version 1.0 
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Andrés Sanabria Valerin>
// Credits to Jeisson Hidalgo to provide the statistics main in is list of videos "Taller de C++ a C"
#include <stdio.h>
#include <stdlib.h>

#include "goldbach.h"

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS; // 0

  goldbach_t* goldbach = goldbach_create();
  if (goldbach) { // If goldbach is NULL, jump to an error
    error = goldbach_run(goldbach, argc, argv); // Pass the arguments and run all the process of goldbach.c   
    goldbach_destroy(goldbach);  // At the end of all process, the destructor free all the memory and destroys all the arrays
  } else {
    fprintf(stderr, "error: could not allocate goldbach object\n"); // error to alocate goldbach object
    error = EXIT_FAILURE; // 1 
  }

  return error;
}
