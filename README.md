# Estudiante: Fabio Andrés Sanabria Valerín C07194 

Curso de Programación Paralela y Concurrente Grupo 3

Repositorio privado para subir las tareas, ejercicios, talleres, entre otras cosas...

# Glosario Ejercicio 2

## Programacion Serial

La programacion serial es cuando todos los procesos son realizados por un mismo hilo, eso quiere decir que si el proceso 1 se esta ejecutando, entonces el proceso 2 debe de esperar a que el hilo principal termine de ejecutar el proceso 1 para seguir con el proceso 2 y asi sucesivamente

## Programacion concurrente

Segun el profesor Jeisson Hidalgo, la definicion de programacion concurente es muy simple y es lo no serial, quiere decir que todos los calculos de un programa se realizan de manera concurrente o simultanea

## Programacion paralela

Es el uso simultáneo de múltiples recursos computacionales para resolver un problema computacional, divide el problema para que diferentes procesos/hilos se encarguen de una parte incrementando el desempeño y la velocidad

## Concurrencia de tareas

Se refiere al uso de la programacion concurrente para dividir diversas tareas a diferentes hilos para aumentar la velocidad de un programa


## Paralelismo de datos

Es un paradigma de la programación concurrente que consiste en subdividir el conjunto de datos de entrada a un programa, de manera que a cada procesador le corresponda un subconjunto de esos datos.

## Hilo de ejecución

El hilo es un flujo de control dentro de un programa y es el encargado de realizar todos los procesos que el programador le pide, si existen varios hilos en el programa entonces nos da la oportunidad de utilizar la concurrencia para incrementar la velocidad

## Indeterminismo

El indeterminismo en un programa sucede normalmente cuando nos adentramos en el mundo de la concurrencia y significa que no tenemos certeza de lo que va a pasar en nuestro programa, o sea que nuestro programa se vuelve impredecible ya que no se sabe el orden en que los hilos van a ejecutar sus procesos

## Memoria privada y compartida.

La memoria privada son los datos que tiene cada hilo, es decir, los datos unicos que tiene cada hilo como lo es el thread_number y la memoria compartida son los datos que tienen en comun los hilos o vaya la redudancia datos que deseamos que compartan dichos hilos y que si algun hilo lo llega a modificar entonces lo modifica para todos los hilos como puede ser el thread_Count

## Espera activa

Es una técnica donde un proceso repetidamente verifica una condición, tal como esperar una entrada de teclado o si el ingreso a una sección crítica está habilitado. Esto es una pesima practica de programacion ya que puede llevar el procesados al maximo pudiendo traer repercusiones negativas a nuestra computadora

## Condición de carrera

Es una situacion problematica  y ocurre cuando dos o mas hilos intentan realizar una operacion al mismo tiempo como puede ser la lectura, escritura o alguna otra accion que involucre **modificacion concurrente de memoria compartida**

## Control de concurrencia

Es un mecanismos utilizado para evitar el uso de esperas activas y evitar las condiciones de carrera, en la mayoria de los casos involucra la serializacion de fragmentos de codigo paralelo para no realizar una modificacion concurrente de memoria compartida

## Seguridad condicional

Consiste en hacer que cada hilo tenga su propia parte de memoria para evitar la modificacion de memoria compartida, la diferencia entre el control de concurrenccia es que no vuelve el una parte del codigo serial, si no que siempre va a modificar un fragmento de memoria que otros hilos jamas van a tocar

## Exclusión mutua

Se utiliza en la programación concurrente para evitar que entre más de un proceso a la vez en la sección crítica. La sección crítica es el fragmento de código donde puede modificarse un recurso compartido. La exclusion mutua es utilizada por el mecanismo de control de concurrencia **mutex**

## Semáforo

El semaforo es un mecanismo de control de concurrencia que indica el número de procesos que se encuentran ejecutando la sección crítica. Cuando toma el valor 0, significa que algún proceso está en su sección crítica, mientras que cuando toma el valor 1 significa que no hay ningún proceso ejecutándola. La seccion critica no tiene dueño.

## Barrera

Es un mecanismo de control de concurrencia que lo que hace es evitar que los hilos ejecuten un parte del codigo hasta que todos esten listos, es decir que si el hilo 0 es el unico que no ha terminado sus procesos entonces los demas hilos no pueden ejecutar una parte especifica del codigo hasta que el hilo 0 termine.

## Variable de condición

Es un mecanismo de control de concurrencia muy parecido a la barrera, no obstante este depende de una condicion que si no se cumple entonces el hilo se queda en espera hasta que llegue otro hilo y cumpla la condicion y haga un broadcast para liberar a los demas hilos.

## Candado de lectura y escritura

Es un mecanismo para bloqueo de lectura y escritura, permite que varios lectores accedan al recurso, pero solo permite un lector en un momento dado.

## Descomposición

 Se trata de identificar las tareas o unidades de trabajo que se pueden realizar de forma independiente, y por lo tanto, de forma paralela. Una vez definidas, las tareas se consideran unidades indivisibles de computación. Es seguido por el mapeo, que consiste en asignar esas unidades de trabajo a los ejecutantes (procesos o hilos de ejecución)

 ## Mapeo

 Es la asignacion de tareas o trabajos entre diversos hilos y existen 2 tipos de mapeos:  En el mapeo estático la asignación de las unidades de trabajo se conoce de antemano, es decir, antes de iniciar el trabajo, sólo depende de conocer el número total de unidades de trabajo y trabajadores. El mapeo por bloque y cíclico son dos formas de mapeo estático. En el mapeo dinámico la asignación ocurre durante la realización del trabajo, es decir, los trabajadores se asignan las unidades de trabajo conforme ellos terminan las previas. Aunque normalmente consigue mejores distribuciones, es más costoso en recursos que los mapeos estáticos, dado que requiere control de concurrencia y manipulación de estructuras de datos dinámicas.

 ## Incremento de velocidad

 Normalmente cuando se tiene un programa muy ineficiente se le hacen varios ajustes para volverlo mas rapido, en este caso una de las maneras para hacer un programa mas eficiente es mediante la concurrencia y siempre hay un incremento de velocidad que se calcula de la siguiente forma:

 speed up = tiempoSerial/tiempoParalelo

## Comunicacion punto a punto

En el mundo de la distribucion hay una desventaja muy grande y es la falta de comunicacion entre procesos, en el curso se utilizó MPI como tecnología distributiva y a diferencia de pthreads la comunicacion entre procesos es muy compleja ya que se debe de tener seguimiento a los send y recieve y ademas se debe de tener cuidado.

## Comunicación colectiva entre procesos.

Una de las manera mas efectivas para lograr una comunicacion entre procesos de manera mas sencilla es utilizando comunicacion colectiva, hace la misma funcion que la comunicacion punto a punto solo que esta le manda el mismo mensaje a todos los procesos al mismo tiempo mediante un broadcast hacaiendo el ccodigo mas compacto y eficiente.

## Reducción

La reducción es utilizada en MPI y OMP para reducir un valor de un grupo de procesos en un único proceso raíz. Las funciones de reduccion realizan una operación de reducción global (como puede ser calcular el máximo, la suma, hacer un AND lógico, etc) sobre cada uno de los miembros del grupo. La operación de reducción puede ser tanto una predefinida de la lista de operaciones como una operación definida por el usuario.
Se puede notar que únicamente se especifica una vez el número de datos y el tipo de datos, esto es porque el número de datos es el mismo tanto en la emisión como en la recepción, al igual que el tipo.