// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

// Already seen
#include <mpi.h>
// Libreria que se utiliza para
// utilizar la concurrencia declarativa
// gracias a esta libreria, se puede
// resumir una enorme cantidad de lineas
// de codigo usando pthreads
#include <omp.h>
#include <iostream>

int main(int argc, char* argv[]) {
  // Inicializa el ambiente de ejecucion de MPI
  // para poder utilizar todas sus funciones
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
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
  // Imprime el mensaje que necesitamos
    std::cout << "Hello from main thread of process " << process_number
      << " of " << process_count << " on " << process_hostname << std::endl;
    // Obtenemos la cantidad maxima de hilos que ofrece
    // el sistema
    int thread_count = omp_get_max_threads();
    if (argc == 2) {
    // De lo contrario, si el usuario pide la cantidad
    // de hilos mediante argumento en la linea de comandos,
    // entonces la guardamos en thread_count  
      thread_count = atoi(argv[1]);
    }
    // Usamos directiva de OMP para iniciar con la concurrencia
    #pragma omp parallel num_threads(thread_count) default(none) \
      shared(std::cout, process_number, process_count, process_hostname)
    {  // NOLINT(whitespace/braces)
    // Se crea una zona critica/ mutex para poder
    // controlar la entrada de los hilos
      #pragma omp critical(stdout)
      std::cout << "\tHello from thread " << omp_get_thread_num() << " of "
        << omp_get_num_threads() << " of process " << process_number
        << " of " << process_count << " on " << process_hostname << std::endl;
    }
    // Aqui terminan todos los procesos de MPI
    MPI_Finalize();
  } else {
    // Por si da algun error al inicializar MPI    
    std::cerr << "error: could not init MPI" << std::endl;
  }
  // Return del main  
  return 0;
}
