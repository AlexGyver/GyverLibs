#define COEF 0.1
#define LOW_VALUE 15      // к этому значению "остывает" система
#define SIGNAL_COEF 0.15  // сила сигнала

float process(int signal) {
  static uint32_t tmr;
  static float valueSpeed, signalSpeed, value = LOW_VALUE;
  static float valueSpeed_f, signalSpeed_f;

  // сигнал == скорость нагрева
  if (abs(signalSpeed - signal) > 1) {
    if (signalSpeed < signal) signalSpeed += 0.6;   // нагрев быстрее
    if (signalSpeed > signal) signalSpeed -= 0.3;   // чем остывание
  } else {
    signalSpeed = signal;
  }
  signalSpeed_f += (signalSpeed - signalSpeed_f) * 0.1;

  // складываем скорость сигнала и скорость охлаждения
  // скорость охлаждения получаем как разность "температуры" и её нулевого значения
  valueSpeed = signalSpeed_f * SIGNAL_COEF + (LOW_VALUE - value) * COEF;

  // прибавляем скорость (интегрируем)
  value += valueSpeed;  // без задержки
  
  return value;
}
