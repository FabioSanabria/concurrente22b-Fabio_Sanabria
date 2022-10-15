// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <iostream>

int main(int argc, char* argv[]) {
  // Obtiene la cantidad de hilos equivalente a
  // la cantidad de CPUs
  int thread_count = omp_get_max_threads();
  // El usuario coloca la cantidad de hilos que
  // desea
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }
  // Contador de las iteraciones realizadas por
  // cada hilo
  int iteration_count = thread_count;
  // Argumento enviado por el usuario para decir
  // la cantidad de iteraciones que van a realizar
  // dichos hilos
  if (argc >= 3) {
    iteration_count = atoi(argv[2]);
  }
  // Aqui empieza la parte concurrente del programa,
  // crea los hilos y los pone a iterar varias veces en
  // en for para imprimir las iteraciones, el numero
  // de hilo y el total de hilos en un cout

  // El default es utilizado para evitar que los hilos sean
  // enviados predeterminadad, es decir, sin que nosotros
  // sepamos si se enviaron de manera privada o compartida,
  // es por eso que a la par del default se pone el shared
  // para saber si los valores son enviados de una u otra manera
  // esto es usado asi para que tengamos mas facilidad a la hora
  // de buscar las condiciones de carrera, ademas, el for significa
  // que los hilos se van a dividir las tareas de la manera mas
  // equitativa posible
  #pragma omp parallel for num_threads(thread_count) \
    default(none) shared(iteration_count, std::cout)
    // Aqui va iterando detro de un ciclo
  for (int iteration = 0; iteration < iteration_count; ++iteration) {
    // Entra en la region critica para imprimir los hilos y las iteraciones
    #pragma omp critical(stdout)
    // Aqui se imprimen todos los que nos pidieron en el enunciado usando
    // 2 metodos de omp.h
    std::cout << omp_get_thread_num() << '/' << omp_get_num_threads()
      << ": iteration " << iteration << '/' << iteration_count << std::endl;
  }
}
