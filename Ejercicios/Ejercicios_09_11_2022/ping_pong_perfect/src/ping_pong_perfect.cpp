// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
// Already seen
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

// Logitud total que va a tener los mensajes que van a ser
// guardados y deplegados por un buffer estatico
const size_t MESSAGE_CAPACITY = 512;

/**
 * @brief Manda al programa a saludar una cantidad de veces
 * solicitadas por el usuario, a diferencia del anterior programa
 * este utiliza un control de concurrencia diferente y es el
 * conditional safe ya que cada elemento va guardando su saludo
 * en diferentes partes de la memoria para luego ser desplegadas
 * @param process_number Numero del proceso que va a saludar
 * @param process_count Cantidad de procesos que van a saludar
 * @param process_hostname Maquina por donde se saluda
 * @return void
 */
void simulate_ping_pong(int process_number, int process_count, const char* process_hostname);

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
    // Metodo greet
      simulate_ping_pong(process_number, process_count, process_hostname);
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

void simulate_ping_pong(int process_number, int process_count
    , const char* process_hostname) {
  // Aqui se crea el buffer que va a encargarse de
  // "guardar" los mensajes de los hilos para ser
  // desplegados luego
  std::stringstream buffer;
  // Aqui se le dice al buffer que se le meta el saludo
  // del hilo que acaba de entrar
  buffer << "Jugador  " << process_number
    << " lanzo la pelota al jugador 2 \n";
  // Si el proceso es diferente de 1 entonces se hace un send
  // para enviar la cadena/ saludo del hilo y guardarlo en una
  // variable locan dentro del ciclo de recv
  if (process_number != 0) {
    // Aqui se crea una variable local que va a contener la cadena
    // del hilo que entra en ese momento
    const std::string& message = buffer.str();
    // Aqui se le envia el mensaje/saludo al recv para  que lo imprima en
    // orden en el ciclo de abajo, en otras palabras en recv contiene el
    // saludo y si el indice es el correcto, este mete el saludo en una
    // variable y la despliega
    if (MPI_Send(message.data(), message.length(), MPI_CHAR, /*target*/ 0
      , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
      // Mensaje por si falla
      fail("could not send message");
    }

    // std::vector<char> message2(MESSAGE_CAPACITY);
    
    // if (MPI_Recv(&message2[0], MESSAGE_CAPACITY, MPI_CHAR, 0
    //     , /*tag*/ 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
    //     fail("could not receive message");
    // }

    // std::cout << message2[0] << std::endl;
    
  } else {
    // Ciclo que se utiliza para imprimir los datos

      std::vector<char> message(MESSAGE_CAPACITY);
      if (MPI_Recv(&message[0], MESSAGE_CAPACITY, MPI_CHAR, 1
        , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
        fail("could not receive message");
      }
      // Aqui imprime el mensaje de uno de los procesos
      std::cout << &message[0] << std::endl;
      std::cout << "Jugador 2 recibio la pelota y la lanzo al jugador " << process_count -1
      << std::endl;


  std::stringstream buffer;

  buffer << "Jugador  " << process_number
    << " lanzo la pelota al jugador 1 y la recibe \n";

      // const std::string& message2 = buffer.str();
      // if (MPI_Send(message2.data(), message2.length(), MPI_CHAR, /*target*/ 0
      //   , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
      //   // Mensaje por si falla
      //   fail("could not send message");
      // }
  }
}
