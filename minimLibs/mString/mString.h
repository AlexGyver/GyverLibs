// РЕЗКАЯ КАК ПОНОС И ЛЁГКАЯ КАК ПЁРЫШКО ЗАМЕНА STRING

#ifndef mString_h
#define mString_h

#include <Arduino.h>

// закодил, но тут вроде не используется
char* mUtoa(uint32_t value, char *buffer, bool clear = 1);
char* mLtoa(int32_t value, char *buffer, bool clear = 1);
char* mFtoa(double value, int8_t decimals, char *buffer);

char* mUtoa(uint32_t value, char *buffer, bool clear) {
  buffer += 11;
  if (clear) *--buffer = 0;
  do {
    *--buffer = value % 10 + '0';
    value /= 10;
  } while (value != 0);
  return buffer;
}

char* mLtoa(int32_t value, char *buffer, bool clear) {
  bool minus = value < 0;
  if (minus) value = -value;
  buffer = mUtoa(value, buffer, clear);
  if (minus) *--buffer = '-';
  return buffer;
}

char* mFtoa(double value, int8_t decimals, char *buffer) {
  int32_t mant = (int32_t)value;
  value -= mant;
  uint32_t exp = 1;
  while (decimals--) exp *= 10;
  exp *= (float)value;
  buffer = ltoa(mant, buffer, DEC);
  byte len = strlen(buffer);
  *(buffer + len++) = '.';
  ltoa(exp, buffer + len++, DEC);
  return buffer;
}

template < uint16_t SIZE >
class mString {
  public:
    char buf[SIZE] = "";
    uint16_t length() {
      return strlen(buf);
    }
    void clear() {
      buf[0] = NULL;
    }

    // add
    mString& add(const char c) {
      int len = length();
      if (len + 1 >= SIZE) return *this;
      buf[len++] = c;
      buf[len] = NULL;
      return *this;
    }
    mString& add(const char* data) {
      if (length() + strlen(data) >= SIZE) return *this;
      strcat(buf, data);
      return *this;
    }
    mString& add(const __FlashStringHelper *data) {
      PGM_P p = reinterpret_cast<PGM_P>(data);
      if (length() + strlen_P(p) >= SIZE) return *this;
      strcpy_P(buf + length(), p);
      return *this;
    }
    mString& add(uint32_t value) {
      char vBuf[11];
      utoa(value, vBuf, DEC);
      return add(vBuf);
    }
    mString& add(uint16_t value) {
      return add((uint32_t)value);
    }
    mString& add(uint8_t value) {
      return add((uint32_t)value);
    }
    mString& add(int32_t value) {
      char vBuf[11];
      ltoa(value, vBuf, DEC);
      return add(vBuf);
    }
    mString& add(int16_t value) {
      return add((int32_t)value);
    }
    mString& add(int8_t value) {
      return add((int32_t)value);
    }
    mString& add(double value, int8_t dec = 2) {
      char vBuf[20];
      mFtoa(value, dec, vBuf);
      return add(vBuf);
    }
    mString& add(mString data) {
      return add(data.buf);
    }
    mString& add(String data) {
      return add(data.c_str());
    }

    // add +=
    mString& operator += (const char c) {
      return add(c);
    }
    mString& operator += (const char* data) {
      return add(data);
    }
    mString& operator += (const __FlashStringHelper *data) {
      return add(data);
    }
    mString& operator += (uint32_t value) {
      return add(value);
    }
    mString& operator += (int32_t value) {
      return add(value);
    }
    mString& operator += (uint16_t value) {
      return add(value);
    }
    mString& operator += (int16_t value) {
      return add(value);
    }
    mString& operator += (uint8_t value) {
      return add(value);
    }
    mString& operator += (int8_t value) {
      return add(value);
    }
    mString& operator += (double value) {
      return add(value);
    }
    mString& operator += (mString data) {
      return add(data.buf);
    }
    mString& operator += (String data) {
      return add(data);
    }

    // +
    mString operator + (const char c) {
      return (*this).add(c);
    }
    mString operator + (const char* data) {
      return (*this).add(data);
    }
    mString operator + (const __FlashStringHelper *data) {
      return (*this).add(data);
    }
    mString operator + (uint32_t value) {
      return (*this).add(value);
    }
    mString operator + (int32_t value) {
      return (*this).add(value);
    }
    mString operator + (uint16_t value) {
      return (*this).add(value);
    }
    mString operator + (int16_t value) {
      return (*this).add(value);
    }
    mString operator + (uint8_t value) {
      return (*this).add(value);
    }
    mString operator + (int8_t value) {
      return (*this).add(value);
    }
    mString operator + (double value) {
      return (*this).add(value);
    }
    mString operator + (mString data) {
      return (*this).add(data);
    }
    mString operator + (String data) {
      return (*this).add(data);
    }

