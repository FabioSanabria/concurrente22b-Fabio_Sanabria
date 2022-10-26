// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <iostream>
#include <vector>
/**
 * @brief Subrutina que lo que hace es imprimir los tipos de mapeos que estan
 * contenidos en el vector
 * @param type es el block size, o sea,el numero que acompana al mapeo
 * @param mapping Es el vector con las posiciones de los hilos con su
 * respectivo mapeo
 * @return NULL
 */
void print_mapping(const char* type, const std::vector<int>& mapping);

int main(int argc, char* argv[]) {
    // Optiene la cantidad maxima de hilos
  // si el usuario no ingresa ningun numero
  int thread_count = omp_get_max_threads();
  if (argc >= 2) {
    // Agarra el segundo argumento
    // que es el de los hilos, si
    // no envia nada entonces se
    // toman la cantidad de CPUs
    // que tenga la maquina
    thread_count = atoi(argv[1]);
  }

    // Aqui se consigue la cantidad de iteraciones
  // que se desean realizar
  int iteration_count = thread_count;
  if (argc >= 3) {
    iteration_count = atoi(argv[2]);
  }
  // Vector que contiene los numeros de los
  // hilos mapeados
  std::vector<int> mapping(iteration_count);
  // En esta parte se utiliza el runtime schedule, que lo que hace
  // es utilizar cualquiera de los mapeos disponibles, se debe de
  // modificar una variable en especifico antes de empezar la
  // ejecucion, si no se toca esa variable pues agarra el mapeo por
  // default que es el estatico por bloque
  #pragma omp parallel for num_threads(thread_count) schedule(runtime) \
    default(none) shared(iteration_count, mapping)
  for (int iteration = 0; iteration < iteration_count; ++iteration) {
    mapping[iteration] = omp_get_thread_num();
  }
  // Se utiliza un single para evitar que el print_mapping sea llamado
  // por mas de un hilo
  #pragma omp single
  // Imprime el mapeo
  print_mapping("runtime   ", mapping);
}

void print_mapping(const char* type, const std::vector<int>& mapping) {
  std::cout << type;
  for (size_t index = 0; index < mapping.size(); ++index) {
    std::cout << mapping[index] << (index == mapping.size() - 1 ? '\n' : ' ');
  }
}
