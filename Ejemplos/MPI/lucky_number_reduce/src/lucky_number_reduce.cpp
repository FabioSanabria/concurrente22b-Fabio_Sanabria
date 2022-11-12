// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
// Already seen
#include <mpi.h>
// Already seen
#include <iostream>
// Already seen
#include <stdexcept>

// Clase para obtener numeros aleatorios
#include "UniformRandom.hpp"

#define fail(msg) throw std::runtime_error(msg)

/**
 * @brief Subrutina que genera los numeros de la suerte
 * de todos los procesos que se decidieron tener y ademas
 * revisa cual de ellos es el menor, el mayor y el promedio
 * @param process_number Numero de proceso que entra a la
 * subrutina y que va a generar su propio numero lucky
 * @param process_count Numero total de procesos que el usuario
 * decidio utilizar
 * @return void
*/
void generate_lucky_statistics(int process_number, int process_count);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
// Inicializa todos los procesos de MPI y obtiene valores
// como los numeros de proceso, la cantidad de procesos
// el hostname de la maquina, etc. Es el mismo
// procedimiento que se sigue en anteriores ejemplos
// por lo que se va a omitir
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    try {
      int process_number = -1;
      MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

      int process_count = -1;
      MPI_Comm_size(MPI_COMM_WORLD, &process_count);

      char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
      int hostname_length = -1;
      MPI_Get_processor_name(process_hostname, &hostname_length);
      // Llama a la subrutina que genera los numeros aleatorios
      // o numeros lucky de los procesos
      generate_lucky_statistics(process_number, process_count);
    } catch (const std::runtime_error& exception) {
      std::cout << exception.what() << std::endl;
      error = EXIT_FAILURE;
    }
    // Finaliza todos los procesos de MPI
    MPI_Finalize();
  } else {
    std::cerr << "error: could not init MPI" << std::endl;
    error = EXIT_FAILURE;
  }
  return error;
}

void generate_lucky_statistics(int process_number, int process_count) {
  // Generate my lucky number
  // Llama a la clase uniformRandom y crea un objeto, es una clase
  // plantilla por lo que es reuilizable
  UniformRandom<int> uniformRandom(process_number);
  // Numero de la suerte de los procesadores que se crearon,
  // se llama al objeto de tipo UniformRandom para obtener numeros
  // aleatorios
  const int my_lucky_number = uniformRandom.between(0, 100);
  // Aqui imprime los numeros de la suerte de todos los procesos que
  // pasan por aqui, todos los numeros de la suerte deben de ser
  // aleatorios y diferentes
  std::cout << "Process " << process_number << ": my lucky number is "
    << my_lucky_number << std::endl;

  // Inicializa las variables que se van a utilizar para determinar
  // el minimo, maximo y la suma para luego sacar el promedio
  int all_min = -1;
  int all_max = -1;
  int all_sum = -1;

  // Update distributed statistics from processes' lucky numbers
  // Aqui se utiliza MPI_Reduce para reducir los valores de todos los procesos
  // a un unico valor que lo guarda un solo proceso y que lo puede desplegar
  // en cualquier momento, en este caso se reduce all_min a un solo valor en
  // lugar de que todos los procesos lo tengan
  if (MPI_Reduce(/*input*/ &my_lucky_number, /*output*/ &all_min, /*count*/ 1
    , MPI_INT, MPI_MIN, /*root*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce min");
  }
  // Aqui se utiliza reduce para que all_max sea reducido a un solo valor
  // y lo guarda en el proceso 0
  if (MPI_Reduce(/*input*/ &my_lucky_number, /*output*/ &all_max, /*count*/ 1
    , MPI_INT, MPI_MAX, /*root*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce max");
  }
  // Aqui se utiliza reduce para que all_sum sea reducido a un solo valor
  // y lo guarda en el proceso 0
  if (MPI_Reduce(/*input*/ &my_lucky_number, /*output*/ &all_sum, /*count*/ 1
    , MPI_INT, MPI_SUM, /*root*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce sum");
  }
  // Si el numero de proceso es 0 entonces imprime todos los datos
  // recibidos por el reduce
  if (process_number == 0) {
    const double all_average = double(all_sum) / process_count;
    // Aqui imprime el valor mas pequeno de todos los procesos
    std::cout << "Process " << process_number << ": all minimum = "
      << all_min << std::endl;
    // Aqui imprime el promedio de todos los procesos
    std::cout << "Process " << process_number << ": all average = "
      << all_average << std::endl;
    // Aqui imprime el valor mas grande de todos los procesos
    std::cout << "Process " << process_number << ": all maximum = "
      << all_max << std::endl;
  }
}
