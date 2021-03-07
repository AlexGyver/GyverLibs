#ifndef UnixDate_h
#define UnixDate_h
// unix time to date
// based on https://www.oryx-embedded.com/doc/date__time_8c_source.html
class UnixDate {
public:
	UnixDate (uint32_t t, int8_t gmt) {
		uint32_t a;
		uint32_t b;
		uint32_t c;
		uint32_t d;
		uint32_t e;
		uint32_t f;
		int h;
		int j;
		int k;

		t += gmt * 3600;
		seconds = t % 60ul;
		t /= 60;
		minutes = t % 60ul;
		t /= 60;
		hours = t % 24ul;
		t /= 24;

		a = (uint32_t) ((4ul * t + 102032) / 146097 + 15);
		b = (uint32_t) (t + 2442113 + a - (a / 4));
		c = (20 * b - 2442) / 7305;
		d = b - 365 * c - (c / 4);
		e = d * 1000 / 30601;
		f = d - e * 30 - e * 601 / 1000;

		// январь и февраль как 13 и 14 месяцы
		if (e <= 13) {
			c -= 4716;
			e -= 1;
		} else {
			c -= 4715;
			e -= 13;
		}

		year = c;
		month = e;
		day = f;

		if (e <= 2)	{
			e += 12;
			c -= 1;
		}
		j = c / 100;
		k = c % 100;	
		h = f + (26 * (e + 1) / 10) + k + (k / 4) + (5 * j) + (j / 4);  // Уравнение Зеллера
		dayOfWeek = ((h + 5) % 7) + 1;
	}
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t dayOfWeek;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
private:
};
#endif