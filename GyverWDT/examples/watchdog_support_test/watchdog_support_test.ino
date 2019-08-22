#include <GyverWDT.h>
/* Скетч , тестирующий поддержку автоматического watchdog reset на вашем bootloader'е */

/* !!! Если после запуска скетча программа перезапускается каждые ~8с - ваш bootloader поддерживает  watchdog reset !!!
   !!! Если после таймаута программа не перезагружается , а светодиод на плате начинает быстро мигать - watchdog reset не поддерживается !!!
   Если ваш bootloader не поддерживаем watchdog reset , вы можете использовать watchdog для генерации прерываний, см. GyverWDT.h
*/

void setup() {
  Serial.begin(9600);
  Serial.println("wait 10 sec");
  delay(10000); // задержка , чтобы иметь возможность перепрошить контроллер в случае bootloop
  watchdog_enable(WDT_PRESCALER_1024); // таймаут ~ 8с
  Serial.println("watchdog enabled");
  while (1) { // бесконечный цикл, в котором нет watchdog_reset();
    if (!(millis() % 1000)) { // каждую секунду
      delay(10);
      Serial.println((int)((millis() / 1000) - 10)); // выводим прошедшее время в секундах
    }
  }
}

void loop() {

}
