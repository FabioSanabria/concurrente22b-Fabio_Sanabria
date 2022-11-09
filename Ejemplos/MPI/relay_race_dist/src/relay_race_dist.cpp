// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <mpi.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#define fail(msg) throw std::runtime_error(msg)

/**
 * @brief Simula el inicio de la carrera de relevos, en ella
 * se inicializan varias variables con los parametros enviados
 * por el usuario para luego llamar a las demas subrutinas que
 * simulan ahora si las diferentes etapas de la carrera de relevos
 * @param argc Numero de argumentos ingresados por el usuario
 * @param argv Argumentos que envia el usuario y almacenados en
 * memoria
 * @param process_count Cantidad de procesos que van a saludar
 * @param process_hostname Maquina por donde se saluda
 * @return void
 */
void simulate_relay_race(int argc, char* argv[], int process_number
  , int process_count);

/**
 * @brief Simula la primera parte de la carrera en donde se encuentra
 * la barrera que simula el inicio de la carrera, cuando todos los
 * procesos estan listos corren y le pasan el baton al siguiente
 * proceso
 * @param stage1_delay Tiempo que se dura recorriendo la primera
 * parte del circuito
 * @param argv Argumentos que envia el usuario y almacenados en
 * memoria
 * @param procces_number Numero del proceso que esta corriendo
 * en la primera parte
 * @param team_count Numero de equipos que corren en la pista
 * si son 8 procesos entonces son 4 equipos
 * @return void
 */
void run_stage1(int stage1_delay, int process_number, int team_count);

/**
 * @brief Simula la segunda parte de la carrera en donde los procesos
 * ya tienen el baton y siguen corriendo hasta llegar a la linea de
 * meta que es un mutex dado por el arbitro
 * @param stage2_delay Tiempo que se dura recorriendo la segunda
 * parte del circuito
 * @param argv Argumentos que envia el usuario y almacenados en
 * memoria
 * @param procces_number Numero del proceso que esta corriendo
 * en la primera parte
 * @param team_count Numero de equipos que corren en la pista
 * si son 8 procesos entonces son 4 equipos
 * @return void
 */
void run_stage2(int stage2_delay, int process_number, int team_count);

/**
 * @brief Simula al arbitro que dice el lugar en el que llegaron
 * los equipos creados anteriormente
 * @param team_count Numero de equipos que corren en la pista
 * si son 8 procesos entonces son 4 equipos
 * @return void
 */
void referee(int team_count);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  // Inicia MPI para poder utilizar todas las herramientas que nos
  // brinda esta tecnologia
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    try {
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
    // Aqui llama a la subrutina relay_Race que simula una carrera
    // de relevos utilizando las herramientas dadas por MPI
    simulate_relay_race(argc, argv, process_number, process_count);
    // Excepciones por si algo falla
  } catch (const std::runtime_error& exception) {
    // Mensaje por si algo sale mal
    std::cout << exception.what() << std::endl;
    error = EXIT_FAILURE;
  }
  // Termina todos los procesos de MPI cuando todo lo anteriror
  // se ejecuta
    MPI_Finalize();
  } else {
    std::cerr << "error: could not init MPI" << std::endl;
    error = EXIT_FAILURE;
  }
  return error;
}

void simulate_relay_race(int argc, char* argv[], int process_number
  , int process_count) {
    // Agarra los argumentos de la linea de comandos
  if (argc == 3) {
    // La cantidad de jugadores debe de ser mayor a 3
    // y par para dividirlos en equipos
    if (process_count >= 3 && process_count % 2 == 1) {
      // Aqui obtenemos la cantidad de equipos, si el
      // usuario solicito 8 procesos entonces tiene 4
      // equipos
      const int team_count = (process_count - 1) / 2;
      // Duracion de la primera parte de la carrera de
      // relevos, en ella esta la barrera inicial que
      // mantiene a todos los procesos quietos
      const int stage1_delay = atoi(argv[1]);
      // Tiempo que se dura en recorrer toda la
      // segunda etapa hasta llegar al arbitro y
      // les da el lugar en el que llegaron
      const int stage2_delay = atoi(argv[2]);
      // El proceso 0 llama a todos los metodos para que
      // inicien con la ejecucion y la comunicacion directa
      if (process_number == 0) {
        // Aqui llama al metodo referee
        referee(team_count);
      } else if (process_number <= team_count) {
      // Aqui llama a la primera parte de la carrera
        run_stage1(stage1_delay, process_number, team_count);
      } else {
        // Aqui llama a la segunda parte de la carrera luego
        // de que los procesos le pasaran el baton a otro
        // proceso
        run_stage2(stage2_delay, process_number, team_count);
      }
    } else {
      // Error por si el numero es menor a 3 o impar
      fail("error: process count must be odd and greater than 3");
    }
  } else {
    // Error por si no ponen los delays
    fail("usage: relay_race_dist stage1_delay stage2_delay");
  }
}

