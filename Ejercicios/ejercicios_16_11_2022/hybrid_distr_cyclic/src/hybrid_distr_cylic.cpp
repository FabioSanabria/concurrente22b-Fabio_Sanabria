// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

// Already seen
#include <mpi.h>
// Already seen
#include <omp.h>
// Libreria con varios algoritmos importantes como obtener el minimo
// de varios numeros
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

// Este encabezado define varias funciones de propósito general,
// incluida la gestión de memoria dinámica, la generación de
// números aleatorios, la comunicación con el entorno, la
// aritmética de enteros, la búsqueda, la clasificación y la conversión.
#include <cstdlib>
// Already seen
#include <iostream>

#define fail(msg) throw std::runtime_error(msg)
/**
 * @brief Calcula el numero inicial del bloque, o sea el inicio del
 * intervalo de tareas Ej : [3, 6] calcula el 3
 * @param workers Cantidad de hilos totales que crea el usuario
 * @param begin Inicio de la cantidad total de tareas que ingresa el usuario
 * es usado para calcular cuantas tareas le toca al hilo
 * @param end Fin de la cantidad total de tareas que ingresa el usuario
 * es usado para calcular cuantas tareas le toca al hilo
 * @param rank Numero de hilo que llega a ejecutar el proceso
 * @return devuelve el inicio de la tarea que va a ejecutar un hilo
*/
int calculate_start(int rank, int end, int workers, int begin);

/**
 * @brief Calcula el numero final del bloque, o sea el fin del
 * intervalo de tareas Ej : [3, 6] calcula el 6
 * @param workers Cantidad de hilos totales que crea el usuario
 * @param begin Inicio de la cantidad total de tareas que ingresa el usuario
 * es usado para calcular cuantas tareas le toca al hilo
 * @param end Fin de la cantidad total de tareas que ingresa el usuario
 * es usado para calcular cuantas tareas le toca al hilo
 * @param rank Numero de hilo que llega a ejecutar el proceso
 * @return devuelve el inicio de la tarea que va a ejecutar un hilo
*/
int calculate_finish(int rank, int end, int workers, int begin);

