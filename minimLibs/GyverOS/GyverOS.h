#ifndef GyverOS_h
#define GyverOS_h

//#define OS_BENCH

template < uint16_t SIZE >
class GyverOS {
public:
	void tick() {    
		#ifdef OS_BENCH
		if (loadP > 0) {
			if (millis() - loadTmr >= loadP) {
				loadTmr += loadP;
				load = (loadSum / 10ul) / loadP;  // (loadSum / 1000) / loadP * 100
				loadSum = 0;
			}
			us = micros();
		}
		#endif

		bool flag = 0;    
		for (int i = 0; i < SIZE; i++) {
			if (callback[i] && states[i]) {   
				if (prds[i] == 0 || millis() - tmrs[i] >= prds[i]) {
					tmrs[i] += prds[i];
					#ifdef OS_BENCH
					if (loopTimeNum == i) loopTime = micros();
					#endif
					callback[i]();
					#ifdef OS_BENCH
					if (loopTimeNum == i) loopTime = micros() - loopTime;
					#endif
					flag = 1;
				}
			}
		}
		#ifdef OS_BENCH
		if (loadP > 0 && flag) loadSum += micros() - us;
		#endif
	}

	void attach(int num, void (*handler)(), uint32_t prd = 0) {
		if (num >= SIZE) return;
		callback[num] = *handler;    
		prds[num] = prd;
		start(num);
	}
	void detach(int num) {
		if (num >= SIZE) return;
		callback[num] = NULL;
	}
	void setPeriod(int num, uint32_t prd) {
		if (num >= SIZE) return;
		prds[num] = prd;
	}
	void start(int num) {
		if (num >= SIZE) return;
		states[num] = 1;
		tmrs[num] = millis();
	}
	void stop(int num) {
		if (num >= SIZE) return;
		states[num] = 0;
	}
	void exec(int num) {
		if (num >= SIZE) return;
		callback[num]();
	}
	uint32_t getLeft() {
		uint32_t nearPrd = UINT32_MAX;
		uint32_t tm;
		for (int i = 0; i < SIZE; i++) {
			if (callback[i] && states[i]) {
				uint32_t tm = prds[i] + tmrs[i] - millis();
				nearPrd = min(nearPrd, tm);     
			}
		}
		return nearPrd;
	}

	void attachLoopTime(int num) {
		#ifdef OS_BENCH
		loopTimeNum = num;
		#endif
	}
	void detachLoopTime() {
		#ifdef OS_BENCH
		loopTimeNum = -1;
		#endif
	}
	uint32_t getLoopTime() {
		#ifdef OS_BENCH
		return loopTime;
		#else
		return 0;
		#endif
	}
	int getLoad() {
		#ifdef OS_BENCH
		return load;
		#else
		return 0;
		#endif
	}    
	void enableLoad(uint32_t loadp = 1000) {
		#ifdef OS_BENCH
		loadP = loadp;
		loadTmr = millis();
		loadSum = 0;
		#endif
	}
	void disableLoad() {
		#ifdef OS_BENCH
		loadP = 0;
		#endif
	}

private:
	void (*callback[SIZE])() = {};
	uint32_t tmrs[SIZE], prds[SIZE], loopTime = 0;
	bool states[SIZE];
#ifdef OS_BENCH
	uint32_t loadP = 0, us = 0, loadTmr = 0, loadSum = 0;
	int load = 0, loopTimeNum = -1;
#endif
};
#endif
