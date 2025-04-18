# Ejercicio 31 Chess_vous

## Pseudocodigo del ejercicio chess_vous

En un torneo de ajedrez los participantes efectúan el siguiente itinerario tradicional. Los jugadores y árbitros se identifican para ingresar al centro deportivo **(enter_room)**. Cada jugador ubica la mesa con su tablero y se sienta en ella. Una vez que ambos jugadores están presentes, avisan al árbitro. El árbitro establece el tiempo en el reloj **(set_clock)** del tablero y con ello ambos jugadores inician la partida **(play_chess)**.

El problema se considera resuelto si el árbitro establece el reloj sólo hasta que los dos jugadores hayan ingresado al centro deportivo **(enter_room)** y los jugadores juegan **(play_chess)** hasta que el árbitro haya establecido el reloj **(set_clock)**. Considere el siguiente código inicial que trata de representar el escenario descrito.

    procedure main()
    shared player1_ready := semaphore(0)
    shared player2_ready := semaphore(0)
    shared players_ready := semaphore(0)
    shared clock_ready := semaphore(0)

    create_thread(player1)
    create_thread(player2)
    create_thread(referee)
    end procedure

    procedure player1()
    enter_room()
    play_chess()
    end procedure

    procedure player2()
    enter_room()
    play_chess()
    end procedure

    procedure referee()
    enter_room()
    set_clock()
    end procedure

**Agregue control de concurrencia al código inicial para considerando todas las posibles rutas de ejecución, el problema siempre esté resuelto.**
