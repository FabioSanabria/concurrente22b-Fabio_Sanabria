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
  // Aqui obtenemos la cantidad de hilos (Explicado en anteriores
  // ejemplos)
  int thread_count = omp_get_max_threads();
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }
  // Aqui se consigue la cantida de iteraciones
  // que se desean realizar
  int iteration_count = thread_count;
  if (argc >= 3) {
    iteration_count = atoi(argv[2]);
  }
  // Este es el numero que simboliza el tamano de
  // bloque y si el usuario lo coloca entonces en la
  // impresion debe de aparecer a la par del tipo de
  // mapeo
  int block_size = 0;
  if (argc >= 4) {
    // Aqui se agarra el argumento
    block_size = atoi(argv[3]);
  }
  // Vector que contiene los numeros de los
  // hilos mapeados
  std::vector<int> mapping(iteration_count);
  // Se crean los hilos y se inicia la parte
  // concurrente
  #pragma omp parallel num_threads(thread_count) \
    default(none) shared(iteration_count, mapping, block_size)
  {
    // Si el usuario ingreso un numero como tercer argumento
    if (block_size == 0) {
      // Aqui se elige el tipo de mapeo que se desea utilizar
      // en este caso se va a usar el mapeo estatico
      #pragma omp for schedule(static)
      // Inician las iteraciones para guardar los numeros de los hilos en
      // un vector
      for (int iteration = 0; iteration < iteration_count; ++iteration) {
        mapping[iteration] = omp_get_thread_num();
      }
      // Se utiliza un single para evitar que el print_mapping sea llamado
      // por mas de un hilo
      #pragma omp single
      // Se llama el metodo que imprime el tipo de mapeo estatico
      print_mapping("static    ", mapping);

      // Aqui se elige el tipo de mapeo que se desea utilizar
      // en este caso se va a usar el dinamico
      #pragma omp for schedule(dynamic)
      // Inician las iteraciones para guardar los numeros de los hilos en
      // un vector
      for (int iteration = 0; iteration < iteration_count; ++iteration) {
        mapping[iteration] = omp_get_thread_num();
      }
      // Se utiliza un single para evitar que el print_mapping sea llamado
      // por mas de un hilo
      #pragma omp single
      // Se llama el metodo que imprime el tipo de mapeo dinamico
      print_mapping("dynamic   ", mapping);

      // Aqui se elige el tipo de mapeo que se desea utilizar
      // en este caso se va a usar el guiado
      #pragma omp for schedule(guided)
      // Inician las iteraciones para guardar los numeros de los hilos en
      // un vector
      for (int iteration = 0; iteration < iteration_count; ++iteration) {
        mapping[iteration] = omp_get_thread_num();
      }
      // Se utiliza un single para evitar que el print_mapping sea llamado
      // por mas de un hilo
      #pragma omp single
      // Se llama el metodo que imprime el tipo de mapeo guiado
      print_mapping("guided    ", mapping);
    } else {
    // Practicamente toda la siguiente parte es lo mismo que lo de arriba
    // solamente que si el usuario ingreso el numero de bloque se va a
    // colocar una N que simboliza el tamano del bloque, entonces imprime
    // los mapeos correspondientes simplemente que con una N en frente
      #pragma omp for schedule(static, block_size)
      for (int iteration = 0; iteration < iteration_count; ++iteration) {
        mapping[iteration] = omp_get_thread_num();
      }
      // Se utiliza un single para evitar que el print_mapping sea llamado
      // por mas de un hilo
      #pragma omp single
      // Aqui se elige el tipo de mapeo que se desea utilizar
      // en este caso se va a usar el estatico
      print_mapping("static,N  ", mapping);

      #pragma omp for schedule(dynamic, block_size)
      for (int iteration = 0; iteration < iteration_count; ++iteration) {
        mapping[iteration] = omp_get_thread_num();
      }
      // Se utiliza un single para evitar que el print_mapping sea llamado
      // por mas de un hilo
      #pragma omp single
      // Aqui se elige el tipo de mapeo que se desea utilizar
      // en este caso se va a usar el dinamico
      print_mapping("dynamic,N ", mapping);

      #pragma omp for schedule(guided, block_size)
      for (int iteration = 0; iteration < iteration_count; ++iteration) {
        mapping[iteration] = omp_get_thread_num();
      }
      // Se utiliza un single para evitar que el print_mapping sea llamado
      // por mas de un hilo
      #pragma omp single
      // Aqui se elige el tipo de mapeo que se desea utilizar
      // en este caso se va a usar el guiado
      print_mapping("guided,N  ", mapping);
    }
  }
}

void print_mapping(const char* type, const std::vector<int>& mapping) {
  std::cout << type;
  for (size_t index = 0; index < mapping.size(); ++index) {
  // Imprime lo que tiene el vector dentro
    std::cout << mapping[index] << (index == mapping.size() - 1 ? '\n' : ' ');
  }
}
