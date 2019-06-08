#define SMOOTH_K 0.25     // коэффициент сглаживания
#define TIME_CHANGE 200   // период изменения огня
#define MIN_BRIGHT 30     // мин яркость (красный)
#define MAX_BRIGHT 255    // макс яркость (жёлтый)

#include <GyverRGB.h>
GRGB diode(5, 6, 9);  // куда подключены цвета (R, G, B)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  randomSeed(analogRead(0));
  diode.setDirection(REVERSE);
}

float k = 0.25;
int val, rndVal;
uint32_t prevTime;

void loop() {
  if (millis() - prevTime > TIME_CHANGE) {
    rndVal = random(0, 13) * 10;
    prevTime = millis();
  }
  val = val * (1 - SMOOTH_K) + rndVal * SMOOTH_K;
  diode.colorWheel(val);
  diode.setBrightness(map(val, 0, 120, MIN_BRIGHT, MAX_BRIGHT));
  Serial.println(val);
  delay(40);
}
