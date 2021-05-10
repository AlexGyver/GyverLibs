// тест парсера строк
#include "Parser.h"

void setup() {
  Serial.begin(9600);

  // ==================== ПРИМЕР 1 ======================
  // строка для примера
  // данные разделены разделителем, например запятой
  // могут быть получены из Serial/UDP/TCP/MQTT итд
  char str[] = "1234,3.14,hello,4567,lolkek,qwerty";

  // кормим строку парсеру, указываем разделитель (умолч. запятая)
  Parser data(str, ',');

  // разделяем
  // ВНИМАНИЕ! Операция "ломает" строку, заменяя разделители на NULL
  int am = data.split();
  // получаем количество данных

  Serial.println(am); // выводим количество

  // можем обратиться к полученным строкам как data[i] или data.str[i]
  for (byte i = 0; i < am; i++) Serial.println(data[i]);

  // также можно получить их в виде int и float чисел
  // передав индекс строки
  Serial.println(data.getInt(0));
  Serial.println(data.getFloat(1));

  // можно сравнить со строкой (номер парс строки, строка для сравнения)
  if (data.equals(2, "hello")) Serial.println("true");
  else Serial.println("false");
  
  Serial.println();
  
  // ==================== ПРИМЕР 2 ======================
  // быстрый парсинг целочисленных данных с разделителем
  char str2[] = "123,456,789,222,333,444";
  Parser data2(str2);   // кормим строку парсеру

  // создаём массив с количеством ячеек data2.amount() - столько данных в пакете
  int ints[data2.amount()];
  
  int am2 = data2.parseInts(ints);  // раскидает в указанный массив и вернёт количество
  // фактически тут am2 == data2.amount() - количество данных
  // выводим
  for (byte i = 0; i < am; i++) Serial.println(ints[i]);
}

void loop() {
}
