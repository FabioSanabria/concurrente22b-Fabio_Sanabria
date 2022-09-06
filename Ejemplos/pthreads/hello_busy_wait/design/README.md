### Hello_order_busywait

## Codigo del tema 6 y poco del 7

Haga que los threads saluden siempre en orden. Es decir, si se crean w threads, la salida sea siempre en orden

`Hello from thread 0 of w`
`Hello from thread 1 of w`
`Hello from thread 2 of w`
...
`Hello from thread w of w`

Utilice espera activa como mecanismo de sincronización (control de concurrencia).

Ejecute varias veces su solución de hello_order_busywait con la cantidad máxima de threads que el su sistema operativo le permite. ¿Es consistente la duración de su solución con espera activa? ***R/ Es eficaz pero no eficiente, casi me explota la computadora***

Describa el comportamiento del programa y explique la razón de este comportamiento en el archivo readme.md dentro de su carpeta hello_order_busywait. Indique en qué condiciones sería apremiante utilizar espera activa, o en caso de no haberlas, sugiera soluciones alternativas.
**R/ Como pregunta tantas veces en el ciclo no deja descansar al procesador por lo que constantemente esta al 100% de su capacidad lo que hace que puede tumbar todo el **sistema operativo y cerrar todo, no es recomendado utilizar este tipo de metodos pero segun nos ha comunicado el profesor, la espera activa es utilizada en `MPI`, una alternativa es usar los famosos mutex y semaforos para que puedan estar en las colas de espera**
