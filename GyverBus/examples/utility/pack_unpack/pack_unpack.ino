#include "GyverBus.h"

void setup() {
  Serial.begin(9600);

  // для примера возьмём структуру
  struct MyStruct {
    byte val_b;
    int val_i;
    float val_f;
  };

  // создадим и заполним данными
  MyStruct struct1;
  struct1.val_b = 123;
  struct1.val_i = 12345;
  struct1.val_f = 123.456;

  // байтовый массив
  byte data[sizeof(struct1)];

  // разбиваем структуру на байты и пакуем в массив
  packDataBytes(data, struct1);

  // отправили-приняли итд

  // создаём пустую структуру
  MyStruct struct2;

  // распаковываем в неё буфер
  unpackDataBytes(data, struct2);

  // выводим проверяем
  Serial.println(struct2.val_b);
  Serial.println(struct2.val_i);
  Serial.println(struct2.val_f);
}

void loop() {
  // put your main code here, to run repeatedly:

}
