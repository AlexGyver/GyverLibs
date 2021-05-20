#include "UnixDate.h"

UnixDate stamp(3);  // указать GMT (3 для Москвы)

void setup() {
  Serial.begin(9600);
  // ========= Дата и время в UNIX ==========
  // установка даты и времени библиотеки через функцию setDateTime
  // setDateTime(год, месяц (с 1), день (с 1), час, минута, секунда);
  // либо stamp.year = 2021 и так далее
  //stamp.setDateTime(2021, 1, 1, 0, 0, 0);
  stamp.setDateTime(2021, 5, 20, 7, 4, 15);   // 20 мая 2021, 7:04:15

  // getUnix() - получить unix время (выполняется ~56 us на AVR)
  uint32_t unix = stamp.getUnix();
  Serial.println(unix);

  // ========= UNIX в дату и время ==========
  // конвертировать unix в дату и время
  // getDateTime(unix stamp) выполняется ~500 us на AVR
  // https://www.unixtimestamp.com/index.php
  stamp.getDateTime(1621497944);

  // забираем вот так
  Serial.println(stamp.year);
  Serial.println(stamp.month);
  Serial.println(stamp.day);
  Serial.println(stamp.hours);
  Serial.println(stamp.minutes);
  Serial.println(stamp.seconds);
  Serial.println(stamp.dayOfWeek);  // 1 понедельник, 7 воскресенье
}

void loop() {
}
