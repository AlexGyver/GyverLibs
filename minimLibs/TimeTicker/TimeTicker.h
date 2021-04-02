class TimeTicker {
  public:
    byte sec = 0;
    byte min = 0;
    byte hour = 0;
    byte day = 0;
    int ms = 0;
    uint32_t weekMs = 0;
    uint32_t weekS = 0;

    int getMs() {
      return (tmr - millis());
    }
    void setMs(int ms) {
      tmr = millis() + ms;
    }
    uint32_t getWeekS() {
      return day * 86400ul + hour * 3600ul + min * 60 + sec;
    }
    bool newSec() {
      if (prevSec != sec) {
        prevSec = sec;
        return true;
      }
      return false;
    }
    bool newMin() {
      if (prevMin != min) {
        prevMin = min;
        return true;
      }
      return false;
    }

    void tick() {
      ms = millis() - tmr;
      if (ms >= 1000) {
        tmr += 1000;
        if (++sec >= 60) {
          sec = 0;
          if (++min >= 60) {
            min = 0;
            if (++hour >= 24) {
              hour = 0;
              if (++day >= 7) {
                day = 0;
              }
            }
          }
        }
      }
      weekMs = getWeekS() * 1000ul + millis() - tmr;
    }
  private:
    uint32_t tmr;
    byte prevSec = 0;
    byte prevMin = 0;
};
