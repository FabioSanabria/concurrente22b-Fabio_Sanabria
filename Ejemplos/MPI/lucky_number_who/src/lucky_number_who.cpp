// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
// Already seen
#include <mpi.h>
// Already seen
#include <iostream>
// Already seen
#include <stdexcept>
// Clase de numeros aleatorios
#include "UniformRandom.hpp"

#define fail(msg) throw std::runtime_error(msg)

/**
 * @brief Subrutina que genera los numeros de la suerte
 * de todos los procesos que se decidieron tener y ademas
 * revisa cual de ellos es el menor, el mayor y el promedio.
 * La diferencia de esta subrutina con la anterior es que
 * en esta los procesos tienen los todos los valores que
 * anteriormente solo tenia el proceso 0
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
  std::cout << "Process " << process_number << ": my lucky number is "
    << my_lucky_number << std::endl;
  // Inicializa las variables que se van a utilizar para determinar
  // el minimo, maximo y la suma para luego sacar el promedio
  int all_min = -1;
  int all_max = -1;
  int all_sum = -1;

  // Update distributed statistics from processes' lucky numbers
  // Aqui hay una diferencia en comparacion con el programa pasado y es que el
  // reduce ahora se llama Allreduce, eso significa que ahora todos los procesos
  // van a tener todos los valores que anteriormente solo lo tenia el proceso 0,
  // esto por debajo es un arbol binario que tiene forma de mariposa
  // simulado por un broadcast. En este caso ya no hay root
  // entonces el all_min va a ser guardado en todos los procesos
  // con el minimo de todos los valores
  if (MPI_Allreduce(/*input*/ &my_lucky_number, /*output*/ &all_min, /*count*/ 1
    , MPI_INT, MPI_MIN, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce min");
  }
  // Aqui el all_max lo van a tener todos los procesos para que todos sepan cual
  // de los numeros lucky es el mas grande
  if (MPI_Allreduce(/*input*/ &my_lucky_number, /*output*/ &all_max, /*count*/ 1
    , MPI_INT, MPI_MAX, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce max");
  }
  // Aqui la suma de todos los valores lo van a tener todos los procesos a
  // diferencia del primer ejemplo, por lo que todos saben cual es la suma
  if (MPI_Allreduce(/*input*/ &my_lucky_number, /*output*/ &all_sum, /*count*/ 1
    , MPI_INT, MPI_SUM, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce sum");
  }
  // Aqui se calcula el promedio
  const double all_average = double(all_sum) / process_count;
  // Aqui comparamos si el numero del proceso "n" es el numero menor
  // de la ejecucion, si lo es entra y dice que es el menor de todos
  if (my_lucky_number == all_min) {
    std::cout << "Process " << process_number << ": my lucky number ("
      << my_lucky_number << ") is the minimum (" << all_min << ")" << std::endl;
  }
  // Aqui comparamos si el numero del proceso "n" es menor
  // que el promedio de todos los numeros lucky,
  // si lo es entra y dice que es menor que el promedio
  if (my_lucky_number < all_average) {
    std::cout << "Process " << process_number << ": my lucky number ("
      << my_lucky_number << ") is less than the average (" << all_average
      << ")" << std::endl;
  } else if (my_lucky_number > all_average) {
    // Si el numero no es mas pequeno pero si es mas grande
    // que el promedio entonces entonces imprime que es mas grande
    std::cout << "Process " << process_number << ": my lucky number ("
      << my_lucky_number << ") is greater than the average (" << all_average
      << ")" << std::endl;
  } else {
    // Si no cumple con ningun de los requisitos de arriba, significa que
    // el lucky number igual que el promedio de todos los numeros
    std::cout << "Process " << process_number << ": my lucky number ("
      << my_lucky_number << ") is equal to the average (" << all_average
      << ")" << std::endl;
  }
  // Aqui busca entre todos los procesos cual es el numero mas grande
  // para imprimirlo
  if (my_lucky_number == all_max) {
    std::cout << "Process " << process_number << ": my lucky number ("
      << my_lucky_number << ") is the maximum (" << all_max << ")" << std::endl;
  }
}