void run_stage1(int stage1_delay, int process_number, int team_count) {
  // wait_barrier()
  // Inicia la barrera que va a obligar a todos los procesos a esperar
  // hasta un momento dado
  if (MPI_Barrier(MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could wait for barrier");
  }
  // EL tiempo que va a durar este proceso en ejecutarse
  // puede que un proceso/"hilo" se retrase ya que hay
  // indeterminismo en este punto
  usleep(1000 * stage1_delay);
  // Este va a ser la pareja del proceso que esta ahora mismo en la
  // pista 1, le debe de hacer baton al peer
  const int peer = process_number + team_count;
  // Aqui pone el baton en true para que cuando se mande el mensaje
  // el otro proceso se de cuenta de que ya puede iniciar su carrera
  bool baton = true;
  // send(&baton, 1, peer)
  // Aqui le envia al siguiente proceso que ya puede correr y ejecutar
  // su parte
  if (MPI_Send(&baton, /*count*/ 1, MPI_C_BOOL, peer, /*tag*/ 0
    , MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not send baton");
  }
}

void run_stage2(int stage2_delay, int process_number, int team_count) {
  // wait_barrier()
  // Barrera que detiene los procesos hasta que la pista 1 de la senal
  if (MPI_Barrier(MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could wait for barrier");
  }
  // Numero de equipo que llega en un momento determinado de la
  // ejecucion
  int peer = process_number - team_count;
  // El baton solo se inicializa en false, abajo se cambiara si llega
  // un proceso y si el comunicador asi lo desea
  bool baton = false;
  // receive(&baton, 1, peer)
  // Aqui recibe el mensaje enviado por comunicador/pareja del proceso
  // que esta ejecutando esta parte del codigo, aqui dice cuando el
  // proceso termina para que inicie el proceso 2
  if (MPI_Recv(&baton, /*capacity*/ 1, MPI_C_BOOL, /*source*/ peer
    , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
    fail("could not receive baton");
  }
  // Aqui el proceso se duerme para simular la carrera
  usleep(1000 * stage2_delay);
  // send(&peer, 1, 0)
  // Aqui envia una senal al referi diciendo que ya termino su ejecucion
  // para que le de el lugar en el que quedo
  if (MPI_Send(&peer, /*count*/ 1, MPI_INT, /*target*/ 0, /*tag*/ 0
    , MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not send team number to referee");
  }
}

void referee(int team_count) {
  // Aqui el arbitro inicia cronometro para ver
  // cuanto dura cada equipo en llegar
  const double start_time = MPI_Wtime();
  // wait_barrier()
  // Barrera que atrasa a todos los procesos para que realicen
  // todos los procesos anterior mencionados
  if (MPI_Barrier(MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could wait for barrier");
  }
  // Inicializa la variable place
  int place = 0;
  // Inicia el ciclo para dar los lugares en los que quedaron
  // los procesos
  for (int index = 0; index < team_count; ++index) {
    int team = 0;
    // receive(&team, 1, any_process)
    // Aqui recibe al equipo que termino la carrera, se guarda la variable
    // en team para luego imprimir los datos
    if (MPI_Recv(&team, /*capacity*/ 1, MPI_INT, MPI_ANY_SOURCE
      , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
      fail("error: could not receive team number");
    }
    const double elapsed = MPI_Wtime() - start_time;
    ++place;
    std::cout << "Place " << place << ": team " << team << " in " << elapsed
      << "s" << std::endl;
  }
}
