# Tema 20 OpenMP

## Ejemplo de hello.c

El siguiente es un **"Hola mundo"** en **OpenMP**. En adelante se usará C++, aunque OpenMP puede usarse perfectamente con C. Considera conceptos como los siguientes:

1. `Instrumentalización de código.`

2. `Directivas del preprocesador (pragma): son ignoradas por compiladores en los que no se ha habilitado la instrumentalización.`

3. `Región paralela (omp parallel): siempre implica la creación de un equipo de hilos`

4. `Barrera o join tras el bloque de instrucciones que conforman la región paralela.`

5. `En los Linux OpenMP usa Pthreads internamente. La instrumentalización traslada el código de la región paralela a una rutina, crea hilos con pthread_create(), y espera por ellos con pthread_join().`

6. `El hilo principal no puede ejecutar trabajo durante la región paralela, a diferencia de Pthreads.`
