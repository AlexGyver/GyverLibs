#ifndef Parser_h
#define Parser_h
// простой и быстрый парсер строк в отдельные строки и числа

class Parser {
  public:
    Parser (char* data, char newDiv = ',') {
      buf = data;
      div = newDiv;
    }
    ~Parser() {
      clear();
    }
    void clear() {
      if (str) free(str);
    }
    int amount() {
      int i = 0, count = 0;
      while (buf[i++]) if (buf[i] == div) count++;  // подсчёт разделителей
      return ++count;
    }
    int split() {
      int am = amount();            // количество данных
      clear();                      // освобождаем буфер
      str = (char**)malloc(am * sizeof(char*)); // создаём буфер
      str[0] = buf;                 // строка 0
      int i = 0, j = 0;             // счётчики
      while (buf[i]) {              // пока не NULL
        if (buf[i] == div) {        // если разделитель
          buf[i] = '\0';            // меняем на NULL
          str[++j] = buf + i + 1;   // запоминаем начало строки
        }
        i++;
      }
      return am;
    }
    int16_t getInt(int num) {
      return atol(str[num]);
    }
    float getFloat(int num) {
      return atof(str[num]);
    }
    bool equals(int num, const char* comp) {
      return !strcmp(str[num], comp);
    }
    int parseInts(int* data) {
      int count = 0;
      char* offset = buf;
      while (true) {
        data[count++] = atoi(offset);
        offset = strchr(offset, div);
        if (offset) offset++;
        else break;
      }
      return count;
    }
    int parseBytes(byte* data) {
      int count = 0;
      char* offset = buf;
      while (true) {
        data[count++] = atoi(offset);
        offset = strchr(offset, div);
        if (offset) offset++;
        else break;
      }
      return count;
    }

    char* buf = NULL;
    char** str = NULL;

    char* operator [] (uint16_t idx) {
      return str[idx];
    }
    char div;
  private:
};

#endif
