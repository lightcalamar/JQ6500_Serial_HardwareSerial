// Modificada  para usar USART por hardware de la STM32F103C8
// utilizando el core STM32GENERIC.
// En la STM32F103C8, USART2 -> pin Tx es el PA2, y el Rx el PA3.
// necesita que el método timedRead() de la clase Stream sea público
/** 
 * Arduino Library for JQ6500 MP3 Module
 * 
 * Copyright (C) 2014 James Sleeman, <http://sparks.gogo.co.nz/jq6500/index.html>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 * THE SOFTWARE.
 * 
 * @author James Sleeman, http://sparks.gogo.co.nz/
 * @license MIT License
 * @file
 */

#include "JQ6500_Serial_HardwareSerial.h"

JQ6500_Serial_HardwareSerial::JQ6500_Serial_HardwareSerial(HardwareSerial& suart) : _suart(suart)
{
}	
void  JQ6500_Serial_HardwareSerial::play()
{
  this->sendCommand(0x0D);
}

void  JQ6500_Serial_HardwareSerial::restart()
{
  byte oldVolume = this->getVolume();  
  this->setVolume(0);
  this->next();
  this->pause();
  this->setVolume(oldVolume);
  this->prev();
}

void  JQ6500_Serial_HardwareSerial::pause()
{
  this->sendCommand(0x0E);
}

void  JQ6500_Serial_HardwareSerial::next()
{
  this->sendCommand(0x01);
}

void  JQ6500_Serial_HardwareSerial::prev()
{
  this->sendCommand(0x02);
}

void  JQ6500_Serial_HardwareSerial::playFileByIndexNumber(unsigned int fileNumber)
{  
  this->sendCommand(0x03, (fileNumber>>8) & 0xFF, fileNumber & (byte)0xFF);
}

void  JQ6500_Serial_HardwareSerial::nextFolder()
{
  this->sendCommand(0x0F, 0x01);  
}

void  JQ6500_Serial_HardwareSerial::prevFolder()
{
  this->sendCommand(0x0F, 0x00);
}

void  JQ6500_Serial_HardwareSerial::playFileNumberInFolderNumber(unsigned int folderNumber, unsigned int fileNumber)
{
  this->sendCommand(0x12, folderNumber & 0xFF, fileNumber & 0xFF);
}

void  JQ6500_Serial_HardwareSerial::volumeUp()
{
  this->sendCommand(0x04);
}

void  JQ6500_Serial_HardwareSerial::volumeDn()
{
  this->sendCommand(0x05);
}

void  JQ6500_Serial_HardwareSerial::setVolume(byte volumeFrom0To30)
{
  this->sendCommand(0x06, volumeFrom0To30);
}

void  JQ6500_Serial_HardwareSerial::setEqualizer(byte equalizerMode)
{
  this->sendCommand(0x07, equalizerMode);
}

void  JQ6500_Serial_HardwareSerial::setLoopMode(byte loopMode)
{
  this->sendCommand(0x11, loopMode);
}

void  JQ6500_Serial_HardwareSerial::setSource(byte source)
{
  this->sendCommand(0x09, source);
}

void  JQ6500_Serial_HardwareSerial::sleep()
{
  this->sendCommand(0x0A);
}

void  JQ6500_Serial_HardwareSerial::reset()
{
  this->sendCommand(0x0C);
  delay(500); // We need some time for the reset to happen
}


