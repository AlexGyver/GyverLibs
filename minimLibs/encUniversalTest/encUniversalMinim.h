// мини-класс для работы с энкодером, версия 1.0

class encMinim
{
  public:
    encMinim(uint8_t clk, uint8_t dt, uint8_t sw, boolean dir, boolean type);
    void tick();
    boolean isClick();
    boolean isTurn();
    boolean isRight();
    boolean isLeft();
    boolean isRightH();
    boolean isLeftH();

  private:
    byte _clk, _dt, _sw;
    boolean _type = false;
    boolean _state, _lastState, _turnFlag, _swState, _swFlag, _turnState;
    byte _encState;
    uint32_t _debTimer;
    // 0 - ничего, 1 - лево, 2 - право, 3 - правоНажат, 4 - левоНажат
};

encMinim::encMinim(uint8_t clk, uint8_t dt, uint8_t sw, boolean dir, boolean type) {
  if (dir) {
    _clk = clk;
    _dt = dt;
  } else {
    _clk = dt;
    _dt = clk;
  }
  _sw = sw;
  _type = type;
  pinMode (_clk, INPUT);
  pinMode (_dt, INPUT);
  pinMode (_sw, INPUT_PULLUP);
  _lastState = digitalRead(_clk);
}

void encMinim::tick() {
  _encState = 0;
  _state = digitalRead(_clk);
  _swState = digitalRead(_sw);

  if (_state != _lastState) {
    _turnState = true;
    _turnFlag = !_turnFlag;
    if (_turnFlag || !_type) {
      if (digitalRead(_dt) != _lastState) {
        if (_swState) _encState = 1;
        else _encState = 3;
      } else {
        if (_swState) _encState = 2;
        else _encState = 4;
      }
    }
    _lastState = _state;
  }

  if (!_swState && !_swFlag && millis() - _debTimer > 80) {
    _debTimer = millis();
    _swFlag = true;
    _turnState = false;
  }
  if (_swState && _swFlag && millis() - _debTimer > 80) {
    _debTimer = millis();
    _swFlag = false;
    if (!_turnState) _encState = 5;
  }
}
boolean encMinim::isTurn() {
  if (_encState > 0 && _encState < 5) {
    return true;
  } else return false;
}
boolean encMinim::isRight() {
  if (_encState == 1) {
    _encState = 0;
    return true;
  } else return false;
}
boolean encMinim::isLeft() {
  if (_encState == 2) {
    _encState = 0;
    return true;
  } else return false;
}
boolean encMinim::isRightH() {
  if (_encState == 3) {
    _encState = 0;
    return true;
  } else return false;
}
boolean encMinim::isLeftH() {
  if (_encState == 4) {
    _encState = 0;
    return true;
  } else return false;
}
boolean encMinim::isClick() {
  if (_encState == 5) {
    _encState = 0;
    return true;
  } else return false;
}
