#ifndef bitPack_h
#define bitPack_h
template <byte AMOUNT>
class BitPack {
  public:
	BitPack() {
	  clearAll();
	}
    void set(byte num) {
      bitSet(pack[(num) >> 3], (num) & 0b111);
    }
    void clear(byte num) {
      bitClear(pack[(num) >> 3], (num) & 0b111);
    }
    void toggle(byte num) {
      read(num) ? clear(num) : set(num);
    }
    void write(byte num, bool state) {
      state ? set(num) : clear(num);
    }
    bool read(byte num) {
      return bitRead(pack[(num) >> 3], (num) & 0b111);
    }
    void setAll() {
      memset(pack, 255, sizeof(pack));
    }
    void clearAll() {
      memset(pack, 0, sizeof(pack));
    }
    byte operator[] (byte num) const {
      return read(num);
    }
    byte& operator [] (byte num) {
      buf = (num) & 0b111;
      return pack[(num) >> 3];
    }
    BitPack& operator = (bool val) {
      val ? bitSet(*this, buf) : bitClear(*this, buf);
    }

  private:
    byte pack[(AMOUNT + 8 - 1) >> 3];
    byte buf;
};
#endif
