#define TIMEOUT 100     // таймаут в миллисекундах на отработку неправильно посланных данных
boolean _recievedFlag;
boolean _getStarted;
byte _index;
String _string_convert = "";

void parsingStream(int *incData) {
  if (Serial.available() > 0) {
    char incomingByte = Serial.read();        // обязательно ЧИТАЕМ входящий символ
    if (_getStarted) {                         // если приняли начальный символ (парсинг разрешён)
      if (incomingByte != ' ' && incomingByte != ';') {   // если это не пробел И не конец
        _string_convert += incomingByte;       // складываем в строку
      } else {                                // если это пробел или ; конец пакета
        incData[_index] = _string_convert.toInt();  // преобразуем строку в int и кладём в массив
        _string_convert = "";                  // очищаем строку
        _index++;                              // переходим к парсингу следующего элемента массива
      }
    }
    if (incomingByte == '$') {                // если это $
      _getStarted = true;                      // поднимаем флаг, что можно парсить
      _index = 0;                              // сбрасываем индекс
      _string_convert = "";                    // очищаем строку
    }
    if (incomingByte == ';') {                // если таки приняли ; - конец парсинга
      _getStarted = false;                     // сброс
      _recievedFlag = true;                    // флаг на принятие
    }
  }
}

void sendPacket(int *intData, uint8_t size) {
	byte n = size / sizeof(intData[0]);
	Serial.print('$');
	for (byte i = 0; i < n; i++) {  // выводим элементы массива
      Serial.print(intData[i]);
	  if (i < (n - 1)) Serial.print(" ");
    }
	Serial.println(';');
}

boolean dataReady() {
	if (_recievedFlag) {
		_recievedFlag = false;
		return true;
	} else {
		return false;
	}
}