    // assign
    mString& operator = (const char c) {
      clear();
      return add(c);
    }
    mString& operator = (const char* data) {
      clear();
      return add(data);
    }
    mString& operator = (const __FlashStringHelper *data) {
      clear();
      return add(data);
    }
    mString& operator = (uint32_t value) {
      clear();
      return add(value);
    }
    mString& operator = (int32_t value) {
      clear();
      return add(value);
    }
    mString& operator = (uint16_t value) {
      clear();
      return add(value);
    }
    mString& operator = (int16_t value) {
      clear();
      return add(value);
    }
    mString& operator = (uint8_t value) {
      clear();
      return add(value);
    }
    mString& operator = (int8_t value) {
      clear();
      return add(value);
    }
    mString& operator = (double value) {
      clear();
      return add(value);
    }
    mString& operator = (String data) {
      clear();
      return add(data);
    }

    // compare
    bool operator == (const char c) {
      return (buf[0] == c && buf[1] == 0);
    }
    bool operator == (const char* data) {
      return !strcmp(buf, data);
    }
    bool operator == (uint32_t value) {
      char valBuf[11];
      return !strcmp(buf, utoa(value, valBuf, DEC));
    }
    bool operator == (uint16_t value) {
      char valBuf[6];
      return !strcmp(buf, utoa(value, valBuf, DEC));
    }
    bool operator == (uint8_t value) {
      char valBuf[4];
      return !strcmp(buf, utoa(value, valBuf, DEC));
    }
    bool operator == (int32_t value) {
      char valBuf[11];
      return !strcmp(buf, ltoa(value, valBuf, DEC));
    }
    bool operator == (int16_t value) {
      char valBuf[6];
      return !strcmp(buf, ltoa(value, valBuf, DEC));
    }
    bool operator == (int8_t value) {
      char valBuf[4];
      return !strcmp(buf, ltoa(value, valBuf, DEC));
    }
    bool operator == (float value) {
      char valBuf[20];
      return !strcmp(buf, mFtoa(value, 2, valBuf));
    }
    bool operator == (mString data) {
      return !strcmp(buf, data.buf);
    }
    bool operator == (String data) {
      return !strcmp(buf, data.c_str());
    }

    // convert & parse
    char operator [] (uint16_t index) const {
      return buf[index];
    }
    char& operator [] (uint16_t index) {
      return buf[index];
    }
    char charAt(uint16_t index) {
      return buf[index];
    }
    void setCharAt(uint16_t index, char c) {
      buf[index] = c;
    }
    int32_t toInt(uint16_t from = 0) {
      return atol(buf + from);
    }
    uint32_t toUint(uint16_t from = 0) {
      return atol(buf + from);
    }
    float toFloat(uint16_t from = 0) {
      return atof(buf + from);
    }
    const char* c_str() {
      return buf;
    }

    bool startsWith(const char *data, uint16_t offset = 0) {
      return strlen(data) == strspn(buf + offset, data);
    }

    void substring(uint16_t from, uint16_t to, char* arr) {
      char backup = buf[++to];
      buf[to] = NULL;
      strcpy(arr, buf + from);
      buf[to] = backup;
    }
    int split(char** ptrs, char div = ',') {
      int i = 0, j = 1;
      ptrs[0] = buf;
      while (buf[i]) {
        if (buf[i] == div) {
          buf[i] = NULL;
          ptrs[j++] = buf + i + 1;
        }
        i++;
      }
      return j;
    }
    void truncate(uint16_t amount) {
      uint16_t len = length();
      if (amount >= len) clear();
      else buf[len - amount] = NULL;
    }
    void remove(uint16_t index, uint16_t count) {
      uint16_t len = length();
      if (index >= len) return;
      if (count <= 0) return;
      if (count > len - index) {
        count = len - index;
      }
      char *writeTo = buf + index;
      len = len - count;
      strncpy(writeTo, buf + index + count, len - index);
      buf[len] = 0;
    }

    void toLowerCase() {
      if (!length()) return;
      for (char *p = buf; *p; p++) *p = tolower(*p);
    }

    void toUpperCase() {
      if (!length()) return;
      for (char *p = buf; *p; p++) *p = toupper(*p);
    }

    int indexOf(char ch, uint16_t fromIndex = 0) {
      if (fromIndex >= length()) return -1;
      const char* temp = strchr(buf + fromIndex, ch);
      return (temp == NULL) ? -1 : (temp - buf);
    }

    int indexOf(const char* ch, uint16_t fromIndex = 0) {
      if (fromIndex >= length()) return -1;
      const char* temp = strstr(buf + fromIndex, ch);
      return (temp == NULL) ? -1 : (temp - buf);
    }

    int parseBytes(byte* data, int len, char div = ',', char ter = NULL) {
      int b = 0, c = 0;
      data[b] = 0;
      while (true) {
        if (buf[c] == div) {
          b++;
          c++;
          if (b == len) return b;
          data[b] = 0;
          continue;
        }
        if (buf[c] == ter || b == len) return b + 1;
        data[b] *= 10;
        data[b] += buf[c] - '0';
        c++;
      }
    }
    int parseInts(int* data, int len, char div = ',', char ter = NULL) {
      int b = 0, c = 0;
      data[b] = 0;
      while (true) {
        if (buf[c] == div) {
          b++;
          c++;
          if (b == len) return b;
          data[b] = 0;
          continue;
        }
        if (buf[c] == ter || b == len) return b + 1;
        data[b] *= 10;
        data[b] += buf[c] - '0';
        c++;
      }
    }
  private:
};
#endif
