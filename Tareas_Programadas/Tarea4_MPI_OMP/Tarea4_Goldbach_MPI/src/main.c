// Goldbach_optimizado program v1.3 Fabio Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr>
// Copyright [2022] <Fabio Sanabria Valerin>
#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>
#include <mpi.h>
#include "goldbach.h"

int process_job(int argc, char* argv[], int process_number,
int process_count, int thread_count);

// procedure main
int main(int argc, char* argv[]) {
  // se obtienen cpus
  uint64_t thread_count = omp_get_max_threads();

  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
        int process_number = -1;
        MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

        int process_count = -1;
        MPI_Comm_size(MPI_COMM_WORLD, &process_count);

        char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
        int hostname_length = -1;
        MPI_Get_processor_name(process_hostname, &hostname_length);
        // const double start_time = MPI_Wtime();
        // const double elapsed = MPI_Wtime() - start_time;
        process_job(argc, argv, process_number, process_count, thread_count);
        MPI_Finalize();
    } else {
      fprintf(stderr, "error: could not init MPI\n");
    }

  return 0;
}  // end procedure

int process_job(int argc, char* argv[], int process_number,
int process_count, int thread_count) {
  int error = EXIT_SUCCESS;
  if (process_number == 0) {
    array_int64_t goldbach;
    if (argc == 2) {
      if (sscanf(argv[1], "%u", &thread_count) ==1) {
      } else {
        fprintf(stderr, "error : invalid thread count\n");
      }
    }
    array_int64_init(&goldbach);
    goldbach_run(&goldbach, thread_count, argc, argv, process_number,
    process_count);
    // goldbach, argc, argv se pasan los argumentos
    array_int64_destroy(&goldbach);  // se llama al destructor
  }
  return error;
}
