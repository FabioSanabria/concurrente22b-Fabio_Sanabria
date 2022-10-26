// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#ifndef RELAY_RACE_H
#define RELAY_RACE_H

#define _DEFAULT_SOURCE

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct shared_data {
  // Esto es la cantidad de hilos o
  // equipos de hilos que se desean usar
  size_t team_count;
  // Este va a ser el tiempo que tarde el
  // primer corredor en llegar al siguiente
  // stage
  useconds_t stage1_duration;
  // Este va a ser el tiempo que tarde el
  // primer corredor en atravezar la etapa
  // 2 de la carrera
  useconds_t stage2_duration;
  // Posicion en la que quedan los corredores
  // si quedan en primero, segundo y tercer lugar
  size_t position;
  // Barrera utilizada para hacer esperar a los hilos
  // esto es usado para simular a los corredores, si uno
  // de ellos no llega entonces el otro no puede seguir,
  // esto va a ser utilizado para mandar a ejecutar el
  // pthread_barrier_init
  pthread_barrier_t start_barrier;
  // Semaforos que simulan los relevos de los competidores
  // se necesitan crear varios semaforos de ese tipo
  sem_t* batons;
  // Mutex que controla la concurrencia de la variable position
  // para evitar las condiciones de carrera, ademas, ayuda a
  // imprimir los elementos en orden
  pthread_mutex_t finish_mutex;
} shared_data_t;

/**
 * @struct private_data_t
 * @brief crea memoria privada
 * estructura para crear la memoria
 * privada de los hilos
 * @elements thread_number Numero de hilo que se esta
 * ejecutando
 * @elements shared_data Datos compartidos entre hilos
*/
typedef struct private_data {
  // Numero de hilo que se esta
  // ejecutando
  size_t thread_number;  // rank
  // datos compartidos entre hilos
  shared_data_t* shared_data;
} private_data_t;

/**
 * @brief Subrutina responsable de la creacion de los hilos pedidos por el
 * usuario y utiliza memoria dinamica para crear una lista dinamica con el
 * numero de hilos como parametro para empezar la carrera
 * @param shared_data Shared_data of all the threads that we create
 * @return Error code, 0 if the executions success and 1 if the program fails
 */
int create_threads(shared_data_t* shared_data);

/**
 * @brief Subrutina que analiza los argumentas mandados por el usuario, si algun
 * argumento enviado es incorrecto, se le hace saber al usuario mediante un warning
 * que cometio algun error, pero, si manda los datos correctos entonces continua la
 * ejecucion y no hay problemas
 * @param arc Number of arguments given in the command prompts
 * @param argv[] a array of char* that constains the arguments given by the user
 * @param shared_data Shared_data of all the threads that we create
 * @return Error code, 0 if the executions success and 1 if the program fails
 */
int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data);

/**
 * @brief Subrutina que simula el inicio de la carrera de relevos, el inicio es
 * considerado como el primer corredor, osea, apenas inicia la ejecucion el start_Race
 * simula al primer corredor de los equipos corriendo hasta llegar al segundo corredor que
 * esta esperando hasta que el primer corredor le mande un "signal" para que pueda avanzar
 * (signal al semaforo del segundo corredor)
 * @param data Datos compartidos de los hilos
 * @return void
 */
void* start_race(void* data);

/**
 * @brief Subrutina que simula la entrada de premios de los hilos que llegaron
 * de primeros, es decir, si llega el hilo 2 de los semaforos de primero entonces
 * se le da el premio del primer lugar y asi sucesivamente
 * @param data Datos compartidos de los hilos
 * @return void
 */
void* finish_race(void* data);

