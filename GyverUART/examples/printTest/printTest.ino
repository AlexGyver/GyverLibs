#include <GyverUART.h>

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

void setup() {
  uartBegin();
  uartPrintln(data1);
  uartPrintln(data2);
  uartPrintln(data3);
  uartPrintln(data4);
  uartPrintln(data5);
  uartPrintln(data6);
  uartPrintln(data7);
  uartPrintln(data8, 3);
  uartPrintln(data9);
  uartPrintln("KEK KEK");
  uartPrintln(F("KEK KEK MACRO"));
  uartPrintln(data10[2]);
}

void loop() {
  
}
