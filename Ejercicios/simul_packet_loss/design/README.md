### Network_simul_packet_loss
## Codigo del Ejercicio 24

Modifique el código de la simulación de red para introducir pérdida de paquetes. Reciba un parámetro más en línea de comandos que corresponde a la probabilidad de perder paquetes, como un porcentaje en **precisión flotante**. Implemente un hilo ensamblador que es uno de los destinos del repartidor, como se ve en el siguiente diseño.

Por cada paquete que este hilo ensamblador recibe, se genera un **número flotante pseudoaleatorio entre 0 y 100**. Si este número generado es menor que la probabilidad de pérdida de paquetes, el hilo descarta el paquete, de lo contrario, modifica al azar el destino del paquete, y lo pone de regreso en la cola entre el productor y el repartidor.

`Se puede observar que depende del numero que ingrese el usuario, uno de los productores que es conocido tambien como el "Assembler" va perdiendo los productos atrasando asi la ejecucion, no obstante, no afecta mucho ya que puede haber mas productores que van consumiendo de la cola de productores, en el siguiente ejercicio se vera como puede afectar ponerle esta condicion de perdida de paquetes a todos los consumers (MIENTRAS MAS GRANDE SEA EL ARGUMENTO DE PACKET LOSS MENOS POSIBILIDADES HAY DE PERDER EL PAQUETE, MIENTRAS MAS PEQUENO MAS POSIBILIDADES DE PERDERLO, SON COMO LAS POSIBILIDADES DE EXITO)`