#include "GyverGFX.h"

void GyverGFX::dot(int x, int y, uint8_t fill) {}

GyverGFX::GyverGFX(int x, int y) {
  _maxX = x;
  _maxY = y;
}

void GyverGFX::fastLineH(int y, int x0, int x1, uint8_t fill) {
  GFX_SWAP(x0, x1);
  for (int x = x0; x <= x1; x++) dot(x, y, fill);
}

void GyverGFX::fastLineV(int x, int y0, int y1, uint8_t fill) {
  GFX_SWAP(y0, y1);
  for (int y = y0; y <= y1; y++) dot(x, y, fill);
}

void GyverGFX::line(int x0, int y0, int x1, int y1, uint8_t fill) {
  if (x0 == x1) fastLineV(x0, y0, y1, fill);
  else if (y0 == y1) fastLineH(y0, x0, x1, fill);
  else {
    int sx, sy, e2, err;
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    if (x0 < x1) sx = 1; else sx = -1;
    if (y0 < y1) sy = 1; else sy = -1;
    err = dx - dy;
    for (;;) {
      dot(x0, y0, fill);
      if (x0 == x1 && y0 == y1) return;
      e2 = err << 1;
      if (e2 > -dy) {
        err = err - dy;
        x0 = x0 + sx;
      }
      if (e2 < dx) {
        err = err + dx;
        y0 = y0 + sy;
      }
    }
  }
}

void GyverGFX::rect(int x0, int y0, int x1, int y1, uint8_t fill) {
  if (fill == GFX_STROKE) {
    fastLineH(y0, x0 + 1, x1 - 1);
    fastLineH(y1, x0 + 1, x1 - 1);
    fastLineV(x0, y0, y1);
    fastLineV(x1, y0, y1);
  } else {
    GFX_SWAP(y0, y1);
    for (int y = y0; y <= y1; y++) fastLineH(y, x0, x1, fill);
  }
}

void GyverGFX::roundRect(int x0, int y0, int x1, int y1, uint8_t fill) {
  if (fill == GFX_STROKE) {
    fastLineV(x0, y0 + 2, y1 - 2);
    fastLineV(x1, y0 + 2, y1 - 2);
    fastLineH(y0, x0 + 2, x1 - 2);
    fastLineH(y1, x0 + 2, x1 - 2);
    dot(x0 + 1, y0 + 1);
    dot(x1 - 1, y0 + 1);
    dot(x1 - 1, y1 - 1);
    dot(x0 + 1, y1 - 1);
  } else {
    fastLineV(x0, y0 + 2, y1 - 2, fill);
    fastLineV(x0 + 1, y0 + 1, y1 - 1, fill);
    fastLineV(x1 - 1, y0 + 1, y1 - 1, fill);
    fastLineV(x1, y0 + 2, y1 - 2, fill);
    rect(x0 + 2, y0, x1 - 2, y1, fill);
  }
}

void GyverGFX::circle(int x, int y, int radius, uint8_t fill) {
  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x1 = 0;
  int y1 = radius;
  uint8_t fillLine = (fill == GFX_CLEAR) ? 0 : 1;
  dot(x, y + radius, fillLine);
  dot(x, y - radius, fillLine);
  dot(x + radius, y, fillLine);
  dot(x - radius, y, fillLine);
  if (fill != GFX_STROKE) fastLineV(x, y - radius, y + radius - 1, fillLine);
  while (x1 < y1) {
    if (f >= 0) {
      y1--;
      ddF_y += 2;
      f += ddF_y;
    }
    x1++;
    ddF_x += 2;
    f += ddF_x;
    if (fill == GFX_STROKE) {
      dot(x + x1, y + y1);
      dot(x - x1, y + y1);
      dot(x + x1, y - y1);
      dot(x - x1, y - y1);
      dot(x + y1, y + x1);
      dot(x - y1, y + x1);
      dot(x + y1, y - x1);
      dot(x - y1, y - x1);
    } else {
      fastLineV(x + x1, y - y1, y + y1, fillLine);
      fastLineV(x - x1, y - y1, y + y1, fillLine);
      fastLineV(x + y1, y - x1, y + x1, fillLine);
      fastLineV(x - y1, y - x1, y + x1, fillLine);
    }
  }
}

void GyverGFX::bezier(uint8_t* arr, uint8_t size, uint8_t dense, uint8_t fill) {
  int a[size * 2];
  for (int i = 0; i < (1 << dense); i++) {
    for (int j = 0; j < size * 2; j++) a[j] = arr[j] << 3;
    for (int j = (size - 1) * 2 - 1; j > 0; j -= 2)
      for (int k = 0; k <= j; k++)
        a[k] = a[k] + (((a[k + 2] - a[k]) * i) >> dense);
    dot(a[0] >> 3, a[1] >> 3, fill);
  }
}

void GyverGFX::bezier16(int* arr, uint8_t size, uint8_t dense, uint8_t fill) {
  int a[size * 2];
  for (int i = 0; i < (1 << dense); i++) {
    for (int j = 0; j < size * 2; j++) a[j] = arr[j];
    for (int j = (size - 1) * 2 - 1; j > 0; j -= 2)
      for (int k = 0; k <= j; k++)
        a[k] = a[k] + (((a[k + 2] - a[k]) * i) >> dense);
    dot(a[0], a[1], fill);
  }
}

