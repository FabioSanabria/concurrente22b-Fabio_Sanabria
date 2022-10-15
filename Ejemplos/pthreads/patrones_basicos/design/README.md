# Patrones Basicos (Rendezvous)

## PseudoCodigo documentado

**Rendezvous1.pseudo:** Usa dos semáforos para asegurar que dos threads han llegado a cierto punto. En esta solución, los dos threads avisan apenas hayan terminado de ejecutar las instrucciones a1 o b1. En esta solución el nombre del semáforo indica el evento ocurrido, como a1_ready, y es lo que se prefiere. Otro nombre válido es indicar el manejador del evento o acción que debe realizarse cuando la señal ocurre, como can_run_b2.

**Rendezvous2a.pseudo y Rendezvous2a.pseudo:** En esta versión un thread primero espera y el otro avisa. Es una solución correcta aunque ligeramente menos eficiente que la versión 1. Hay dos variantes equivalentes: a) El hilo a primero avisa y luego espera y el hilo b primero espera y luego avisa. b) El hilo a primero espera y luego avisa y el hilo b primero avisa y luego espera.

**Rendezvous3.pseudo:** En esta versión ambos threads esperan a que el otro haya terminado de ejecutar su instrucción. No es una solución al problema porque genera un bloqueo mutuo `(deadlock)`.

**sem_mutex.pseudo:** Un semáforo inicializado en 1 y que nunca supera este valor, es un semáforo binario. Es equivalente a un `mutex` excepto que no tiene dueño `(ownership)`. Cuando el semáforo tiene el valor 1 indica que el mutex está disponible, y el valor 0 que está bloqueado. Se diferencia en que un `mutex` nunca supera el valor 1, mientras que un semáforo técnicamente puede hacerlo, y de que un `mutex` sólo puede ser incrementado por el mismo thread que lo decrementó.

**sem_mutex_sym.pseudo:** El semáforo usado adecuadamente obliga a una serialización de los hilos.

**MUltiplex:** Simplemente se inicializa el semáforo con el límite superior de threads permitidos concurrentemente. Si el semáforo llega a alcanzar el valor 0, indica que se agotó la capacidad concurrente y los hilos subsecuentes tendrán que esperar. Cuando un hilo incrementa el semáforo es porque "sale del salón" y deja espacio para que otro ingrese. ¿Cuál será el valor final del semáforo cuando todos los hilos hayan pasado por la sección crítica? R/ **Va a tener el mismo valor con el que se inicializo ya que todos los patinadores al final le hacen signal al semaforo**