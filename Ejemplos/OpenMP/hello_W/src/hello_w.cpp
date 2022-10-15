// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>  // Libreria de OMP, es .h ya esta en C
#include <iostream>

int main(int argc, char* argv[]) {
  // En esta parte lo que hace es llamar un metodo
  // de omp.h que lo que hace es agarrar la maxima cantidad
  // de hilos dependiendo de la cantidad de CPUS que tiene
  // la computadora
  int thread_count = omp_get_max_threads();
  // Si el numero de argumentos es 2
  if (argc == 2) {
    // Entra y agarra la cantidad de hilos que quiere el
    // usuario
    thread_count = atoi(argv[1]);
  }
  // Este pragma lo que hace es llamar a un metodo preprogramado
  // que toma la cantidad de hilos que desesa el usuario y los
  // manda como parametro para que OMP los cree de una vez
  #pragma omp parallel num_threads(thread_count)
  {
    // Mutex de Omp previamente explicado en el hello.c
    #pragma omp critical(stdout)
    // Imprime el hola de cada uno de los hilos junto con 2 metodos
    // que retornan un numero entero, omp_get_thread_num retorna el
    // numero de hilo que esta ejecutando el metodo y omp_get_num_threads
    // nos manda la cantidad total de hilos
    std::cout << "Hello from secondary thread " << omp_get_thread_num()
      << " of " << omp_get_num_threads() << std::endl;
  }
}
