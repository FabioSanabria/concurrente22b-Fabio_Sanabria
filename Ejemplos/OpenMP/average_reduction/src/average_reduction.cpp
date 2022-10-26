// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
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
  // Vector de valores que guarda los valores
  // para sacar el promedio de estos
  std::vector<double> values;

  // Valor que va a ser cambiado constantemente
  // ya que va a ser modificado por el cin para
  // guardarlo en el vector
  double value = 0.0;
  while (std::cin >> value) {
    values.push_back(value);
  }

  double sum = 0.0;
  // Zona en donde se utiliza OMP para, poder volver concurrente
  // la parte del calculo del promedio, los parametros shared,
  // default, etc ya fueron explicados en anteriores codigos
  // la directiva reduction realiza los procesos de manera directa
  // haciendo que el programa sume todos los valores contenidos
  // en values a sum, haciendo el codigo mas compacto y rapido
  #pragma omp parallel for num_threads(thread_count) schedule(runtime) \
    default(none) shared(values) reduction(+:sum)
  for (size_t index = 0; index < values.size(); ++index) {
    sum += values[index];
  }
  // Calcula el promedio, no obstante, habria problemas si el denominador
  // es 0 por lo que se revisa que este no sea 0 para no dividirlo
  const double average = values.size() ? sum / values.size() : 0.0;
  std::cout << average << std::endl;
}
