/*
   Все возможности библиотки.
*/

/* Производим настройку в .h файле библиотеки
#define TEMP_TYPE float / int  // Тип данных для температуры точность / экономия flash (По умолчанию float)
#define CHECK_CRC true / false // Проверка подлинности принятых данных (По умолчанию true)
#define CRC_USE_TABLE false / true // Использовать таблицу для CRC быстрее,но +256 байт flash (<1мкс VS ~6мкс)
*/

#include <microDS18B20.h>

#define DS_PIN1 9   // Создаем две линии для подключения сенсоров
#define DS_PIN2 10


/*
   Пример подключения нескольких датчиков к одной линии с адресацией
   Указываем уникальные адреса датчиков для работы на одной линии (пине)
   Для получения адресов используйте пример address_read
*/
const uint8_t sensor1_address[8] = {0x28, 0x13, 0xCD, 0xEC, 0x9, 0x0, 0x0, 0x7};
const uint8_t sensor2_address[8] = {0x32, 0x10, 0xC0, 0x1C, 0x90, 0x0, 0x0, 0x9};
MicroDS18B20 sensor1(DS_PIN1, sensor1_address); // Создаем два датчика с адресацией на одной линии
MicroDS18B20 sensor2(DS_PIN1, sensor2_address);


/*
   Пример создания датчика без адресации (один датчик на линии)
*/
MicroDS18B20 sensor3(DS_PIN2); 		// Создаем еще датчик на отдельной линии без адресации


void setup() {
  Serial.begin(9600);

  /* пример чтения адреса третьего термометра */
  uint8_t address[8]; 				// массив, в который мы считаем адрес третьего датчика
  sensor3.readAddress(address);
  Serial.print("Address: {");
  for (uint8_t i = 0; i < 8; i++) {
    Serial.print("0x");
    Serial.print(address[i], HEX);
    if (i < 7)Serial.print(",");
  }
  Serial.println("};");

  /* пример настройки разрешения датчика */
  
  sensor1.setResolution(12);
  sensor2.setResolution(9);
}

/*
   запрос температуры на одной линии делается только для одного термометра
   пока преобразование в процессе - линия "занята"
   на разных линиях можно работать паралельно
*/

void loop() {

  /*
     Пример запроса температуры
  */
  sensor1.requestTemp();
  sensor3.requestTemp();
  delay(1000);

  /*
     Пример чтения температуры температуры
  */
  Serial.print("sensor1 :");
  Serial.print(sensor1.getTemp());
  Serial.println(" *C");

  Serial.print("sensor3 :");
  Serial.print(sensor3.getTemp());
  Serial.println(" *C");
}