int main(int argc, char* argv[]) {
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    const double start_time = MPI_Wtime();
  // Inicializa el ambiente de ejecucion de MPI
  // para poder utilizar todas sus funciones
    int process_number = -1;  // rank
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
    char process_hostname[MPI_MAX_PROCESSOR_NAME];
    // Se coloca en -1 para ver si a la hora de
    // imprimir hay algun tipo de error de salida
    int hostname_length = -1;
    // Obtiene el nombre del procesador que ejecuta el programa
    // en este caso se llama Fabiosv-VirtualBox
    MPI_Get_processor_name(process_hostname, &hostname_length);
    // Obtenemos la cantidad de tareas que va a realizar diversas
    // maquinas, el inicio simboliza la tarea por donde va a empezar
    // y el finish significa el final de las tareas, simbolizado
    // mediante un intervalo se puede ver de la siguiente forma
    // [3, 20]
    if (argc == 3) {
      // Inicio de las tareas
      const int overall_start = atoi(argv[1]);
      // Indica cual es la tarea final que se deben repartir
      // los hilos
      const int overall_finish = atoi(argv[2]);
      // Aqui guarda en el process_start la formula de bloque para
      // calcular desde que tarea se va a encargar X hilo
      const int process_start = calculate_start(process_number, overall_finish
        , process_count, overall_start);
      // Aqui guarda en el process_finish la formula de bloque para
      // calcular desde que tarea se va a encargar X hilo
      const int process_finish = calculate_finish(process_number, overall_finish
        , process_count, overall_start);
      // La cantidad de procesos que va a realizar un hilo, si empieza desde el
      // 3 y termina en el 6 entonces realiza 3 procesos
      const int process_size = process_finish - process_start;
      // En este print nos dice la cantidad de procesos que se van a repartir
      // todos los hilos de ejecucion
      std::cout << process_hostname << ':' << process_number << ": range ["
        << process_start << ", " << process_finish << "[ size " << process_size
        << std::endl;
      // Aqui inicia la parte concurrente, crea los hilos para que se puedan
      // dividir las tareas pedidas anteriormente
      #pragma omp parallel default(none) shared(std::cout, process_hostname) \
        shared(process_number, process_start, process_finish, start_time)
      {  // NOLINT(whitespace/braces)
        // El inicio del trabajo que va a realizar el hilo
        int thread_start = -1;
        // Hasta donde va a llegar el hilo al realizar el trabajo
        // ej: este hilo realiza el trabajo desde el proceso 3
        // al 7
        int thread_finish = -1;
        // Aqui se dividen los deberes, se utiliza mapeo dinamico
        #pragma omp for schedule(static, 1)
        // ciclo que reparte todas las tareas pedidas anteriormente
        for (int index = process_start; index < process_finish; ++index) {
          // Revisa si el hilo ya tiene un inicio, si no lo tiene entonces
          // le agrega
          if (thread_start == -1) {
            // Le da el inicio del trabajo
            thread_start = index;
          }
          // Si ya tiene un inicio entonces se le agrega un final
          thread_finish = index;
        }
      // Aqui le incrementa a thread finish, lo que se quiere hacer es que como
      // el paralel divide las iteraciones del ciclo for entonces a cada
      // hilo se le va a asignar una cantidad de trabajo prevista debido al
      // tipo de mapeo, entonces si por ejemplo se inicia en 3, entonces va a
      // realizar las iteraciones que pide el mapeo de bloque hasta llegar (por
      // ejemplo) a 5 y asi va con todas las iteraciones
        ++thread_finish;
        // Cantidad de tareas que le tocan a X hilo de la ejecucion
        const int thread_size = thread_finish - thread_start;
      // Mutex que protege de la condicion de carrera de los hilos
        #pragma omp critical(can_print)
        // Imprime las tareas que le corresponden a cada hilo que llega a
        // ejecutar este codigo, si le toca las tareas de la 3 a la 5, de la
        // 5 a la 8 y asi sucesivamente
        std::cout << '\t' << process_hostname << ':' << process_number << '.'
          << omp_get_thread_num() << ": range [" << thread_start << ", " <<
          thread_finish << "[ size " << thread_size << std::endl;

        // Aqui dice la cantidad de tiempo que duro cada proceso
        const double elapsed = MPI_Wtime() - start_time;
        // Aqui imprime lo que duro cada proceso
        #pragma omp critical(can_print)
          std::cout << process_hostname << ':' << process_number << '.'
          << omp_get_thread_num() << ": range [" << thread_start << ", " <<
          thread_finish << "[ size " << thread_size <<  " in " <<
          elapsed << "s" <<std::endl;
      }
    } else {
      // Inicializa las variables de inicio y final
        int overall_start = 0;
        int overall_finish = 0;
      if (process_number == 0) {
        // Pide los datos al usuario en la entrada estandar
        fscanf(stdin, "%d", &overall_start);
        fscanf(stdin, "%d", &overall_finish);
          // Se elimino el coclo y ahora se usa broadcast sin mas ya que este
          // envia todos los elementos necesarios a otros procesos sin necesidad
          // de iterar
          if (MPI_Bcast(&overall_start, /*count*/ 1, MPI_INT, /*root*/ 0
            , MPI_COMM_WORLD) != MPI_SUCCESS ) {
            fail("could not broadcast value count");
          }

          if (MPI_Bcast(&overall_finish, /*count*/ 1, MPI_INT, /*root*/ 0
            , MPI_COMM_WORLD) != MPI_SUCCESS ) {
            fail("could not broadcast value count");
          }
        const int process_start = calculate_start(process_number, overall_finish
          , process_count, overall_start);
        // Aqui guarda en el process_finish la formula de bloque para
        // calcular desde que tarea se va a encargar X hilo
        const int process_finish = calculate_finish(process_number,
        overall_finish, process_count, overall_start);
        // La cantidad de procesos que va a realizar un hilo,
        // si empieza desde el
        // 3 y termina en el 6 entonces realiza 3 procesos
        const int process_size = process_finish - process_start;
        // En este print nos dice la cantidad de procesos que se van a repartir
        // todos los hilos de ejecucion
        std::cout << process_hostname << ':' << process_number << ": range ["
          << process_start << ", " << process_finish <<
          "[ size " << process_size
          << std::endl;
        // Aqui inicia la parte concurrente, crea los hilos para que se puedan
        // dividir las tareas pedidas anteriormente
        #pragma omp parallel default(none) shared(std::cout, process_hostname) \
          shared(process_number, process_start, process_finish, start_time)
        {  // NOLINT(whitespace/braces)
          // El inicio del trabajo que va a realizar el hilo
          int thread_start = -1;
          // Hasta donde va a llegar el hilo al realizar el trabajo
          // ej: este hilo realiza el trabajo desde el proceso 3
          // al 7
          int thread_finish = -1;
          // Aqui se dividen los deberes, se utiliza mapeo dinamico
          #pragma omp for schedule(static, 1)
          // ciclo que reparte todas las tareas pedidas anteriormente
          for (int index = process_start; index < process_finish; ++index) {
            // Revisa si el hilo ya tiene un inicio, si no lo tiene entonces
            // le agrega
            if (thread_start == -1) {
              // Le da el inicio del trabajo
              thread_start = index;
            }
            // Si ya tiene un inicio entonces se le agrega un final
            thread_finish = index;
          }
        // Aqui le incrementa a thread finish,
        // lo que se quiere hacer es que como
        // el paralel divide las iteraciones del ciclo for entonces a cada
        // hilo se le va a asignar una cantidad de trabajo prevista debido al
        // tipo de mapeo, entonces si por ejemplo se inicia en 3, entonces va a
        // realizar las iteraciones que pide el mapeo de bloque
        // hasta llegar (por ejemplo) a 5 y asi va con todas las iteraciones
          ++thread_finish;
          // Cantidad de tareas que le tocan a X hilo de la ejecucion
          const int thread_size = thread_finish - thread_start;
        // Mutex que protege de la condicion de carrera de los hilos
          #pragma omp critical(can_print2)
          // Imprime las tareas que le corresponden a cada hilo que llega a
          // ejecutar este codigo, si le toca las tareas de la 3 a la 5, de la
          // 5 a la 8 y asi sucesivamente
          std::cout << '\t' << process_hostname << ':' << process_number << '.'
            << omp_get_thread_num() << ": range [" << thread_start << ", " <<
            thread_finish << "[ size " << thread_size << std::endl;

          // Aqui dice la cantidad de tiempo que duro cada proceso
          const double elapsed = MPI_Wtime() - start_time;
          // Aqui imprime lo que duro cada proceso
          #pragma omp critical(can_print2)
            std::cout << process_hostname << ':' << process_number << '.'
            << omp_get_thread_num() << ": range [" << thread_start << ", " <<
            thread_finish << "[ size " << thread_size <<  " in " <<
            elapsed << "s" <<std::endl;
        }
      } else {
        // Aqui esperan hasta que el proceso 0 le pase a todos de manera global
        // los datos que necesitan
          if (MPI_Bcast(&overall_start, /*count*/ 1, MPI_INT, /*root*/ 0
            , MPI_COMM_WORLD) != MPI_SUCCESS ) {
            fail("could not broadcast value count");
          }

          if (MPI_Bcast(&overall_finish, /*count*/ 1, MPI_INT, /*root*/ 0
            , MPI_COMM_WORLD) != MPI_SUCCESS ) {
            fail("could not broadcast value count");
          }
        // Aqui guarda en el process_start la formula de bloque para
        // calcular desde que tarea se va a encargar X hilo
        const int process_start = calculate_start(process_number, overall_finish
          , process_count, overall_start);
        // Aqui guarda en el process_finish la formula de bloque para
        // calcular desde que tarea se va a encargar X hilo
        const int process_finish = calculate_finish(process_number,
        overall_finish, process_count, overall_start);
        // La cantidad de procesos que va a realizar
        // un hilo, si empieza desde el
        // 3 y termina en el 6 entonces realiza 3 procesos
        const int process_size = process_finish - process_start;
        // En este print nos dice la cantidad de procesos que se van a repartir
        // todos los hilos de ejecucion
        std::cout << process_hostname << ':' << process_number << ": range ["
          << process_start << ", " << process_finish
          << "[ size " << process_size << std::endl;
        // Aqui inicia la parte concurrente, crea los hilos para que se puedan
        // dividir las tareas pedidas anteriormente
        #pragma omp parallel default(none) shared(std::cout, process_hostname) \
          shared(process_number, process_start, process_finish, start_time)
        {  // NOLINT(whitespace/braces)
          // El inicio del trabajo que va a realizar el hilo
          int thread_start = -1;
          // Hasta donde va a llegar el hilo al realizar el trabajo
          // ej: este hilo realiza el trabajo desde el proceso 3
          // al 7
          int thread_finish = -1;
          // Aqui se dividen los deberes, se utiliza mapeo dinamico
          #pragma omp for schedule(static, 1)
          // ciclo que reparte todas las tareas pedidas anteriormente
          for (int index = process_start; index < process_finish; ++index) {
            // Revisa si el hilo ya tiene un inicio, si no lo tiene entonces
            // le agrega
            if (thread_start == -1) {
              // Le da el inicio del trabajo
              thread_start = index;
            }
            // Si ya tiene un inicio entonces se le agrega un final
            thread_finish = index;
          }
        // Aqui le incrementa a thread finish,
        // lo que se quiere hacer es que como
        // el paralel divide las iteraciones del ciclo for entonces a cada
        // hilo se le va a asignar una cantidad de trabajo prevista debido al
        // tipo de mapeo, entonces si por ejemplo se inicia en 3, entonces va a
        // realizar las iteraciones que pide el mapeo de bloque
        // hasta llegar (por ejemplo) a 5 y asi va con todas las iteraciones
          ++thread_finish;
          // Cantidad de tareas que le tocan a X hilo de la ejecucion
          const int thread_size = thread_finish - thread_start;
        // Mutex que protege de la condicion de carrera de los hilos
          #pragma omp critical(can_print3)
          // Imprime las tareas que le corresponden a cada hilo que llega a
          // ejecutar este codigo, si le toca las tareas de la 3 a la 5, de la
          // 5 a la 8 y asi sucesivamente
          std::cout << '\t' << process_hostname << ':' << process_number << '.'
            << omp_get_thread_num() << ": range [" << thread_start << ", " <<
            thread_finish << "[ size " << thread_size << std::endl;

          // Aqui dice la cantidad de tiempo que duro cada proceso
          const double elapsed = MPI_Wtime() - start_time;
          // Aqui imprime lo que duro cada proceso
          #pragma omp critical(can_print3)
            std::cout << process_hostname << ':' << process_number << '.'
            << omp_get_thread_num() << ": range [" << thread_start << ", " <<
            thread_finish << "[ size " << thread_size <<  " in " <<
            elapsed << "s" <<std::endl;
        }
      }
    }
    // Termina la ejecucion del MPI
    MPI_Finalize();
  }
  // Return del main
  return 0;
}

int calculate_start(int rank, int end, int workers, int begin) {
  // Formula sacada de la pagina de Jeisson sobre el mapeo de bloque
  const int range = end - begin;
  return begin + rank * (range / workers) + std::min(rank, range % workers);
}

int calculate_finish(int rank, int end, int workers, int begin) {
  // Formula sacada de la pagina de Jeisson sobre el mapeo de bloque
  return calculate_start(rank + 1, end, workers, begin);
}

