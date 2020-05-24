// спим в EXTSTANDBY_SLEEP, считаем время таймером 2
// используется uptime2.h (лежит в папке с примером)
// НЕ РЕКОМЕНДУЕТСЯ К ИСПОЛЬЗОВАНИЮ, смотрите пример sleepDelayMillis

#include "uptime2.h"
#include <GyverPower.h>

void setup() {
  Serial.begin(9600);
  uptime2Init();    // запуск миллиса на 2 таймере
  power.setSleepMode(EXTSTANDBY_SLEEP);

  // ВНИМАНИЕ!
  // миллис2 сделан на прерываниях таймера 2
  // прерывания настроены на частоту ~976.56 Гц
  // с этой частотой МК будет просыпаться вне зависимости от указанного периода сна!!
  // можно даже SLEEP_FOREVER поставить
}

void loop() {
  static uint32_t tmr;
  // наш таймер на миллис2
  if (millis2() - tmr >= 1000) {
    tmr += 1000;
    Serial.print("time: ");
    Serial.println(millis2());
    delay2(50);
  }

  // спим
  power.sleep(SLEEP_FOREVER);
  // тут проснулись в любом случае по прерыванию и код продолжает выполняться
}
