// Copyright 2022 Fabio Sanabria <fabio.sanabria@ucr.ac.cr> CC-BY 4.0
#include <iostream>
#include "Mpi.hpp"
#include "MpiError.hpp"
int main(int argc, char* argv[]) {
  try {
    Mpi mpi(argc, argv);
    std::cout << "Hello from main thread of process " << mpi.getProcessNumber()
      << " of " << mpi.getProcessCount() << " on " << mpi.getHostname()
      << std::endl;
  } catch (const std::exception& error) {
    fail("could not receive message");
  }
}
