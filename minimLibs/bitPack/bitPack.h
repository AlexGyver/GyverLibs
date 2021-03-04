#ifndef bitPack_h
#define bitPack_h
template <byte AMOUNT>
class BitPack {
  public:
    void set(byte num) {
      bitSet(pack[(num) >> 3], (num) & 0b111);
    }
    void clear(byte num) {
      bitClear(pack[(num) >> 3], (num) & 0b111);
    }
    void toggle(byte num) {
      read(num) ? clear(pack, num) : set(pack, num);
    }
    void write(byte num, bool state) {
      state ? set(pack, num) : clear(pack, num);
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
    byte pack[(AMOUNT >> 3) + 1];
    byte buf;
};
#endif
