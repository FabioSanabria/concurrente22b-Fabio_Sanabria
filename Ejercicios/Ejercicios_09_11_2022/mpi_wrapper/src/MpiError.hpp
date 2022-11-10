// Copyright 2022 Fabio Sanabria <fabio.sanabria@ucr.ac.cr> CC-BY 4.0
// Already seen
#ifndef MPIERROR_HPP
#define MPIERROR_HPP
#include <mpi.h>
// Already seen
#include <cstdlib>
// Already seen
#include <sstream>
// Libreria para manejo de excepciones
#include <stdexcept>
// Already seen
#include <vector>
#include <iostream>
#include <string>
#include "Mpi.hpp"
// Manejo de excepciones

class MpiError: public std::runtime_error {
 public:
  // MpiError();
  explicit MpiError(const std::string& message) : std::runtime_error(message) {
    std::cout << message << std::endl;
  }

  MpiError(const std::string& message, Mpi& mpi):std::runtime_error(message) {
    std::cout <<mpi.getHostname() << ": "<< mpi.getProcessNumber()
    << ": " << message << std::endl;
  }

  MpiError(const std::string& message, Mpi& mpi, const int threadNumber):
  runtime_error(message) {
    std::cout <<mpi.getHostname() << ": "<< mpi.getProcessNumber()
    << ": " << threadNumber << ": "<< message << std::endl;
  }
};
#endif
