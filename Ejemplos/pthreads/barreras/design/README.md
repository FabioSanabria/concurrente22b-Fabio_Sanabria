# Tema 19 Barreras

## Ejemplos subidos por el profe Jeisson en los videos

### Sem_barrier

Generalice su solución a la actividad **rendezvous** *(Ejemplo 15)* para asegurar que una cantidad arbitraria de hilos no continúen su ejecución hasta que todos hayan alcanzado (ejecutado) el punto de encuentro. Es decir, si se crean w hilos, los primeros `w - 1` hilos que lleguen al punto de encuentro deberían esperar hasta que el hilo en la `posición w` llegue al punto de encuentro. En tal momento todos los `w` threads podrán continuar ejecutándose concurrentemente. A este patrón se le conoce como barrera (barrier). Implemente una `barrera` con semáforos en el siguiente pseudocódigo para que la instrucción Statement B se ejecute sólo hasta que todos los hilos hayan ejecutado la instrucción Statement A.

### Sem_barrier_reusable

Haga que su solución a la actividad `Ejemplo 19` sea `reusable`. Es decir, que la barrera quede lista para volver a usarse, por ejemplo, dentro de un ciclo. Debe asegurarse de que el contador quede en `0`, y los `threads` que salen de la barrera no se combinen con otros que aún están en ella.

**Sugerencia**: Puede **reemplazar la barrera por dos torniquetes**. Un torniquete (turnstile) o "trompo" es un dispositivo de control de acceso que impide o da paso a una secuencia de personas de acuerdo a ciertas condiciones. Se usa típicamente en instalaciones deportivas, transporte público, acceso a edificios, parques de diversiones, y otros.

Un torniquete es un semáforo que detiene a todos los hilos que lleguen a él. Pero apenas un hilo logre pasarlo, inmediatamente habilita el paso al siguiente. Puede pensarse como una manera más elegante de que el último hilo que llega a la barrera haga un ciclo de incrementos por todos los demás.

### sem_barrier_include

Es practicamente los mismo que el reusable solo que se convierte la barrera vista en el **sem_barrier_reusable** en codigo que se puede reusar mejor

### Relay_race

Simule una **carrera de relevos**, donde los competidores son equipos de threads. Cada equipo consta de **dos corredores**. Un corredor tomará la estafeta (en inglés, baton) y se posicionará en la línea de salida. El otro corredor esperará en la mitad de la pista a que el primer corredor llegue y le transfiera la estafeta. En tal momento, el segundo corredor seguirá a toda velocidad hasta alcanzar la meta. **Ganan únicamente los tres primeros equipos** en llegar con la estafeta a la meta, quienes son premiados con **oro, plata, y bronce**.

Su simulación recibirá tres parámetros: `la cantidad de equipos, la duración en milisegundos que tarda el primer corredor atravesando la etapa 1 de la carrera (entre la salida y la mitdad de la pista), y la duración en milisegundos del segundo corredor atravesando la etapa 2 de la carrera (de la mitad de la pista hasta la meta)`. Suponga que los corredores de una etapa tardan exactamente esta duración y no un valor generado al azar. Ejemplo de ejecución:

`./relay_race 10 300 700`
`Place 1: team 2`
`Place 2: team 4`
`Place 3: team 1`
`Simulation time: 1.006079000s`

En la salida anterior se reportan sólo los tres primeros equipos en llegar. Recuerde verificar la corrección de su solución (ej.: **fugas de memoria, condiciones de carrera**). Puede usar el siguiente código como punto de partida.
