### Hello_order_semaphore_2

## Codigo del Ejercicio 14

Modifique su solución de **Ejemplo 7** para *inicializar todos los semáforos en 0*. Luego haga que el hilo principal *incremente el semáforo del hilo 0*. **¿Produce el mismo resultado que su solución anterior?**

`R/ Lo que sucede es practicamente lo mismo que el ejemplo 7, ya que el ejemplo 7 inicializa los valores con "!thread_number", esto significa que los valores que`
`que no son 0 son convertidos en 0 y el valor 0 es convertido en 1, lo que hace que el primer thread siempre tenga un 1 y los demas tengan un 0`
`lo diferente de este ejercicio es que en lugar de utilizar "!thread_number" se inicializo directamente los semaforos en 0 excepto el primero que se inicializo en 1`

**¿Qué pasa si el hilo principal incrementa el semáforo del hilo 0 después de hacer su saludo Hello from main thread?**

`Al colocar el incremento del semaforo del hilo 0 ocasiono varios bellos durmientes haciendo que el programa se quede corriendo para siempre debido a que`
`los demas hilos empezaron su ejecucion y se quedaron esperando a que el thread 0 les diera la indicacion para que ellos pudieran continuar ejecutando su` 
`parte, pero como el hilo 0 se inicializa con 1 despues del saludo del main thread el programa queda para siempre atrapado en el metodo greet de algun hilo`
`secundario que busca ser incrementado por un hilo 0 que nunca va a llegar`