// procedimiento main
int main(int argc, char* argv[]) {
  // Codigo de error
  int error = EXIT_SUCCESS;
  // Se hace un calloc al shared_Data para poder utilizarlo
  // con memoria dinamica
  shared_data_t* shared_data = (shared_data_t*)
    calloc(1, sizeof(shared_data_t));
  // Si el shared_data es creado
  if (shared_data) {
    // Analiza los argumentos
    error = analyze_arguments(argc, argv, shared_data);
    if (error == EXIT_SUCCESS) {
      // posicion se pone igual a 0 para agarrar desde el hilo inicial
      // hasta el final de los hilos
      shared_data->position = 0;
      // Se inicializa la barrera de los hilos
      error = pthread_barrier_init(&shared_data->start_barrier,
        /*attr*/ NULL, /*count*/ shared_data->team_count);
      // Semaforo de relevos que esperan hasta que se les indique mediante
      // un signal
      shared_data->batons = (sem_t*) calloc(shared_data->team_count
        , sizeof(sem_t));
      // Mutex que protege los resultados finales de la carrera de relevos
      error += pthread_mutex_init(&shared_data->finish_mutex, /*attr*/ NULL);
      // Si no hay errores y batons fue creado exitosamente
      if (error == EXIT_SUCCESS && shared_data->batons) {
      // Aqui inicializa todos los semaforos de relevos mediante un for
        for (size_t index = 0; index < shared_data->team_count; ++index) {
          sem_init(&shared_data->batons[index], /*pshared*/ 0, /*value*/ 0);
        }
        // Metodos para ver el tiempo de ejecucion
        struct timespec start_time, finish_time;
        clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &start_time);

        error = create_threads(shared_data);

        clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &finish_time);
        double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
          (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
        printf("execution time: %.9lfs\n", elapsed_time);
        // Fin de metodos para ver el tiempo de ejecucion
        // Aqui destruye los semaforos de relevos y libera la memoria para
        // evitar los memory leaks
        for (size_t index = 0; index < shared_data->team_count; ++index) {
          sem_destroy(&shared_data->batons[index]);
        }
        // Destruye el mutex que controla la
        // concurrencia del finish para liberar
        pthread_mutex_destroy(&shared_data->finish_mutex);
        // Libera el semaforo para evitar memory leaks
        free(shared_data->batons);
        // Destruye la barrera para liberar memoria
        pthread_barrier_destroy(&shared_data->start_barrier);
      } else {
        // Imprime un error si no se pudo inicializar el mutex
        fprintf(stderr, "error: could not init mutex\n");
        error = 11;
      }
    }
    // Libera el shared_data
    free(shared_data);
  } else {
    // No se pudo crear la memoria compartida
    fprintf(stderr, "error: could not allocated shared memory\n");
    error = 12;
  }

  return error;
}

int analyze_arguments(int argc, char* argv[]
    , shared_data_t* shared_data) {
  // Si los argumentos son iguales a 4
  if (argc == 4) {
    // El primer argumento es la cantidad de hilos que quiere utilzar
    // el usuario para la carrera de relevos
    if ( sscanf(argv[1], "%zu", &shared_data->team_count) != 1
      || shared_data->team_count == 0 ) {
      fprintf(stderr, "invalid team count: %s\n", argv[1]);
      return 11;
    }
    // La duracion que tarda el corredosren la primera etapa, osea, en la
    // etapa de la barrera
    if ( sscanf(argv[2], "%u", &shared_data->stage1_duration) != 1 ) {
      fprintf(stderr, "invalid stage 1 duration: %s\n", argv[2]);
      return 12;
    }
    // La duracion que tarda el corredor la segunda etapa, o sea, en la etapa
    // en la que se utilzzan los signals de los semaforos para indicar si los
    // corredores pueden pasar o no
    if ( sscanf(argv[3], "%u", &shared_data->stage2_duration) != 1 ) {
      fprintf(stderr, "invalid stage 2 duration: %s\n", argv[3]);
      return 13;
    }
    return EXIT_SUCCESS;
  } else {
    // Si el usuario no ingresa los valores o ingresa algo que es erroneo, salta
    // este anucio que dice que es lo que se tiene que
    // colocar para que el programa lo ejecute
    fprintf(stderr, "usage: relay_race teams stage1duration stage2duration\n");
    return 10;
  }
}

int create_threads(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  // La cantidad de hilos que se van a utilizar, se multiplica por 2 ya que
  // son 2 equipos con esa cantidad de hilos
  const size_t thread_count = 2 * shared_data->team_count;
// Crea un valor de tipo thread que se necesita para hacer funcionar el programa
// especificamente en la parte en donde se manda un thread como
// parametro en el create se realiza en memoria dinamica mediante
// un malloc para que los datos se puedan guardar en el heap
  pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t));
