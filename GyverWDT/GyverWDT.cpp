#include <GyverWDT.h>


/* указатель на функцию прерывания */
void (*isr_wdt)();

/* непосредственно прерывание ватчдога */
ISR(WDT_vect) {
  (*isr_wdt)();
}

/* сброс счетчика watchdog в 0 */
void watchdog_reset(void) {
  asm volatile("wdr"); // ассемблерная вставка "watchdog reset"
}

/* Полное отключение watchdog'a */
void watchdog_disable(void) {
  WDTCSR |= (1 << WDCE) | (1 << WDE); // разрешение на вмешательство
  WDTCSR = 0; // обнуление регистра wdt
}


/* Вариант функции для работы ватчдога с прерываниями при таймауте */
void watchdog_enable(uint8_t  mode , uint8_t prescaler , void (*isr)()) {
  isr_wdt = *isr;  //указатель на функцию
  switch (mode) {
    case 0: // interrupt
      if (prescaler > 7) { // если больше 7
        prescaler -= 8; // отняли старший бит
        WDTCSR |= (1 << WDCE) | (1 << WDE); // разрешили вмешательство
        WDTCSR = 0b01100000 | prescaler; // вкл режим int , установили старший бит и задвинули прескалер
      }
      else { // если меньше 7
        WDTCSR |= (1 << WDCE) | (1 << WDE); // разрешили вмешательство
        WDTCSR = 0b01000000 | prescaler; // вкл режим int , просто задвинули прескалер
      }
      break;
    case 1: // interrupt+reset
      if (prescaler > 7) { // если больше 7
        prescaler -= 8; // отняли старший бит
        WDTCSR |= (1 << WDCE) | (1 << WDE); // разрешили вмешательство
        WDTCSR = 0b01101000 | prescaler; // вкл режим int+rst , установили старший бит и задвинули прескалер
      }
      else { // если меньше 7
        WDTCSR |= (1 << WDCE) | (1 << WDE); // разрешили вмешательство
        WDTCSR = 0b01001000 | prescaler; // вкл режим int+rst , просто задвинули прескалер
      }
      break;
  }
}

/* вариант для работы без прерываний (только сброс) */
void watchdog_enable(uint8_t prescaler) {
  if (prescaler > 7) { // если больше 7
    prescaler -= 8; // отняли старший бит
    WDTCSR |= (1 << WDCE) | (1 << WDE); // разрешили вмешательство
    WDTCSR = 0b00101000 | prescaler; // вкл режим rst , установили старший бит и задвинули прескалер
  }
  else { // если меньше 7
    WDTCSR |= (1 << WDCE) | (1 << WDE);  // разрешили вмешательство
    WDTCSR = 0b00001000 | prescaler; // вкл режим rst , просто задвинули прескалер
  }
}