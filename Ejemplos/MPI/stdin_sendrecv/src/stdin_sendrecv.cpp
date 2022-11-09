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
void process_values(int process_number, int process_count
  , const char* process_hostname);

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
      process_values(process_number, process_count, process_hostname);
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

void process_values(int process_number, int process_count
  , const char* process_hostname) {
  // Vector en donde se va a guardar los datos ingresados
  // por el usuario
  std::vector<double> values;
  // Cantidad de valores ingresados
  size_t value_count = 0;
  // Si el proceso es el numero 0 entonces
  // significa que es el encargado de mandar los
  // mensajes y mantener la comunicacion entre procesos
  if (process_number == 0) {
    // Temporal para guardar los numeros ingresados
    double value = 0.0;
    // Ciclo encargado de perdirle al usuario los numeros
    // que desea para luego meterlos en el vector y
    // enviarlo a otros procesos
    while (std::cin >> value) {
      values.push_back(value);
    }
    // Se le hace un size al vector para obtener el
    // tamanio
    value_count = values.size();
    // Ciclo usado para mandar los mensajes a los diferentes
    // procesos que estan operando
    for (int target = 1; target < process_count; ++target) {
      // Este assert es usado por si en un futuro se utilizan
      // maquinas que usen enteros mayores o menores a 64 bits
      // esto es para evitar errores a futuro
      static_assert(sizeof(value_count) == sizeof(uint64_t)
        , "update MPI_Send data type to match your architecture");
      // Aqui le envia primero la cantidad de valores que se
      // ingresaron para evitar errores
      if (MPI_Send(&value_count, /*count*/ 1, MPI_UINT64_T, target
        , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        fail("could not send value count");
      }
      // En esta parte se envian los valores que va a analizar
      // el proceso target, o sea, todos los numeros ingresados
      // por el cliente
      if (MPI_Send(&values[0], value_count, MPI_DOUBLE, target
        , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        fail("could not send values");
      }
    }
  } else {
    // Aqui recibe la cantidad de elementos que tiene el vector de elemetos
    // pedidos por entrada estandar
    if (MPI_Recv(&value_count, /*capacity*/ 1, MPI_UINT64_T, /*source*/ 0
      , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
      fail("could not receive value count");
    }
    // Aqui se le hace un resize para evitar problemas de tamanio y que sea
    // fiel al tamanio previamente ingresado
    values.resize(value_count);
    // Aqui recibe los elementos ingresados por el usuario para luego
    // ser impresos en el ciclo de abajo
    if (MPI_Recv(&values[0], /*capacity*/ value_count, MPI_DOUBLE, /*source*/ 0
      , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
      fail("could not receive values");
    }
  }
  // Ciclo que imprime los valores que le llegaron al procesador mediante el
  // rcv
  for (size_t index = 0; index < values.size(); ++index) {
    std::cout << process_hostname << ":" << process_number << ".m: values["
      << index << "] == " << values[index] << std::endl;
  }
}
