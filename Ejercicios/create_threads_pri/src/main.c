// Copyright 2022 Fabio Andres Sanabria Valerin
// <fabio.sanabria@ucr.ac.cr> CC-BY-4
// Simulates a producer and a consumer that share a unbounded buffer

#include <stdlib.h>
#include <stdio.h>

#include "simulation.h"

// The same main that we work in all the exercises
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  // We create the simulation
  simulation_t* simulation = simulation_create();
  if (simulation) {
    // We run all the process
    error = simulation_run(simulation, argc, argv);
    // Free all the memory that we use
    simulation_destroy(simulation);
  } else {
    // Error if something fails in the create of the
    // simulation
    fprintf(stderr, "error: could not allocate simulation\n");
    error = EXIT_FAILURE;
  }
  return error;
}
