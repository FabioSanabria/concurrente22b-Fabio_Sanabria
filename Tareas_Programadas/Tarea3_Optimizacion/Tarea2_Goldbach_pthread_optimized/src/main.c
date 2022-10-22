// Goldbach_optimizado program v1.3 Fabio Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Sanabria Valerin>
#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "goldbach.h"

// procedure main
int main(int argc, char* argv[]) {

  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  
  if (argc == 2) {
    if (sscanf(argv[1], "%" SCNu64, &thread_count) ==1) {
    } else {
      fprintf(stderr, "error : invalid thread count\n");
    }
  }
  array_int64_t goldbach;
  array_int64_init(&goldbach);
  goldbach_run(&goldbach, thread_count, argc, argv);
    // goldbach, argc, argv se pasan los argumentos
  array_int64_destroy(&goldbach);  // se llama al destructor

  return 0;
}  // end procedure
