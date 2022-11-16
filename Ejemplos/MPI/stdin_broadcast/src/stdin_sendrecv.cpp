// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

// Already seen
#include <mpi.h>
// Already seen
#include <cstdint>
// Already seen
#include <cstdlib>
// Already seen
#include <iostream>
// Already seen
#include <sstream>
// Already seen
#include <stdexcept>
// Already seen
#include <vector>

// Clase excepcion por si algo falla
#define fail(msg) throw std::runtime_error(msg)

/**
 * @brief Metodo que se encarga de analizar los valores de
 * la entrada estandar ingresados por el usuario para que
 * llegue uno de los procesadores y los imprima en la salida
 * estandar
 * @param process_number Numero del proceso que va a saludar
 * @param process_cout Cantidad de procesos que van a saludar
 * @param process_hostname Maquina por donde se saluda
 * @return void
 */
void process_values(int process_number, const char* process_hostname);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  // Inicia MPI para poder utilizar todas las herramientas que nos
  // brinda
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    // rank
    int process_number = -1;
    // El numero de proceso/ hilo que esta ejecutando
    // el programa, si solo operamos en una sola zona
    // de trabajo, entonces solo tendremos y procces_count
    // y por lo tanto, 1 process_number
    MPI_Comm_rank(MPI_COMM_WORLD, &process_number);
    // Se coloca en -1 para ver si a la hora de
    // imprimir hay algun tipo de error de salida
    int process_count = -1;
    // Guarda en la variable process_count, la cantidad
    // de zonas/ maquinas que estan ejecutando este pro-
    // grama
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);
    // Esto lo que hace es que aparte una parte de memoria
    // para poder guardar el nombre de la maquina que esta
    // ejecutando el programa, esto con el objetivo de no
    // gastar memoria innecesaria
    char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
    // Se coloca en -1 para ver si a la hora de
    // imprimir hay algun tipo de error de salida
    int hostname_length = -1;
    // Obtiene el nombre del procesador que ejecuta el programa
    // en este caso se llama Fabiosv-VirtualBox
    MPI_Get_processor_name(process_hostname, &hostname_length);
    // Aqui llama a la subrutina greet para que todos los hilos
    // saluden, se utiliza un "semaforo" para lograr este
    // cometido
    try {
    // Marca el inicio del tiempo utilizando wall_time de MPI
      const double start_time = MPI_Wtime();
      // Llama al metodo para leer los datos de la entrada estandar
      // para que se puedan desplegar en la salida estandar
      process_values(process_number, process_hostname);
      // Tiempo trancurrido durante la ejecucion de una de las
      // maquinas
      const double elapsed = MPI_Wtime() - start_time;
      // Aqui imprime el tiempo transcurrido de los procesos
      std::cout << process_hostname << ":" << process_number
        << ".m: elapsed time " << elapsed << "s" << std::endl;
    } catch (const std::runtime_error& exception) {
      std::cerr << "error: " << exception.what() << std::endl;
      error = EXIT_FAILURE;
    }
    // Terminan los procesos de MPI
    MPI_Finalize();
  } else {
    std::cerr << "error: could not init MPI" << std::endl;
    error = EXIT_FAILURE;
  }
  return error;
}

void process_values(int process_number, const char* process_hostname) {
  // Vector de numeros ingresados
  // por el usuario
  std::vector<double> values;
  // Cantidad de elementos ingresados por
  // el usuario
  size_t value_count = 0;
  // En esta parte se van a meter los elementos
  // dentro del vector para guardarlos y posteriormente
  // enviarlos mediante un broadcast
  if (process_number == 0) {
    double value = 0.0;
    while (std::cin >> value) {
      values.push_back(value);
    }
    // Aqui se guarda la cantidad de elementos
    value_count = values.size();
  }
  // Revisa si cumple con las condiciones de 64 bits
  static_assert(sizeof(value_count) == sizeof(uint64_t)
    , "update MPI_Send data type to match your architecture");
  // Aqui se utiliza el broadcast para lograr comunicarse
  // con todos los procesos, es mas eficiente que utilizar send y
  // recieve, y ademas es mucho mas facil
  if (MPI_Bcast(&value_count, /*count*/ 1, MPI_UINT64_T, /*root*/ 0
    , MPI_COMM_WORLD) != MPI_SUCCESS ) {
    fail("could not broadcast value count");
  }
  // Resize del vector para evitar malentendidos
  values.resize(value_count);
  // Broadcast para enviar los valores contenidos en el vector
  // de values para que puedan ser utilzados por los demas procesos
  // no es necesario un receive ya que se difunde la informacion a
  // todos los procesos presentes
  if (MPI_Bcast(&values[0], value_count, MPI_DOUBLE, /*root*/ 0
    , MPI_COMM_WORLD) != MPI_SUCCESS ) {
    fail("could not broadcast values");
  }
  // For que imprime los valores ingresados por el usuario,
  // cada procesador imprime los mismos numeros
  for (size_t index = 0; index < values.size(); ++index) {
    std::cout << process_hostname << ":" << process_number << ".m: values["
      << index << "] == " << values[index] << std::endl;
  }
}
