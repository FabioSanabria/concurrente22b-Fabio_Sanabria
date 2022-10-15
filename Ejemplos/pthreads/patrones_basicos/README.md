# Patrones Basicos (Rendezvous)

## PseudoCodigo documentado

**Dentro de la carpeta diseño se encuentran todos los diseños de pseudocodigo de `rendezvous`, `sem_mutex` y `multiplex`, ademas de un `README.md` que explica por encima que es lo que hace cada cosa**

**Rendezvous:** Modifique los códigos de los hilos para que la instrucción `a1` y `b1` se ejecuten siempre antes que las instrucciones `a2` y `b2`. Este problema tiene el nombre `francés rendezvous` que significa encuentro en un `punto de ejecución`, y ninguno de los dos hilos pueden continuar su ejecución hasta que el otro haya llegado.

**Sem_mutex:** Se debe de agregar `semáforos` al `pseudocódigo` que nos dio el profesor Jeisson para `forzar` a que los incrementos en los hilos se hagan con `exclusión mutua`.

**Sem_mutex_sym:** Note que en la actividad anterior ambos threads ejecutaban el código en subrutinas distintas, a lo que se le llama una `solución asimétrica` (separación de asuntos o concurrencia de tareas). Sin embargo, el código de las subrutinas era el mismo y por tanto podría convertirse en una `solución simétrica`. En una solución simétrica los hilos ejecutan el mismo código, es decir, la misma subrutina.

Las `soluciones simétricas` son a menudo más fáciles de generalizar. Agregue semáforos al pseudocódigo siguiente para forzar a que los incrementos hechos por una cantidad arbitraria de hilos sean con `exclusión mutua`.

**Multiplex:** Modifique el pseudocódigo dado para que imponga una cota o límite superior de n hilos ejecutando una sección de `concurrencia acotada`. A este patrón se le llama `multiplex` y es útil para problemas donde se tienen distintos entes trabajando al mismo tiempo pero con máximo de capacidad.

Por ejemplo, la cantidad de cajeros atendiendo en las ventanillas de un banco o de `clientes patinando en la pista del salón de patines`. En este último caso, si la capacidad de la pista se agota, algunos clientes tendrán que esperar afuera, y apenas un cliente sale de la pista, otro podrá ingresar de inmediato.
