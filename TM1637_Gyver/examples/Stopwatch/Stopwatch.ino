//  Author:Fred.Chu
//  Date:9 April,2013
//  catalex.taobao.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
//  Modified record:
//
/*******************************************************************************/
#include <EEPROM.h>
#include <TimerOne.h>
#include <avr/pgmspace.h>
#include "TM1637.h"
#define ON 1
#define OFF 0

int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};
unsigned char ClockPoint = 1;
unsigned char Update;
unsigned char microsecond_10 = 0;
unsigned char second;
unsigned char _microsecond_10 = 0;
unsigned char _second;
unsigned int eepromaddr;
boolean Flag_ReadTime;

#define CLK 3//pins definitions for TM1637 and can be changed to other ports        
#define DIO 2
TM1637 tm1637(CLK,DIO);

void setup()
{
  Serial.begin(9600);
  tm1637.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  tm1637.init();
  Timer1.initialize(10000);//timing for 10ms
  Timer1.attachInterrupt(TimingISR);//declare the interrupt serve routine:TimingISR  
  Serial.println("Please send command to control the stopwatch:");
  Serial.println("S - start");
  Serial.println("P - pause");
  Serial.println("L - list the time");
  Serial.println("W - write the time to EEPROM ");
  Serial.println("R - reset");
}
void loop()
{
  char command;
  command = Serial.read();
  switch(command)
  {
    case 'S':stopwatchStart();Serial.println("Start timing...");break;
    case 'P':stopwatchPause();Serial.println("Stopwatch was paused");break;
    case 'L':readTime();break;
    case 'W':saveTime();Serial.println("Save the time");break;
    case 'R':stopwatchReset();Serial.println("Stopwatch was reset");break;
    default:break;
  }
  if(Update == ON)
  {
    TimeUpdate();
    tm1637.display(TimeDisp);
  }
}
//************************************************
void TimingISR()
{
  microsecond_10 ++;
  Update = ON;
  if(microsecond_10 == 100){
    second ++;
    if(second == 60)
    {
      second = 0;
    }
    microsecond_10 = 0;  
  }
  ClockPoint = (~ClockPoint) & 0x01;
  if(Flag_ReadTime == 0)
  {
    _microsecond_10 = microsecond_10;
    _second = second;
  }
}
void TimeUpdate(void)
{
  if(ClockPoint)tm1637.point(POINT_ON);//POINT_ON = 1,POINT_OFF = 0;
  else tm1637.point(POINT_ON); 
  TimeDisp[2] = _microsecond_10 / 10;
  TimeDisp[3] = _microsecond_10 % 10;
  TimeDisp[0] = _second / 10;
  TimeDisp[1] = _second % 10;
  Update = OFF;
}
void stopwatchStart()//timer1 on
{
  Flag_ReadTime = 0;
  TCCR1B |= Timer1.clockSelectBits; 
}
void stopwatchPause()//timer1 off if [CS12 CS11 CS10] is [0 0 0].
{
  TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
}
void stopwatchReset()
{
  stopwatchPause();
  Flag_ReadTime = 0;
  _microsecond_10 = 0;
  _second = 0;
  microsecond_10 = 0;
  second = 0;
  Update = ON;
}
void saveTime()
{
  EEPROM.write(eepromaddr ++,microsecond_10);
  EEPROM.write(eepromaddr ++,second);
}
void readTime()
{
  Flag_ReadTime = 1;
  if(eepromaddr == 0)
  {
    Serial.println("The time had been read");
    _microsecond_10 = 0;
    _second = 0;
  }
  else{
  _second = EEPROM.read(-- eepromaddr);
  _microsecond_10 = EEPROM.read(-- eepromaddr);
  Serial.println("List the time");
  }
  Update = ON;
}