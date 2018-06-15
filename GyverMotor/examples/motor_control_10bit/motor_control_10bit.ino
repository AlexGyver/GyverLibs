/*
	Пример управления мотором при помощи драйвера полного моста
	и потенциометра. На всём диапазоне потенциометра:
	от 0 до мёртвой зоны (512 - deadzone) крутим в одну сторону
	в мёртвой зоне не крутим вообще
	от мёртвой зоны (512 + deadzone) до 1023 крутим в другую сторону
	Управление производится ШИМом на timer1.
	Управление 10 битным ШИМом!!! 0-1023
	Вывод таймера1 расположен на 10 и 11 пинах. В такой системе
	не работает управление сервоприводами из библиотеки Servo!
*/

#define DEADZONE 60 // "мёртвая зона" потенциометра
#define IN1 2
#define IN2 10		// IN2 обязательно должен быть 10 или 11 (уно, нано)
#define POT 0      	// сюда подключен потенциометр

#include "GyverMotor.h"
GMotor motor1(IN1, IN2);

int left_min = 512 - DEADZONE;   // расчёт границ мёртвой зоны
int right_min = 512 + DEADZONE;  // расчёт границ мёртвой зоны

void setup() {
  motor1.init10bit(20000);	// настройка частоты (тут 20 кгц)
  motor1.reverse(true);
}

void loop() {
  int potent = analogRead(POT);                    // читаем с потенциометра

  if (potent > left_min && potent < right_min) {  // если мы в "мёртвой" зоне
    motor1.setMode(STOP);
  } else if (potent > right_min) {   // если мы вышли из мёртвой зоны справа
    motor1.setMode(FORWARD);
    int duty = map(potent, right_min, 1023, 0, 1023);
    motor1.setSpeed10bit(duty);
  } else if (potent < left_min) {   // если мы вышли из мёртвой зоны слева
    motor1.setMode(BACKWARD);
    int duty = map(potent, left_min, 0, 0, 1023);
    motor1.setSpeed10bit(duty);
  }

}
