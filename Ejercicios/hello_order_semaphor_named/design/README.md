### Semaforos

## Codigo del tema 7 y 8

Al igual que **Ejemplo 5**, haga que los threads saluden siempre en orden. Es decir, si se crean `w` threads, la salida sea siempre en orden

`Hello from thread 0 of w`
`Hello from thread 1 of w`
`Hello from thread 2 of w`
`...`
`Hello from thread w of w`

Utilice una colección de semáforos como mecanismo de sincronización (control de concurrencia).

*** Se utilizo un arreglo de semaforos para lograr el cometido de ordenar todos los numeros en orden (No hay que preocuparse por el error del relog) ***