#include <GyverUART.h>

char data0 = 'b';
int8_t data1 = -50;
uint8_t data2 = 125;
int16_t data3 = -2000;
uint16_t data4 = 30000;
int32_t data5 = -70000;
uint32_t data6 = 4194967295;
float data7 = 3681.65424;
float data8 = -4375.12353;
String data9 = "LOL LOL";

const char *data10[] = {
  "LOL",
  "KEK",
  "CHEBUREK",
};

byte data11 = 0b11101011;
uint32_t data12 = 0xFAB666;

void setup() {
  uart.begin();
  uart.println(data0);
  uart.println(data1);
  uart.println(data2);
  uart.println(data3);
  uart.println(data4);
  uart.println(data5);
  uart.println(data6);
  uart.println(data7);
  uart.println(data8, 3);
  uart.println(data9);
  uart.println("KEK KEK");
  uart.println(F("KEK KEK MACRO"));
  uart.println(data10[2]);
  uart.println(data11, BIN);
  uart.println(data12, HEX);
}

void loop() {
  
}
