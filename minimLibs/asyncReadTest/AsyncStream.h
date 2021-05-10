#ifndef AsyncStream_h
#define AsyncStream_h

template < uint16_t SIZE >
class AsyncStream {
  public:
    AsyncStream(Stream* port, char ter = ';', uint16_t tout = 50) {
      _port = port;
      _tout = tout;
      _ter = ter;
    }
    bool available() {
      if (_port -> available()) {
        if (!_parseF) {
          _parseF = true;
          _count = 0;
          _tmr = millis();
        }
        char ch = _port -> read();
        if (ch == _ter) {
          buf[_count] = '\0';
          _parseF = false;
          return true;
        } else if (_count < SIZE - 1) buf[_count++] = ch;
        _tmr = millis();
      }
      if (_parseF && millis() - _tmr >= _tout) {
        _parseF = false;
        return true;
      }
      return false;
    }
    
    char buf[SIZE];

  private:
    Stream* _port;
    char _ter;
    uint16_t _tout, _count = 0;
    uint32_t _tmr = 0;
    bool _parseF = false;
};

#endif
