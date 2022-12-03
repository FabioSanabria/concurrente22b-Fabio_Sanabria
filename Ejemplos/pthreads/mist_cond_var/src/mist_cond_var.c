#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
  @brief Struct de la clase mist (mistery), tecnicamente
  es un registro compartido
**/
typedef struct {
  /// Contador de cada hilo, es como un indice que toma el trabajo
  /// de thread_number
  unsigned int counter;
  /// Cantidad maxima de mist que se van a usar
  /// (thread_count)
  unsigned int max;
  /// MUtex para controlar la ejecucion de varios hilos
  pthread_mutex_t mutex;
  /// Este es un nuevo mecanismo de concurrencia,
  /// es un dispositivo de sincronización que permite que los subprocesos
  /// suspendan la ejecución y renuncien a los procesadores hasta que se satisfaga
  /// algún predicado sobre los datos compartidos
  pthread_cond_t cond_var;
} mist_t;

/// @brief Inicializa el mist con el max que le
/// pasa el usuario e inicializa los mecanismos de
/// control de concurrencia
/// @param mist Objeto de tipo mist
/// @param max Cantidad total de mist
void mist_init(mist_t* mist, unsigned max) {
  // Inicializa el contador en 0
  mist->counter = 0;
  // Inicializa el max con el valor enviado por el usuario
  mist->max = max;
  // Inicializa el mutex
  pthread_mutex_init(&mist->mutex, NULL);
  // Inicializa la variable condicion
  pthread_cond_init(&mist->cond_var, NULL);
}

/// @brief Destructor de la clase mist que libera
/// toda la memoria usada
/// @param mist Objeto a destruir
void mist_destroy(mist_t* mist) {
  // Coloca el contador en 0
  mist->counter = 0;
  // Elimina el mutex
  pthread_mutex_destroy(&mist->mutex);
  // Elimina el cond
  pthread_cond_destroy(&mist->cond_var);
}

/// @brief Es un metodo misterio que lo que hace es simular
/// una barrera entre los hilos que van llegando, en este caso
/// van a llegar 3 hilos
/// @param mist Objeto de tipo mist
void mistery(mist_t* mist) {
  // mutex que hace que los hilos se detengan hasta que el hilo
  // que se encuentra dentro de la zona critica termine su trabajo
  pthread_mutex_lock(&mist->mutex);
  // Incrementa el contador para el siguiente hilo
  ++mist->counter;
  // Si el contador es menor entonces todavia hay hilos de lo contrario
  // este es el ultimo hilo
  if (mist->counter < mist->max) {
    // Preferred: while ( pthread_cond_wait(...) != 0 ) /* empty */;
    // Aqui va a esperar hasta que algun hilo madne la condicion, en este
    // caso va a ser un broadcast
    pthread_cond_wait(&mist->cond_var, &mist->mutex);
  } else {
    // Contador es 0
    mist->counter = 0;
    // Manda una signal a todos los hilos para que ya puedan pasar
    pthread_cond_broadcast(&mist->cond_var);
  }
  // Desbloquea el mutex
  pthread_mutex_unlock(&mist->mutex);
}
/// Objeto mist global
static mist_t mist;
/// @brief Metodo que llaman los hilos para realizar la simulacion
/// de la barrera
/// @param data Indice de cada hilo
/// @return NULL
void* run(void* data) {
  // Imprime antes del misterio
  fprintf(stderr, "%zu: before mist()\n", (size_t)data);
  // Duerme dependiendo del indice
  sleep((unsigned)(size_t)data);
  // Entra al metodo misterioso
  mistery(&mist);
  // Imprime despues del misterio
  fprintf(stderr, "%zu: after mist()\n", (size_t)data);
  return NULL;
}

int main() {
  // Llama al metodo para inicializar
  mist_init(&mist, 3);
  // Inicializa a los hilos
  pthread_t* workers = malloc(3 * sizeof(pthread_t));
  for (size_t index = 0; index < 3; ++index) {
    // Crea los hilos y llama al proceso run
    pthread_create(&workers[index], NULL, run, (void*)index);
  }
  // Le hace join a todos los hilos para evitar fugas
  for (size_t index = 0; index < 3; ++index) {
    pthread_join(workers[index], NULL);
  }
  // libera el mist object
  mist_destroy(&mist);
  return 0;
}
