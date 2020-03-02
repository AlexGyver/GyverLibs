/*
   Пример с максимальной экономией flash при использовании библиотеки
   Если датчик один - не используйте адресацию
   Если датчиков несколько - рекомендуется использование адресации и ОДНОЙ линии для экономии flash
*/

/* Производим настройку в .h файле библиотеки
#define TEMP_TYPE int     // Избавляемся от громоздких чисел с плавающей точкой
#define CHECK_CRC false   // Отключаем проверку подлинности (немного экономит память)
 */
 
#include <microDS18B20.h>

#define DS_PIN 9
MicroDS18B20 sensor(DS_PIN); // Датчик один - не используем адресацию

void setup() {
  Serial.begin(9600);
}

void loop() {
  sensor.requestTemp();
  delay(1000);
  Serial.print("t: ");
  Serial.print(sensor.getTemp());
  Serial.println(" *C");
}
