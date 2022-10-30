// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

// Already seen
#include <mpi.h>
// Already seen
#include <cstdlib>
// Already seen
#include <iostream>
// Already seen
#include <stdexcept>

// DistributedExeption(process_number, exception_code, msg)
#define fail(msg) throw std::runtime_error(msg)


/**
 * @brief Manda al programa a saludar una cantidad de veces
 * solicitadas por el usuario, se utiliza un mecanismo para
 * controlar la salida como si fuera un semaforo y se le llama
 * recive_message y send_message
 * @param process_number Numero del proceso que va a saludar
 * @param process_cout Cantidad de procesos que van a saludar
 * @param process_hostname Maquina por donde se saluda
 * @return void
 */
void greet(int process_number, int process_count, const char* process_hostname);

// Subrutina principal que llama a todos los procesos del
// programa
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;

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
    // Metodo greet
      greet(process_number, process_count, process_hostname);
    } catch (const std::runtime_error& exception) {
      // Aqui entra si hay algun tipo de problema a la
      // hora de saludar
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

void greet(int process_number, int process_count
    , const char* process_hostname) {
  const int previous_process = (process_count + process_number - 1)
    % process_count;
  const int next_process = (process_number + 1) % process_count;
  bool can_print = true;

  if (process_number > 0) {
    // receive(&can_print, 1, previous_process)
    // Simula un wait de un semaforo, esto ayuda en la comunicacion
    // entre hilos ya que ejecutara el codigo de adentro solo si
    // entra a este recv
    if (MPI_Recv(&can_print, /*count*/ 1, MPI_C_BOOL, previous_process
      , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
      fail("could not receive message");
    }
  }
  // Aqui imprime el saludo cada uno de los hilos
  std::cout << "Hello from main thread of process " << process_number
    << " of " << process_count << " on " << process_hostname << std::endl;

  // send(can_print, 1, next_process);
  // Este envia un mensaje al siguiente hilo diciendo que puede imprimir su
  // mensaje correspondiente, entonces envia una variable boleana de por
  // referencia junto con la cantidad de variables, o sea 1, un MPI_C_BOOL,
  // el siguiente procesos que quiere que imprima y el mundo o equipo de hilos
  if (MPI_Send(&can_print, /*count*/ 1, MPI_C_BOOL, next_process, /*tag*/ 0
    , MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("could not send message");
  }
}
