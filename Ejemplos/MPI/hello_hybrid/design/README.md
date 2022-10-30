# Ejemplo 44. Distribución híbrida [hello_hybrid]

## Codigo del ejemplo de hello_hybrid usando MPI

Los procesos (distribución) no ejecutan código, sino los hilos (concurrencia) dentro de sí, y los hilos no pueden existir si no es dentro de un proceso. Dado que se puede crear una cantidad arbitraria de procesos y en cada uno de ellos, una cantidad arbitraria de hilos, se tiene un modelo híbrido de ejecución distribuida-concurrente que aproveche las ventajas de cada paradigma.

El ejemplo de **"Hola mundo"** lanza un **único hilo principal** (main thread) en cada proceso ejecutado. Modifique el programa para que cada proceso lance tantos hilos secundarios (secundary thread) como CPUs hay disponibles en la máquina donde se ejecuta. Cada hilo secundario debe imprimir en la salida estándar una línea antecedida por un tabulador en la que salude al usuario. **Por ejemplo si se lanzan dos procesos y cada uno crea tres hilos secundarios, se podría obtener una salida como la siguiente:**

    Hello from main thread of process 0 of 2 on hostname1
        Hello from thread 0 of 3 of process 0 on hostname1
        Hello from thread 2 of 3 of process 0 on hostname1
        Hello from thread 1 of 3 of process 0 on hostname1
    Hello from main thread of process 1 of 2 on hostname2
        Hello from thread 1 of 3 of process 1 on hostname2
        Hello from thread 0 of 3 of process 1 on hostname2
        Hello from thread 2 of 3 of process 1 on hostname2

Dado que existe indeterminismo por la salida estándar, la salida que obtenga puede ser muy distinta a la anterior. Sugerencia: Puede usar OpenMP junto con MPI para lograr el modelo de concurrencia híbrida solicitada en esta actividad.
