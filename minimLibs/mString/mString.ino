#include "mString.h"
/*
  Создание
  mString<размер> str - свой буфер
  mString<0> str(char буфер, размер) - внешний буфер

  str.length() - текущий размер
  str.clear() - очистить
  str.add( [char / char* / Fchar / числа / String] ) - добавить
  str += [char / char* / Fchar / числа / String] - добавить
  str = str + [char / char* / Fchar / числа / String] - можно суммировать
  str == [char / char* / числа / String] - сравнить
  Для добавления/сравнения с mString используй str.buf

  Чтение символа по индексу
  str[idx]
  str.buf[idx]
  str.charAt(idx)

  Запись символа по индексу
  str[idx] = с
  str.buf[idx] = с
  str.setCharAt(idx, c)

  Доступ к char буферу
  str.buf
  str.c_str()

  str.toInt(from) - преобразовать в int начиная с from
  str.toUint(from) - преобразовать в uint начиная с from
  str.toFloat(from) - преобразовать в float начиная с from
  str.startsWith(char*) - начинается с
  str.substring(from, to, char* arr) - скопировать с from до to во внешний arr
  str.truncate(amount) - обрезать с конца на amount
  str.remove(idx, amount) - удалить (вырезать) amount символов начиная с idx
  str.toLowerCase() - преобразовать буквы в нижний регистр
  str.toUpperCase() - преобразовать буквы в верхний регистр
  str.indexOf(char, from) - найти символ char, искать начиная с from
  str.indexOf(char*, from) - найти строку char, искать начиная с from

  Парсинг пакета, в котором данные разделены разделителем div и оканчиваются символом ter
  str.parseBytes(data, len, div, ter) - распарсить содержимое в массив byte длиной len
  str.parseInts(data, len, div, ter) - распарсить содержимое в массив int длиной len
  div и ter по умолчанию , и NULL
  Например для парсинга таких пакетов: "12,34,56"
  Кастомные: "12;34;56;78\n"
  Парсим str.parseBytes(data, len, ';', '\n')
*/

void setup() {
  Serial.begin(9600);
  /*
      mString<20> str;
      str = "kek";
      str = str + ',' + 12345 + ',' + 3.14 + ',' + "hello";
      Serial.println(str.buf);
      str = 1234;
      Serial.println(str.toInt());
      // скетч занимает 2652/218 байт
  */
  /*
      // сравнение со String
      String str;
      str = "kek";
      str = str + ',' + 12345 + ',' + 3.14 + ',' + "hello";
      Serial.println(str);
      str = 1234;
      Serial.println(str.toInt());
      // скетч занимает 5370/208 байт
  */

  // прочие тесты
  // работа с буфером
  //char cBuf[50];
  //mString<0> test(cBuf, 50);
  mString<50> test;

  test.add(F("abc")).add("def").add(" loh").add('=');
  test += "kek";
  test.add(-1289).add(3.14).add("sis").add(-1289);
  test[2] = 't';
  Serial.println(test.buf);   // abtdef loh=kek-12893.14sis-1289

  test = 10;
  test = test + 123.456 + ' ' + 15 + "abcd";
  test.add('a').add('b').add('c');

  mString<10> some;
  some = " added ";
  String str = "String";
  test += some.buf;
  test += str;

  Serial.println(test.buf);   // 10123.45 15abcdabc added String

  // парсинг
  test = "1234,2345,3456,4567,5,6";
  Serial.println(test.startsWith("lol2"));  // 0
  Serial.println(test.startsWith("1234"));  // 1
  Serial.println(test.indexOf('k'));    // -1

  int data[10];
  byte get = test.parseInts(data, 10);
  Serial.println(get);    // 6
  Serial.println();

  for (int i = 0; i < get; i++) {
    Serial.println(data[i]);
  }

  // поиск
  test = "puk kek hello friend hello world";
  Serial.println(test.indexOf('h'));       // 8
  Serial.println(test.indexOf('h', 9));    // 21
  Serial.println(test.indexOf("friend"));  // 14

  // сравнение
  test = 12;
  Serial.println(test == 12);   // 1

  test = "kek";
  Serial.println(test == "kek");    // 1

  mString<40> test2;
  test2 = "kek";
  Serial.println(test == test2.buf);    // 1

  // выделение
  test = "abcd,123,-456,3.14";
  Serial.println(test.toInt(5)); // 123
  Serial.println(test.toInt(9)); // 123
  Serial.println(test.toFloat(14)); // 3.14

  char buf3[10];
  test.substring(0, 5, buf3);
  Serial.println(buf3);   // abcd,1

  test.remove(5, 4);   // откуда, сколько
  test.toUpperCase();
  test.truncate(4);
  Serial.println(test.c_str());  // abcd,-456,3.14

}

void loop() {
}
