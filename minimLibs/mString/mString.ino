#include "mString.h"

void setup() {
  Serial.begin(9600);
  char buf[50];

  // сравнение со String
  String str;
  str = "kek";
  str = str + ',' + 12345 + ',' + 3.14 + ',' + "hello";  
  Serial.println(str);
  str = 1234;
  Serial.println(str.toInt());
  // скетч занимает 5370/208 байт

/*
  mString str(buf);
  str = "kek";
  str = str + ',' + 12345 + ',' + 3.14 + ',' + "hello";  
  Serial.println(str.buf);
  str = 1234;
  Serial.println(str.toInt());
  // скетч занимает 2402/198 байт
*/


  // прочие тесты
  /*
    // работа с буфером
    mString test(buf, 50);
    test.add(F("abc")).add("def").add(" loh").add('=');
    test += "kek";
    test.add(-1289).add(3.14).add("sis").add(-1289);
    test[2] = 't';
    Serial.println(test.buf);

    test = 10;
    test = test + 123.456 + ' ' + 15 + "abcd";
    test.add('a').add('b').add('c');

    Serial.println(test.buf);
  */
  /*
    // парсинг
    test = "1234,2345,3456,4567,5,6";
    Serial.println(test.startsWith("lol2"));
    Serial.println(test.indexOf('k'));

    int data[10];
    byte get = test.parseInts(data, 10);
    Serial.println(get);
    Serial.println();

    for (int i = 0; i < get; i++) {
      Serial.println(data[i]);
    }
  */
}

void loop() {
}
