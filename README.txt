
El repositorio incluye dos directorios principales (client y server), cada
directorio tiene la misma estructura

/dir
    /bin/   <-  contiene el ejecutable
    /obj/   <-  contiene los archivos .o para compilación
    /src/   <-  contiene el código fuente del proyecto

*******************************************************************************

COMPILACIÓN

Para compilar ya sea el cliente o el servidor ejecutar el siguiente comando. 

    make

NOTA: es necesario estar dentro del directorio con el archivo Makefile.

*******************************************************************************

EJECUCIÓN

Para correr el ejecutable se corre como cualquier ejecutable en UNIX usando ./ 
seguido por la ruta y el nombre del ejecutable que se encuentra en bin.

*******************************************************************************

Para limpiar los directorios de obj y bin ejecutar el siguiente comando.

    make clean

NOTA: es necesario estar dentro del directorio con el archivo Makefile.