void GyverGFX::drawBitmap(int x, int y, const uint8_t *frame, int width, int height, uint8_t invert, byte mode) {
  byte bytes = width >> 3;
  byte left = width & 0b111;
  if (left) bytes++;

  for (int yy = 0; yy < height; yy++) {
    for (int xx = 0; xx < (width >> 3); xx++) {
      byte thisByte = pgm_read_word(&(frame[xx + yy * bytes])) ^ invert;
      for (byte k = 0; k < 8; k++) {
        byte val = thisByte & 0b10000000;
        if (val || mode) dot((xx << 3) + k + x, yy + y, val);
        thisByte <<= 1;
      }
    }
    if (left) {
      byte thisByte = pgm_read_byte(&(frame[(width >> 3) + yy * bytes])) ^ invert;
      for (byte k = 0; k < left; k++) {
        byte val = thisByte & 0b10000000;
        if (val || mode) dot(((width >> 3) << 3) + k + x, yy + y, val);
        thisByte <<= 1;
      }
    }
  }
}

void GyverGFX::setCursor(int x, int y) {
  _x = x;
  _y = y;
}
void GyverGFX::setScale(uint8_t scale) {
  scale = constrain(scale, 1, 4);
  _scaleX = scale;
  _scaleY = scale * 8;
}
void GyverGFX::invertText(bool inv) {
  _invert = inv;
}
void GyverGFX::autoPrintln(bool mode) {
  _println = mode;
}
void GyverGFX::textDisplayMode(bool mode) {
  _mode = mode;
}
size_t GyverGFX::write(uint8_t data) {
  // переносы и пределы
  bool newPos = false;
  if (data == '\r') { // получен возврат каретки
    _x = 0;
    newPos = true;
    data = 0;
  }
  if (data == '\n') { // получен перевод строки
    _y += _scaleY;
    newPos = true;
    data = 0;
  }
  if (_println && (_x + 6 * _scaleX) >= _maxX) {
    _x = 0;  // строка переполненена, перевод и возврат
    _y += _scaleY;
    newPos = true;
  }
  if (newPos) setCursor(_x, _y);									  // переставляем курсор
  //if (_y + _scaleY >= _maxY) data = 0;					  // дисплей переполнен
  if (_println && data == ' ' && _x == 0) data = 0; // первый пробел

  // фикс русских букв и некоторых символов
  if (data > 127) {
    uint8_t thisData = data;
    // data = 0 - флаг на пропуск
    if (data > 191) data = 0;
    else if (_lastChar == 209 && data == 145) data = 192; // ё кастомная
    else if (_lastChar == 208 && data == 129) data = 149;	// Е вместо Ё
    else if (_lastChar == 226 && data == 128) data = 0;		// тире вместо длинного тире (начало)
    else if (_lastChar == 128 && data == 148) data = 45;	// тире вместо длинного тире
    _lastChar = thisData;
  }
  if (data == 0) return 1;
  // если тут не вылетели - печатаем символ

  int newX = _x + _scaleX * 6;
  if (newX < 0 || _x > _maxX) {	// пропускаем вывод "за экраном"
    _x = newX;
  } else {
    for (uint8_t col = 0; col < 6; col++) {		// 6 стобиков буквы
      uint8_t bits = getFont(data, col);			// получаем байт
      byte inv = 0;
      if (_invert) bits = ~bits;
      if (_scaleX == 1) {									    // если масштаб 1
        if (_x >= 0 && _x < _maxX) {					// внутри дисплея
          for (int y = 0; y < 8; y++) {
            bool bit = bitRead(bits, y);
            if (bit || _mode) dot(_x, _y + y, bit);
          }
        }
        _x++;
      } else {											// масштаб 2, 3 или 4 - растягиваем шрифт
        long newData = 0;								// буфер
        for (uint8_t i = 0, count = 0; i < 8; i++)
          for (uint8_t j = 0; j < _scaleX; j++, count++)
            bitWrite(newData, count, bitRead(bits, i));		// пакуем растянутый шрифт

        for (uint8_t i = 0; i < _scaleX; i++)
          for (uint8_t j = 0; j < _scaleY; j++) {
            bool bit = bitRead(newData, j);
            if (bit || _mode) dot(_x + i, _y + j, bit);
          }
        _x += _scaleX;
      }
    }
  }
  return 1;
}

uint8_t GyverGFX::getFont(uint8_t font, uint8_t row) {
  if (row > 4) return 0;
  font = font - '0' + 16;   // перевод код символа из таблицы ASCII
  if (font <= 95) {
    return pgm_read_byte(&(charMap[font][row]));    // для английских букв и символов
  } else if (font >= 96 && font <= 111) {         // и пизд*ц для русских
    return pgm_read_byte(&(charMap[font + 47][row]));
  } else if (font <= 159) {
    return pgm_read_byte(&(charMap[font - 17][row]));
  } else {
    return pgm_read_byte(&(charMap[font - 1][row]));  // для кастомных (ё)
  }
}
