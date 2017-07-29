JQ6500_Serial_USART2
=======================

Modificación de la libreria JQ6500_Serial para utilizar puerto serie hardware alternativo de las placas STM32

Modificado por RndMnkIII. 27-07-2017

Simple to use Arduino library to interface to JQ6500 (JQ6500-28P, JQ6500-16P) Mp3 Player Modules

For complete documentation about the JQ6500 Mp3 Player Module, see: 
   http://sparks.gogo.co.nz/jq6500/index.html
   
For a library methods reference see:
   http://sparks.gogo.co.nz/jq6500/doxygen/class_j_q6500___serial.html
   
Como utilizarla:
----------------

Localizar pines USART2, USART3 ó USART4 de la placa STM32. Consultar documentación de referencia
o utilizar la aplicación STM32CubeMX.

Por ejemplo en el micro STM32F103C8, se pueden localizar la USART2 en los pines PA2(Tx),PA3(Rx), y la
USART3 en los pines PB10(Tx),PB11(Rx). Conectar el Tx de la STM32 con el Rx de la JQ6500, y el Rx de
la STM32 al Tx de la JQ6500, además de tener conectado en común el GND. Tener en cuenta las tolerancias
con los niveles lógicos de las señales de 3.3 ó 5v.

Hay que modificar la clase Stream para hacer publico el método timedRead(). Se incluyen archivos
Stream.h y Stream.cpp, descomprimirlos en la carpeta, sustituyendo a los anteriores:

Arduino\hardware\STM32GENERIC\STM32\cores\arduino

Llamar al constructor de la clase JQ6500_Serial_USART2, pasando como parámetro el puerto USART de la 
STM32 que vamos a utilizar, por ejemplo, para usar SerialUART2:

JQ6500_Serial_USART2 mp3(SerialUART2);

Para usar SerialUART3:
JQ6500_Serial_USART2 mp3(SerialUART3); //Este puerto serie todavía no ha sido probado

El resto de métodos de la clase JQ6500_Serial_USART2 se utilizan de la misma forma que la clase
original JQ6500_Serial.

Uso:
--------------------------

Abre el ejemplo PlayByNumber_USART2.
