// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <iostream>
#include <vector>

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

  // Variable que va a almacenar el promedio de los
  // numeros ingresados por el usuario
  double total_sum = 0.0;

  // Zona en donde se utiliza OMP para, poder volver concurrente
  // la parte del calculo del promedio, los parametros shared,
  // default, etc ya fueron explicados en anteriores codigos
  #pragma omp parallel num_threads(thread_count) \
    default(none) shared(values, total_sum)
  {
    double my_partial_sum = 0.0;

  // Directiva del open mp que se encarga de elegir un
  // tipo de mapeo a la hora de correr el programa, utiliza
  // un mapeo que se encuentra en una variable que puede
  // ser modificada antes de ejecutar el programa
    #pragma omp for schedule(runtime)
    for (size_t index = 0; index < values.size(); ++index) {
      my_partial_sum += values[index];
    }
    // Se pone el mutex aqui para incrementar la velocidad
    // ya que si se coloca la zona critica arriba, todos los
    // hilos tendria que ejecutarla una enorme cantidad de
    // veces, en cambio si se coloca aqui, disminuye la cantidad
    // de entradas
    #pragma omp critical(can_add)
    total_sum += my_partial_sum;
  }
  // Calcula el promedio, no obstante, habria problemas si el denominador
  // es 0 por lo que se revisa que este no sea 0 para no dividirlo
  const double average = values.size() ? total_sum / values.size() : 0.0;
  // Imprime el promedio
  std::cout << average << std::endl;
}
