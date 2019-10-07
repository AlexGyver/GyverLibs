/*
  Как перевести скетч с FastLED на microLED:
  0) Задефайнить REPLACE_FASTLED
  1) Задефайнить настройки microLED
  2) Убрать подключение FastLED
  3) Подключить библиотеку microLED  
  4) Заменить CRGB на LEDdata при создании буфера
  5) Создать объект ленты microLED
  6) задефайнить FastLED на название своего объекта
  7) Убрать FastLED.addLeds из setup()
*/

#define NUM_LEDS 8  // кол-во диодов
#define LED_PIN 8   // пин подключения

#define REPLACE_FASTLED // пункт 0
#define COLOR_DEBTH 3   // пункт 1

//#include "FastLED.h"  // пункт 2
#include "microLED.h"   // пункт 3

//CRGB leds[NUM_LEDS];
LEDdata leds[NUM_LEDS]; // пункт 4

microLED strip(leds, NUM_LEDS, LED_PIN);  // пункт 5
#define FastLED strip   // пункт 6

void setup() {
  // пункт 7
  //FastLED.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(50);
  pinMode(13, OUTPUT);
}

void loop() {
  static byte counter = 0;
  for (int i = 0; i < NUM_LEDS; i++ ) {
	// заливка радугой
    leds[i] = CHSV(counter + i * (255 / NUM_LEDS), 255, 255);
  }
  counter++;        // counter меняется от 0 до 255 (тип данных byte)
  FastLED.show();
  delay(5);         // скорость движения радуги
}