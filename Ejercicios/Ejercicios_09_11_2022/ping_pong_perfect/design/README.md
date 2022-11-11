# Ejercicio [ping_pong_perfect]

## Codigo del ejercicio ping_pong_perfect

Un ejercicio clásico de paso de mensajes es simular un juego de tenis de mesa (ping-pong) entre dos procesos. Uno lanza la bola al otro, quien la recibe y la regresa al primero, y así sucesivamente. Los dos jugadores son incansables y nunca pierden un servicio. Haga que su programa simule este comportamiento. Si su programa es invocado con una cantidad distinta a dos jugadores, debe reportar un mensaje de error y finalizar.

**Para compilar el programa se utiliza: [mpiexec -np 2 ./bin/ping_pong_perfect], en la salida va a imprimir la cantidad de puntos que han logrado hacer los 2 procesos, no necesita ningun otro argumento ya que los 2 jugadores son perfectos**