### Seguridad Condicional

## Codigo del tema 8

Al igual que **Ejemplo 7**, haga que los threads saluden siempre en orden. Es decir, si se crean `w` threads, la salida sea siempre en orden

`Hello from thread 0 of w`
`Hello from thread 1 of w`
`Hello from thread 2 of w`
`...`
`Hello from thread w of w`

Evite (si es posible) el control de concurrencia, es decir, trate de usar seguridad condicional (conditionally safe).

*** El sprintf es marcado como si fuera un error en el linter, como el programa funciona gracias a eso se va a dejar tal y como lo hizo el profesor Jeisson ***