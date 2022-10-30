// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

// Libreria que contiene todo lo
// relacionado con lo de MPI
// (MPI_INIT, COMM, ETC)
#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[]) {
  // Inicializa el ambiente de ejecucion de MPI
  // para poder utilizar todas sus funciones
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    // Se coloca en -1 para ver si a la hora de
    // imprimir hay algun tipo de error de salida
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
    // en este caso se llama Fasv-VirtualBox
    MPI_Get_processor_name(process_hostname, &hostname_length);
    // Imprime el mensaje que necesitamos
    std::cout << "Hello from main thread of process " << process_number
      << " of " << process_count << " on " << process_hostname << std::endl;
    // Aqui terminan todos los procesos de MPI
    MPI_Finalize();
  } else {
    // Por si da algun error al inicializar MPI
    std::cerr << "error: could not init MPI" << std::endl;
  }
  // Return del main
  return 0;
}