// Se crea private_data usando memoria dinamica del heap
  private_data_t* private_data = (private_data_t*)
    calloc(thread_count, sizeof(private_data_t));
  // Si threads ya fue creado y private_Data tambien
  if (threads && private_data) {
    // For que va iterando metiendo en el private data todos los atributos
    // de cada hilo como los datos compartidos o el numero de hilo, luego se
    // crea el hilo
    size_t thread_number_local = (shared_data->team_count) - 1;
    for (size_t index = 0; index < shared_data->team_count; ++index) {
      // A private data se le mete el numero de hilo en este momento de
      // la iteracion

      private_data[index].thread_number = thread_number_local;
      // Se le coloca el shared_Data que es el mismo para
      // todos los hilos
      private_data[index].shared_data = shared_data;
      // Aqui crea los hilos que se van a utilizar y llaman a
      // start race
      error = pthread_create(&threads[index], NULL, start_race
        , &private_data[index]);
      
      --thread_number_local;
      if (error) {
      // Error por si algo falla al crear los hilos
        fprintf(stderr, "error: could not create thread %zu\n", index);
        error = 21;
      }
    }
    // Esta parte es muy oarecida a la anterior solo que en lugar de ser
    // el inicio de la carrera, simula el final de esta encargandose de
    // llamar a los hilos que terminaron primero, por lo que se alnacena
    // de nuevo en memoria privada todos los datos como el numero de hilo
    // el shared_Data y se crean los hilos llamando al metodo finish_race

    for (size_t index = shared_data->team_count; index < thread_count;
        ++index) {
      // A private data se le mete el numero de hilo en este momento de
      // la iteracion
      private_data[index].thread_number = index;
      // Se le coloca el shared_Data que es el mismo para
      // todos los hilos
      private_data[index].shared_data = shared_data;
      // Aqui crea los hilos que se van a utilizar y llaman a
      // finish race
      error = pthread_create(&threads[index], NULL, finish_race
        , &private_data[index]);

      if (error) {
     // Error por si algo falla al crear los hilos
        fprintf(stderr, "error: could not create thread %zu\n", index);
        error = 21;
      }
    }
    // Aqui le hace un join a los threads para que puedan liberar toda la
    // memoria que utilizaron en todas las ejecuciones y vueltas del
    // programa
    for (size_t index = 0; index < thread_count; ++index) {
      pthread_join(threads[index], NULL);
    }
    // Libera los datos privados
    free(private_data);
    // Libera los threads que se inicializaron en
    // la partee de arriba
    free(threads);
  } else {
    // Error por si no se pudo meter memoria a los threads
    fprintf(stderr, "error: could not allocate memory for %zu threads\n"
      , shared_data->team_count);
    error = 22;
  }
  // Error code
  return error;
}

void* start_race(void* data) {
  // Aqui se le hace un parse para poder utilizar una
  // variable de tipo privado
  private_data_t* private_data = (private_data_t*)data;
  // Aqui se le extrae la memoria compartida del private
  // data y la utilizamos para meterla dentro de una
  // local
  shared_data_t* shared_data = private_data->shared_data;
  // Numero que tiene el hilo que ejecuta este metodo
  const size_t rank = private_data->thread_number;
  // Asignacion de rank a team_number
  const size_t team_number = rank;
  // Aqui se encuentra la barrera, esta barrera va a
  // esperar a que todos los hilos ingresador
  // en team_count puedan llegar
  pthread_barrier_wait(&shared_data->start_barrier);
  // Tiempo en la  que el hilo/corredor dura para terminar
  // la primera etapa
  usleep(1000 * shared_data->stage1_duration);
  // Ya termino la primera etapa por lo que se incrementa el
  // semaforo de la posicion team number para continuar su
  // ejecucion
  sem_post(&shared_data->batons[team_number]);
  return NULL;
}

void* finish_race(void* data) {
  // Aqui se le hace un parse para poder utilizar una
  // variable de tipo privado
  private_data_t* private_data = (private_data_t*)data;
    // Aqui se le extrae la memoria compartida del private
  // data y la utilizamos para meterla dentro de una
  // local
  shared_data_t* shared_data = private_data->shared_data;
// Numero que tiene el hilo que ejecuta este metodo
  const size_t rank = private_data->thread_number;
  // Asignacion de rank - la cantidad de hilos a team_number
  const size_t team_number = rank - shared_data->team_count;
  assert(team_number < shared_data->team_count);
  // Semaforo que indica si es turno de que el corredor 2 de
  // la pista de relevos puede correr, si anteriormente la
  // barrera le hizo un signal a este semaforo entonces se
  // ejecutan todos los procesos dentro de este
  // wait(batons[team_number])
  sem_wait(&shared_data->batons[team_number]);
  // Tiempo en la  que el hilo/corredor dura para terminar
  // la etapa final
  usleep(1000 * shared_data->stage2_duration);
// Mutex que controla las impresiones de los lugares de cada
// hilo
  pthread_mutex_lock(&shared_data->finish_mutex);
  // Incrementa la posicion del hilo, si llego de primero significa
  // que gano la carrera y los demas hilos tambien van a tener
  // su propio lugar
  const size_t our_position = ++shared_data->position;
  // if (our_position <= 3) {
    // Imprime el lugar en el que quedo el hilo
    printf("Place %zu: team %zu\n", our_position, team_number);
  // }
  // Cierra el control de concurrencia
  pthread_mutex_unlock(&shared_data->finish_mutex);

  return NULL;
}
#endif
