// Copyright 2021 Fabio Sanabria fabio.sanabria@ucr.ac.cr> CC-BY 4.0
// Already seen
#include <mpi.h>
// Already seen
#include <stdint.h>
// Already seen
#include <stdio.h>
// Already seen
#include <stdlib.h>
// Already seen
#include <time.h>
// Already seen
#include <unistd.h>
// Already seen
#include <inttypes.h>
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
 * @brief Simulador de un juego de ping pong entre 2 procesos
 * que van lanzando mensajes y va incrementando los puntos entre
 * @param process_number Numero del proceso que va a saludar
 * @param luck_player_1 La suerte que tiene el jugador 1 al lanzar
 * la bola, es decir, el indice de exito
 * @param luck_player_1 La suerte que tiene el jugador 1 al lanzar
 * la bola, es decir, el indice de exito
 * @return void
*/
void simulate_ping_pong(int process_number, uint64_t luck_player_1,
uint64_t luck_player_2);

/**
 * @brief Analiza quien es el ganador y el que anoto la mayor
 * cantidad de puntos en la ejecucion del programa
 * @param process_number Numero del proceso que va a saludar
 * @param points_2 Puntos que ha logrado marcar el jugador 2
 * @param points_1 Puntos que ha logrado marcar el jugador 1
 * @return void
*/
void searchWiner(int points_1, int points_2);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  srand(time(NULL));
  uint64_t luck_player_1 = 0;
  uint64_t luck_player_2 = 0;

     if (argc == 3) {
         if (sscanf(argv[1], "%" SCNu64, &luck_player_1) == 1) {
         } else {
             // Error code
           fprintf(stderr, "Error: invalid thread count\n");
           return 11;
         }

         if (sscanf(argv[2], "%" SCNu64, &luck_player_2) == 1) {
         } else {
             // Error code
           fprintf(stderr, "Error: invalid thread count\n");
           return 11;
         }
    } else {
      fprintf(stderr,
      "Error: Ingrese los dos indice de exito de los tiros\n");
      return 11;
    }

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
    if (process_count != 2 || process_count == 1) {
      if (process_number == 0)
      throw std::runtime_error("Deben ser solo 2 jugadores");
      return EXIT_FAILURE;
    }
      simulate_ping_pong(process_number, luck_player_1, luck_player_2);
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

void simulate_ping_pong(int process_number, uint64_t luck_player_1,
uint64_t luck_player_2) {
  int puntos_totales_1 = 0;
  int puntos_totales_2 = 0;
  if (process_number == 1) {
    int puntos = 0;  // Los puntos del jugador 1
    int puntos_recv = 0;  // Los puntos del jugador 2 que manda jugador 1
    int puntos_2 = 0;  // los puntos del jugador 2 locales
    uint64_t number_rand = (uint64_t)rand() % 100;
    for (int i = 0; i < 30; i++) {
      if (number_rand < luck_player_1) {
        number_rand = (uint64_t)rand() % 100;
        ++puntos;
        if (MPI_Send(&puntos, 1, MPI_INT, /*target*/ 0
          , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
          // Mensaje por si falla
          fail("could not send message");
        }
        if (MPI_Recv(&puntos_recv, 1, MPI_INT, 0
            , /*tag*/0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
            fail("could not receive message");
        }
        puntos_2 = puntos_recv;
      } else {
        number_rand = (size_t)rand() % 100;
        if (MPI_Send(&puntos, 1, MPI_INT, /*target*/ 0
          , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
          // Mensaje por si falla
          fail("could not send message");
        }

        if (MPI_Recv(&puntos_recv, 1, MPI_INT, 0
            , /*tag*/0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
            fail("could not receive message");
        }
          puntos_2 = puntos_recv;
      }
    }
        puntos_totales_2 = puntos_2;
        std::cout <<"2. " << puntos_2 << std::endl;
        if (MPI_Send(&puntos_totales_2, 1, MPI_INT, /*target*/ 0
          , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
          // Mensaje por si falla
          fail("could not send message");
        }
  } else {
    if (process_number == 0) {
    // Ciclo que se utiliza para imprimir los datos
    int puntos = 0;  // LOs puntos del jugador 2
    int puntos_recv = 0;  // Los puntos del jugaror 1 enviados por 2
    int puntos_1 = 0;  // Variable que guarda los puntos del jugador 1
    size_t number_rand2 = (size_t)rand() % 100;
      for (int i = 0; i < 30; i++) {
        if (number_rand2 < luck_player_2) {
          number_rand2 = (size_t)rand() % 100;
          ++puntos;
          if (MPI_Recv(&puntos_recv, 1, MPI_INT, 1
            , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
            fail("could not receive message");
          }
          puntos_1 = puntos_recv;
          if (MPI_Send(&puntos, 1 , MPI_INT, /*target*/ 1
          , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
            // Mensaje por si falla
            fail("could not send message");
          }
          puntos_1 = puntos_recv;
        } else {
          number_rand2 = (size_t)rand() % 100;
          if (MPI_Recv(&puntos_recv, 1, MPI_INT, 1
            , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
            fail("could not receive message");
          }
          puntos_1 = puntos_recv;
          if (MPI_Send(&puntos, 1 , MPI_INT, /*target*/ 1
          , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
            // Mensaje por si falla
            fail("could not send message");
          }
        }
      }
      puntos_totales_1 = puntos_1;
      std::cout <<"1. " << puntos_1 << std::endl;

      if (MPI_Recv(&puntos_recv, 1, MPI_INT, 1
        , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
        fail("could not receive message");
      }
      searchWiner(puntos_totales_1, puntos_recv);
    }
  }
}

void searchWiner(int points_1, int points_2) {
  // Revisa cual de los dos jugadores tiene el mejor
  // puntaje
  if (points_1 < points_2) {
    std::cout << "EL jugador 2 ha ganado!" << std::endl;
  } else if (points_2 < points_1) {
    std::cout << "El jugador 1 ha ganado!" << std::endl;
  } else {
    std::cout << "Los jugadores han empatado!" << std::endl;
  }
}
