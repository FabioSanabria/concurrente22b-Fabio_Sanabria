# Ejercicio MPI_Wrapper

## Codigo del ejercicio 54 MPI_Wrapper

**Escriba una clase reutilizable Mpi que encapsule funcionalidad repetitiva del estándar MPI y provea un buen manejo de errores. Dado que usará plantillas, su clase estará en un archivo Mpi.hpp. En ejercicios posteriores usted incrementará y reutilizará esta clase**.

La clase debe tener al menos los atributos indicados en el pseudocódigo propuesto para ejecución distribuida (Pseudocódigo de paso de mensajes), esto es: número de proceso (int), cantidad de procesos en el equipo (int), y nombre de la máquina donde corre el proceso (std::string).

Provea un constructor que reciba referencias a los argumentos de línea de comandos, inicialice el ambiente de ejecución de MPI, y llame funciones de MPI para inicializar los atributos de la clase. Lance una instancia de std::runtime_error en el constructor si no puede inicializar el ambiente de MPI u obtener el valor para un atributo. Provea un mensaje descriptivo en cada excepción. Provea un destructor que termine la ejecución del ambiente MPI, pero no lance excepciones en caso de fallo.

Para cada atributo de la clase provea un método get en línea **(inline)**, **pero no provea métodos set**. Provea además un método **rank()** para obtener el el número de proceso, y un método **size()** para obtener la cantidad de procesos en el equipo.

Documente la clase, atributos, y métodos con Doxygen. Agregue a la carpeta un archivo main.cpp con el siguiente código que debería producir el mismo resultado que el **Ejemplo 43**. Pruebe que al ejecutarlo con varios procesos, estos saluden de forma indeterminística.

    int main(int argc, char* argv[]) {
    try {
        Mpi mpi(argc, argv);
        std::cout << "Hello from main thread of process " << mpi.getProcessNumber()
        << " of " << mpi.getProcessCount() << " on " << mpi.getHostname()
        << std::endl;
    } catch (const std::exception& error) {
        std::cerr << "error: " << error.what() << std::endl;
    }
    }

**Al probar varias veces el programas si se puede llegar a concluir que funciona igual al del ejemplo 43 y que hay indeterminismo**

# Ejercicio MPI_Error

## Codigo del ejercicio 55 MPI_Error

Implemente una clase para transportar potenciales errores que puedan surgir al usar la tecnología MPI y que necesitará en ejercicios siguientes.

Cree un archivo MpiError.hpp dentro de la carpeta **mpi_wrapper/**. Cree una clase MpiError que herede de **std::runtime_error**. Declare los tres constructores que están en el siguiente fragmento de código. Necesitará hacer un forward declaration de la clase Mpi.

**explicit MpiError(const std::string& message);**
**MpiError(const std::string& message, const Mpi& mpi);**
**MpiError(const std::string& message, const Mpi& mpi, const int threadNumber);**

En un archivo MpiError.cpp implemente los tres constructores. Cada uno debe invocar al constructor de std::runtime_error y proveerle un texto cuyo formato es el siguiente para cada constructor (las líneas corresponden corresponden a los constructores del código previo):

**message**
**hostname:process_number: mesasge**
**hostname:process_number.thread_number: mesasge**

Modifique la implementación de la clase Mpi para que cuando una función de la biblioteca MPI retorne un código de error, su clase lance una instancia de MpiError. Pruebe su código con varios procesos. Modifique temporalmente su código para provocar o lanzar una excepción y verificar que el texto con formato de la excepción se imprima en el error estándar.
