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
  // Parte paralela del programa, hace lo mismo que el parallel_for
  // no obstante tiene varios cambios como los stage, la idea es que
  // se ejecuten los stage de manera separada y que se vayan
  // imprimiendo
  #pragma omp parallel num_threads(thread_count) \
    default(none) shared(iteration_count, std::cout)
  {
    // Al inicio de cada ciclo se debe de poner un #pragma omp for
    // para que asi los hilos logren dividirse las tareas de manera
    // eficiente
    #pragma omp for
  // Aqui va iterando detro de un ciclo  
    for (int iteration = 0; iteration < iteration_count; ++iteration) {
      #pragma omp critical(stdout)
    // Aqui se imprimen todos los que nos pidieron en el enunciado usando
    // 2 metodos de omp.h, este es el stage 1 que debe de ser independiente
    // a los demas, independiente significa que tenga un for propio, entonces
    // se ejecuta el stage 1 y luego pasa al siguiente stage en donde se
    // va a ejecutar lo mismo solo que sin necesidad de crear mas hilos
      std::cout << "stage 1: " << omp_get_thread_num() << '/'
        << omp_get_num_threads() << ": iteration " << iteration << '/'
        << iteration_count << std::endl;
    }
    // Aqui hay una barrera implicita ya que los omp for las crean

    // Luego tenemos el sigle, esto es usado para lograr
    // imprimir un espacio entre los stage, es necesario
    // ya que cada hilo puede entrar y hacer un salto de linea
    // innecesario, la idea es que solo sea 1
    #pragma omp single
    // Aqui hay otra barrera implicita por el omp single
    std::cout << /*omp_get_thread_num() <<*/ std::endl;
    // #pragma omp barrier

    // Al inicio de cada ciclo se debe de poner un #pragma omp for
    // para que asi los hilos logren dividirse las tareas de manera
    // eficiente
    #pragma omp for
    // Aqui va iterando detro de un ciclo 
    for (int iteration = 0; iteration < iteration_count; ++iteration) {
      // Mutex que protege la region critica
      #pragma omp critical(stdout)
    // Aqui se imprimen todos los que nos pidieron en el enunciado usando
    // 2 metodos de omp.h, este es el stage 2 que debe de ser independiente
    // a los demas, independiente significa que tenga un for propio, entonces
    // se ejecuta el stage 2 y luego pasa al siguiente stage en donde se
    // va a ejecutar lo mismo solo que sin necesidad de crear mas hilos      
      std::cout << "stage 2: " << omp_get_thread_num() << '/'
        << omp_get_num_threads() << ": iteration " << iteration << '/'
        << iteration_count << std::endl;
    }

    // Otro single para hacer solo un espacio
    #pragma omp single
    std::cout << /*omp_get_thread_num() <<*/ std::endl;
    // #pragma omp barrier

    #pragma omp for
    // Aqui va iterando detro de un ciclo 
    for (int iteration = 0; iteration < iteration_count; ++iteration) {
      // Mutex que protege la region critica
      #pragma omp critical(stdout)
      // Este es el ultimo stage de este programa, realiza lo mismo que se
      // exlico anteriormente solo que ya no hay single porque no queremos
      // un salto de linea extra
      std::cout << "stage 3: " << omp_get_thread_num() << '/'
        << omp_get_num_threads() << ": iteration " << iteration << '/'
        << iteration_count << std::endl;
    }
  }
}
