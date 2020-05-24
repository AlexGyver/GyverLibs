// помогаем миллису сохранить свой счёт на время сна sleepDelay
#include <GyverPower.h>

// На время сна в большинстве режимов millis() останавливается,
// но сон при помощи функции sleepDelay автоматически корректирует millis()! 

void setup() {
  Serial.begin(9600);
  Serial.print("calibraion... ");
  
  power.autoCalibrate(); // автоматическая калибровка таймера сна (чтобы спал точное время)
  // выполняется ~2 секунды
  // см. пример WDT_calibration

  Serial.println("done");
  delay(50);

  // по умолчанию стоит самый экономный режим сна POWER DOWN
  
  // power.correctMillis(false); // можно отключить коррекцию миллис (по умолч. включена)
}

void loop() {
  // опрашиваем датчики, мигаем светодиодами, etc
  // таймер на миллис на 3 секунды для теста
  static uint32_t tmr;
  if (millis() - tmr >= 3000) {
    tmr += 3000;
    Serial.println("kek");
    delay(50);
  }

  // спим 1.5 секунды (1500 мс)
  power.sleepDelay(1500);
  // проснулись
  // миллис корректируется автоматически!
}
