// Copyright 2022 Fabio Sanabria <fabio.sanabria@ucr.ac.cr> CC-BY 4.0
// Already seen
#ifndef MPI_HPP
#define MPI_HPP
#include <mpi.h>
// Already seen
#include <cstdlib>
// Already seen
#include <iostream>
// Already seen
#include <sstream>
// Libreria para manejo de excepciones
#include <stdexcept>
// Already seen
#include <vector>

// Manejo de excepciones
#define fail(msg) throw std::runtime_error(msg)

class Mpi {
 public:
    explicit Mpi(int argc, char* argv[]) {
        if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
            // rank
            // El numero de proceso/ hilo que esta ejecutando
            // el programa, si solo operamos en una sola zona
            // de trabajo, entonces solo tendremos y procces_count
            // y por lo tanto, 1 process_number
            MPI_Comm_rank(MPI_COMM_WORLD, &this->process_number);
            // Guarda en la variable process_count, la cantidad
            // de zonas/ maquinas que estan ejecutando este pro-
            // grama
            MPI_Comm_size(MPI_COMM_WORLD, &this->process_count);
            // Obtiene el nombre del procesador que ejecuta el programa
            // en este caso se llama Fabiosv-VirtualBox
            MPI_Get_processor_name(this->process_hostname,
            &this->hostname_length);

            MPI_Finalize();
        }
    }
      ~Mpi() = default;  // Destroyer

   /**
    * @brief Subrutina que lo que hace es devolver
    * el numero de proceso que esta ejecutando los
    * procesos en estos momentos, este es el metodo
    * rank() del enunciado
    * @return process_number
    */
      inline int getProcessNumber() {
        return this->process_number;
      }
   /**
    * @brief Subrutina que lo que hace es devolver
    * la cantidad de procesos que van a saludar en el
    * main. este es el metodo size() del enunciado
    * @return process_count
    */
      inline int getProcessCount() {
       return this->process_count;
      }
   /**
    * @brief Subrutina que lo que hace es devolver
    * nombre del procesador que esta ejecutando todos
    * los procesos que se piden, en este caso es fasav
    * @return process_number
    */
      inline char* getHostname() {
       return this->process_hostname;
      }

 private:
     // El numero de proceso
     int process_number = -1;
     // Cantidad de procesos totales que ejecuta la maquina
     int process_count = -1;
     // El tamanio que va a tener el nombre del procesador
     int hostname_length = -1;
     // EL nombre del procesador que esta ejecutando las instrucciones
     char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
};
#endif
