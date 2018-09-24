/*
	Пример управления мотором при помощи драйвера полного моста
	и потенциометра. На всём диапазоне потенциометра:
	от 0 до мёртвой зоны (512 - deadzone) крутим в одну сторону
	в мёртвой зоне не крутим вообще
	от мёртвой зоны (512 + deadzone) до 1023 крутим в другую сторону
	Управление производится хардварным ШИМ пином на настраиваемой частоте
*/

#define DEADZONE 60 	// "мёртвая зона" потенциометра
#define IN1 2
#define IN2 3			// IN2 обязательно должен быть ШИМ пином!!!
#define POT 0     	 	// сюда подключен потенциометр

#include "GyverMotor.h"
GMotor motor1(IN1, IN2);

int left_min = 512 - DEADZONE;   // расчёт границ мёртвой зоны
int right_min = 512 + DEADZONE;  // расчёт границ мёртвой зоны

void setup() {
  motor1.setDirection(REVERSE);
  
  // изменение частоты ШИМа на 3 пине. Таблицу частот смотрите в библиотеке GyverHacks.h"
  // пины 5 и 6   8 bit mode: 1 (62 500 Гц), 2 (7 812 Гц), 3 (976 Гц), 4 (244 Гц), 5 (61 Гц). ВЛИЯЕТ НА РАБОТУ millis() и delay()
  // пины 9 и 10  8 bit   mode: 1 (62 500 Гц), 2 (7 812 Гц), 3 (976 Гц), 4 (244 Гц), 5 (61 Гц). ВЛИЯЕТ НА РАБОТУ servo
  // пины 3 и 11  8 bit mode: 1 (31 250 Гц), 2 (3 906 Гц), 3 (976 Гц), 4 (488 Гц), 5 (244 Гц), 6 (122 Гц), 7 (30 Гц). ВЛИЯЕТ НА РАБОТУ tone()
  PWMfrequency(IN2, 1);
}

void loop() {
  int potent = analogRead(POT);                    // читаем с потенциометра

  if (potent > left_min && potent < right_min) {  // если мы в "мёртвой" зоне
    motor1.setMode(STOP);
  } else if (potent > right_min) {   // если мы вышли из мёртвой зоны справа
    motor1.setMode(FORWARD);
    byte duty = map(potent, right_min, 1023, 0, 255);
    motor1.setSpeed(duty);
  } else if (potent < left_min) {   // если мы вышли из мёртвой зоны слева
    motor1.setMode(BACKWARD);
    byte duty = map(potent, left_min, 0, 0, 255);
    motor1.setSpeed(duty);
  }

}
