// Ejemplo modificado para usar USART por hardware de la STM32F103C8
// utilizando el core STM32GENERIC.
// En la STM32F103C8, el pin Tx es el PA2, y el Rx el PA3.
// necesita que el método timedRead() de la clase Stream sea público

/** Demonstrate how to play a file by it's (FAT table) index number.
 * @author: RndMnkIII rndmnkiii@gmail.com. Modificado el 27-07-2017.
 * @originalauthor James Sleeman,  http://sparks.gogo.co.nz/
 * @license MIT License
 * @file
 */
 
#include <JQ6500_Serial_HardwareSerial.h>

// Create the mp3 module object, 
//   Arduino Pin 8 is connected to TX of the JQ6500
//   Arduino Pin 9 is connected to one end of a  1k resistor, 
//     the other end of the 1k resistor is connected to RX of the JQ6500
//   If your Arduino is 3v3 powered, you can omit the 1k series resistor
JQ6500_Serial_HardwareSerial mp3(Serial1);

void setup() {
  Serial1.begin(9600);//inicializamos UART2 antes de usar las funciones de JQ6500
  mp3.reset();
  mp3.setVolume(20);
  mp3.setLoopMode(MP3_LOOP_NONE);

}

void loop() {
  
  if(mp3.getStatus() != MP3_STATUS_PLAYING)
  {
    mp3.playFileByIndexNumber(1);  
  }
  
}
