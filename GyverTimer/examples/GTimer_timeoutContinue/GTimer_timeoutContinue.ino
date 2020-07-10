// Пример работы в режиме таймаута

#include "GyverTimer.h"   // подключаем библиотеку

GTimer myTimer(MS);    // создать миллисекундный таймер
//GTimer myTimer(US);    // US - микросекундный

void setup() {
  Serial.begin(9600);
  myTimer.setTimeout(3000);   // настроить таймаут 3 сек
  Serial.println("Start");
}

void loop() {
  // выведет Timeout 3 sec! через 3 секунды после вызова setTimeout(3000)
  // и будет выводить постоянно вплоть ДО ПЕРЕЗАПУСКА ТАЙМЕРА
  if (myTimer.isReady() || !myTimer.isEnabled()) Serial.println("Timeout 3 sec!");
  
  // после срабатывания остановит счёт
  // можно перезапустить при помощи setTimeout(время) или start()
}
