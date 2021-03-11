#ifndef Hamming_h
#define Hamming_h
template <byte HAM_SIZE = 5>
class Hamming {
public:
	template <typename T>
	bool pack(T &data) {
		// 0. Считаем и создаём буфер
		int size = sizeof(T);                               // байтов даты
		byte signif = chunkSizeB - (HAM_SIZE + 1);          // битов даты на чанк
		chunkAmount = (size * 8 + signif - 1) / signif;     // колво чанков (целоч. деление)
		bytes = chunkAmount * chunkSize;                    // размер буфера, байт
		if (buffer != NULL) free(buffer);                   // освобождаем старый
		buffer = (byte*)malloc(bytes);                      // выделяем
		if (buffer == NULL) return 0;                       // не удалось создать
		byte buf[bytes];                                    // ещё буфер
		memset(buf, 0, bytes);                              // чисти чисти
		memset(buffer, 0, bytes);                           // чисти чисти
		uint8_t *ptr = (const uint8_t*) &data;              // для чтения даты
		int ptrCount = 0;

		for (int chunk = 0; chunk < chunkAmount; chunk++) {   // каждый чанк
			// 1. Заполняем дату, минуя ячейки Хэмминга (0,1,2,4,8...)
			for (byte i = 0; i < chunkSizeB; i++) {
				if ((i & (i - 1)) != 0) {   // проверка на степень двойки
					write(buf, chunk * chunkSizeB + i, read(ptr, ptrCount++));  // переписываем побитно
				}
			}

			// 2. Считаем и пишем parity для зон Хэмминга
			uint8_t parityH = 0;
			for (uint8_t i = 0; i < chunkSizeB; i++) {
				for (uint8_t j = 0; j < HAM_SIZE; j++) {
					// если это ячейка хэмминга и бит стоит, инвертируем текущий parity
					if ((i & (1 << j)) && read(buf, chunk * chunkSizeB + i)) parityH ^= (1 << j);
				}
			}
			for (uint8_t i = 0; i < HAM_SIZE; i++) {
				write(buf, chunk * chunkSizeB + (1 << i), (parityH >> i) & 1); // переписываем parity ячеек хэмминга
			}

			// 3. Считаем и пишем общий parity
			uint8_t count = 0;
			for (uint8_t i = 1; i < chunkSizeB; i++) {
				if (read(buf, chunk * chunkSizeB + i)) count++; // считаем
			}
			write(buf, chunk * chunkSizeB, count & 1);        // пишем
		}

		// 4. Перемешиваем
		for (uint8_t i = 0, k = 0; i < chunkSizeB; i++) {
			for (uint8_t j = 0; j < chunkAmount; j++) {
				write(buffer, k++, read(buf, i + j * chunkSizeB));
			}
		}
		return 1;
	}

	int unpack(byte* data, int size) {
		// 0. Считаем и создаём буфер
		byte signif = chunkSizeB - (HAM_SIZE + 1);    // битов даты на чанк
		if (size & chunkSize != 0) return 0;          // не кратно размеру чанка
		chunkAmount = size / chunkSize;               // колво чанков
		bytes = chunkAmount * signif / 8;             // размер буфера, байт (округл. вниз)
		if (buffer != NULL) free(buffer);             // чисти старый
		buffer = (byte*)malloc(bytes);                // выделяем
		if (buffer == NULL) return 0;                 // не удалось создать
		memset(buffer, 0, bytes);                     // чисти чисти
		byte buf[size];
		int ptrCount = 0;
		stat = 0;

		// 1. Разбираем мешанину обратно
		for (uint8_t i = 0, k = 0; i < chunkSizeB; i++) {
			for (uint8_t j = 0; j < chunkAmount; j++) {
				write(buf, i + j * chunkSizeB, read(data, k++));
			}
		}

		for (int chunk = 0; chunk < chunkAmount; chunk++) {   // каждый чанк
			// 2. Получаем хэш ошибки и общий parity
			uint8_t sum = 0, count = 0;
			for (uint8_t i = 0; i < chunkSizeB; i++) {
				if (read(buf, chunk * chunkSizeB + i)) {
					sum ^= i;
					if (i > 0) count++;
				}
			}

			// 3. Анализируем результат
			if (sum != 0) {                           // 1+ err
				if (read(buf, chunk * chunkSizeB) == (count & 1)) stat = max(stat, 2);  // 2 err
				else toggle(buf, chunk * chunkSizeB + sum);         // fix err
				stat = max(stat, 1);
			} else {
				if (read(buf, chunk * chunkSizeB) != (count & 1)) stat = max(stat, 3);  // parity err
			}

			// 4. Собираем дату из ячеек Хэмминга
			for (byte i = 0; i < chunkSizeB; i++) {
				if ((i & (i - 1)) != 0) {   // проверка на степень двойки
					write(buffer, ptrCount++, read(buf, chunk * chunkSizeB + i)); // переписываем побитно
				}
			}
		}
		return stat;
	}

	// 0 всё ок, 1 исправлены ошибки, 2 и 3 - есть неисправленные ошибки
	byte status() {
		return stat;
	}
	
	byte length() {
		return bytes;
	}
	
	~Hamming() {
		free(buffer);
	}
	
	void stop() {
		free(buffer);
	}
	
	byte *buffer = NULL;

private:
	void set(byte* buf, int num) {
		bitSet(buf[num >> 3], num & 0b111);
	}
	void clear(byte* buf, int num) {
		bitClear(buf[num >> 3], num & 0b111);
	}
	void write(byte* buf, int num, bool state) {
		state ? set(buf, num) : clear(buf, num);
	}
	bool read(byte* buf, int num) {
		return bitRead(buf[num >> 3], num & 0b111);
	}
	void toggle(byte* buf, int num) {
		read(buf, num) ? clear(buf, num) : set(buf, num);
	}
	byte stat;
	int bytes = 0;
	int chunkAmount = 0;
	const byte chunkSizeB = (1 << HAM_SIZE);        // вес чанка в битах
	const byte chunkSize = (1 << HAM_SIZE) >> 3;    // вес чанка в байтах
};
#endif