byte  JQ6500_Serial_HardwareSerial::getStatus()    
    {
      byte statTotal = 0;
      byte stat       = 0;
      do
      {
        statTotal = 0;
        for(byte x = 0; x < MP3_STATUS_CHECKS_IN_AGREEMENT; x++)
        {
          stat = this->sendCommandWithUnsignedIntResponse(0x42);      
          if(stat == 0) return 0; // STOP is fairly reliable
          statTotal += stat;
        }

      } while (statTotal != 1 * MP3_STATUS_CHECKS_IN_AGREEMENT && statTotal != 2 * MP3_STATUS_CHECKS_IN_AGREEMENT);
      
      return statTotal / MP3_STATUS_CHECKS_IN_AGREEMENT;      
    }
    
    byte  JQ6500_Serial_HardwareSerial::getVolume()    { return this->sendCommandWithUnsignedIntResponse(0x43); }
    byte  JQ6500_Serial_HardwareSerial::getEqualizer() { return this->sendCommandWithUnsignedIntResponse(0x44); }
    byte  JQ6500_Serial_HardwareSerial::getLoopMode()  { return this->sendCommandWithUnsignedIntResponse(0x45); }
    unsigned int  JQ6500_Serial_HardwareSerial::getVersion()   { return this->sendCommandWithUnsignedIntResponse(0x46); }
    
    unsigned int  JQ6500_Serial_HardwareSerial::countFiles(byte source)   
    {
      if(source == MP3_SRC_SDCARD)
      {
        return this->sendCommandWithUnsignedIntResponse(0x47); 
      }
      else if (source == MP3_SRC_BUILTIN)
      {
        return this->sendCommandWithUnsignedIntResponse(0x49);
      }     
      
      return 0;
    }
    
    unsigned int  JQ6500_Serial_HardwareSerial::countFolders(byte source) 
    {
      if(source == MP3_SRC_SDCARD)
      {
        return this->sendCommandWithUnsignedIntResponse(0x53); 
      }
      
      return 0;
    }
    
    unsigned int  JQ6500_Serial_HardwareSerial::currentFileIndexNumber(byte source)
    {
      if(source == MP3_SRC_SDCARD)
      {
        return this->sendCommandWithUnsignedIntResponse(0x4B); 
      }
      else if (source == MP3_SRC_BUILTIN)
      {
        return this->sendCommandWithUnsignedIntResponse(0x4D)+1; // CRAZY!
      }     
      
      return 0;
    }
    
    unsigned int  JQ6500_Serial_HardwareSerial::currentFilePositionInSeconds() { return this->sendCommandWithUnsignedIntResponse(0x50); }
    unsigned int  JQ6500_Serial_HardwareSerial::currentFileLengthInSeconds()   { return this->sendCommandWithUnsignedIntResponse(0x51); }
    
    void          JQ6500_Serial_HardwareSerial::currentFileName(char *buffer, unsigned int bufferLength) 
    {
      this->sendCommand(0x52, 0, 0, buffer, bufferLength);
    }
    
    // Used for the status commands, they mostly return an 8 to 16 bit integer 
    // and take no arguments
    unsigned int JQ6500_Serial_HardwareSerial::sendCommandWithUnsignedIntResponse(byte command)
    {      
      char buffer[5];
      this->sendCommand(command, 0, 0, buffer, sizeof(buffer));
      return (unsigned int) strtoul(buffer, NULL, 16);
    }
    
    void  JQ6500_Serial_HardwareSerial::sendCommand(byte command)
    {
      this->sendCommand(command, 0, 0, 0, 0);
    }
    
    void  JQ6500_Serial_HardwareSerial::sendCommand(byte command, byte arg1)
    {
       this->sendCommand(command, arg1, 0, 0, 0);
    }
    
    void  JQ6500_Serial_HardwareSerial::sendCommand(byte command, byte arg1, byte arg2)
    {
       this->sendCommand(command, arg1, arg2, 0, 0);
    }    
    
    void  JQ6500_Serial_HardwareSerial::sendCommand(byte command, byte arg1, byte arg2, char *responseBuffer, unsigned int bufferLength)
    {
      
      
      // Command structure
      // [7E][number bytes following including command and terminator][command byte][?arg1][?arg2][EF]
      
      // Most commands do not have arguments
      byte args = 0;
      
      // These ones do
      switch(command)
      {        
        case 0x03: args = 2; break;
        case 0x06: args = 1; break;
        case 0x07: args = 1; break;        
        case 0x09: args = 1; break;
        case 0x0F: args = 1; break;
        case 0x11: args = 1; break;
        case 0x12: args = 2; break;
      }
      
#if MP3_DEBUG
      char buf[4];       
      _suart.println();
      _suart.print("7E ");      
      itoa(2+args, buf, 16); _suart.print(buf); _suart.print(" "); memset(buf, 0, sizeof(buf));
      itoa(command, buf, 16); _suart.print(buf); _suart.print(" "); memset(buf, 0, sizeof(buf));
      if(args>=1) itoa(arg1, buf, 16); _suart.print(buf); _suart.print(" "); memset(buf, 0, sizeof(buf));
      if(args>=2) itoa(arg2, buf, 16); _suart.print(buf); _suart.print(" "); memset(buf, 0, sizeof(buf));
      _suart.print("EF");      
#endif
      
      // The device appears to send some sort of status information (namely "STOP" when it stops playing)
      // just discard this right before we send the command
      while(this->waitUntilAvailable(10)) _suart.read();
      
      _suart.write((byte)0x7E);
      _suart.write(2+args);
      _suart.write(command);
      if(args>=1) _suart.write(arg1);
      if(args==2) _suart.write(arg2);
      _suart.write((byte)0xEF);
      
      
      unsigned int i = 0;
      char         j = 0;
      if(responseBuffer && bufferLength) 
      {
        memset(responseBuffer, 0, bufferLength);
      }
      
      // Allow some time for the device to process what we did and 
      // respond, up to 1 second, but typically only a few ms.
      this->waitUntilAvailable(1000);

      
#if MP3_DEBUG
      _suart.print(" ==> [");
#endif
      
      while(this->waitUntilAvailable(150))
      {
        j = (char) _suart.read();
        
#if MP3_DEBUG
        _suart.print(j);
#endif
        if(responseBuffer && (i<(bufferLength-1)))
        {
           responseBuffer[i++] = j;
        }
      }
      
#if MP3_DEBUG      
      _suart.print("]");
      _suart.println();
#endif
      
    }
    
    
// as readBytes with terminator character
// terminates if length characters have been read, timeout, or if the terminator character  detected
// returns the number of characters placed in the buffer (0 means no valid data found)

size_t JQ6500_Serial_HardwareSerial::readBytesUntilAndIncluding(char terminator, char *buffer, size_t length, byte maxOneLineOnly)
{
    if (length < 1) return 0;
  size_t index = 0;
  while (index < length) {
    int c = _suart.timedRead();
    if (c < 0) break;    
    *buffer++ = (char)c;
    index++;
    if(c == terminator) break;
    if(maxOneLineOnly && ( c == '\n') ) break;
  }
  return index; // return number of characters, not including null terminator
}


// Waits until data becomes available, or a timeout occurs
int JQ6500_Serial_HardwareSerial::waitUntilAvailable(unsigned long maxWaitTime)
{
  unsigned long startTime;
  int c = 0;
  startTime = millis();
  do {
    c = _suart.available();
    if (c) break;
  } while(millis() - startTime < maxWaitTime);
  
  return c;
}
