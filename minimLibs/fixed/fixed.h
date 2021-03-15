#ifndef fixed_h
#define fixed_h
// реализация вычислений с фиксированной точкой

#define FIX_SHIFT 14
#define FIX_MUL (1L<<FIX_SHIFT)
#define FIX_UINT(x) ((x) << FIX_SHIFT)
#define FIX_INT(x) ((x) * FIX_MUL)
#define FIX_FLOAT(x) ((x) * FIX_MUL)
#define expandFix(x) x.fix / FIX_MUL
#define FIX(x) (int32_t&)x

class fixed {
public:    
	int32_t fix = 0;
	fixed(){}
	//fixed(fixed& val) {fix = val.fix;}
	fixed(uint32_t val) {fix = FIX_UINT(val);}
	fixed(int32_t val) {fix = FIX_INT(val);}
	fixed(uint16_t val) {fix = FIX_UINT(val);}
	fixed(int16_t val) {fix = FIX_INT(val);}
	fixed(uint8_t val) {fix = FIX_UINT(val);}
	fixed(int8_t val) {fix = FIX_INT(val);}
	fixed(double val) {fix = FIX_FLOAT(val);}
	fixed(int32_t val1, int32_t val2) {fix = FIX_INT(val1) / val2;}

	fixed& operator = (uint32_t val) {fix = FIX_UINT(val);}
	fixed& operator = (int32_t val) {fix = FIX_INT(val);}
	fixed& operator = (uint16_t val) {fix = FIX_UINT(val);}
	fixed& operator = (int16_t val) {fix = FIX_INT(val);}
	fixed& operator = (uint8_t val) {fix = FIX_UINT(val);}
	fixed& operator = (int8_t val) {fix = FIX_INT(val);}
	fixed& operator = (double val) {fix = FIX_FLOAT(val);}

	fixed& operator += (fixed val) {fix += val.fix; return *this;}
	fixed& operator += (uint32_t val) {fix += FIX_UINT(val); return *this;}
	fixed& operator += (int32_t val) {fix += FIX_INT(val); return *this;}
	fixed& operator += (uint16_t val) {fix += FIX_UINT(val); return *this;}
	fixed& operator += (int16_t val) {fix += FIX_INT(val); return *this;}
	fixed& operator += (uint8_t val) {fix += FIX_UINT(val); return *this;}
	fixed& operator += (int8_t val) {fix += FIX_INT(val); return *this;}
	fixed& operator += (double val) {fix += FIX_FLOAT(val); return *this;}

	fixed operator + (fixed val) {return fixed(*this) += val;}
	fixed operator + (uint32_t val) {return fixed(*this) += val;}
	fixed operator + (int32_t val) {return fixed(*this) += val;}
	fixed operator + (uint16_t val) {return fixed(*this) += val;}
	fixed operator + (int16_t val) {return fixed(*this) += val;}
	fixed operator + (uint8_t val) {return fixed(*this) += val;}
	fixed operator + (int8_t val) {return fixed(*this) += val;}
	fixed operator + (double val) {return fixed(*this) += val;}

	fixed& operator -= (fixed val) {fix -= val.fix; return *this;}
	fixed& operator -= (uint32_t val) {fix -= FIX_UINT(val); return *this;}
	fixed& operator -= (int32_t val) {fix -= FIX_INT(val); return *this;}
	fixed& operator -= (uint16_t val) {fix -= FIX_UINT(val); return *this;}
	fixed& operator -= (int16_t val) {fix -= FIX_INT(val); return *this;}
	fixed& operator -= (uint8_t val) {fix -= FIX_UINT(val); return *this;}
	fixed& operator -= (int8_t val) {fix -= FIX_INT(val); return *this;}
	fixed& operator -= (double val) {fix -= FIX_FLOAT(val); return *this;}

	fixed operator - (fixed val) {return fixed(*this) -= val;}
	fixed operator - (uint32_t val) {return fixed(*this) -= val;}
	fixed operator - (int32_t val) {return fixed(*this) -= val;}
	fixed operator - (uint16_t val) {return fixed(*this) -= val;}
	fixed operator - (int16_t val) {return fixed(*this) -= val;}
	fixed operator - (uint8_t val) {return fixed(*this) -= val;}
	fixed operator - (int8_t val) {return fixed(*this) -= val;}
	fixed operator - (double val) {return fixed(*this) -= val;}

	fixed& operator *= (fixed val) {fix *= val.fix; return *this;}
	fixed& operator *= (uint32_t val) {fix *= (val); return *this;}
	fixed& operator *= (int32_t val) {fix *= (val); return *this;}
	fixed& operator *= (uint16_t val) {fix *= (val); return *this;}
	fixed& operator *= (int16_t val) {fix *= (val); return *this;}
	fixed& operator *= (uint8_t val) {fix *= (val); return *this;}
	fixed& operator *= (int8_t val) {fix *= (val); return *this;}
	fixed& operator *= (double val) {fix *= (val); return *this;}

	fixed operator * (fixed val) {return fixed(*this) *= val;}
	fixed operator * (uint32_t val) {return fixed(*this) *= val;}
	fixed operator * (int32_t val) {return fixed(*this) *= val;}
	fixed operator * (uint16_t val) {return fixed(*this) *= val;}
	fixed operator * (int16_t val) {return fixed(*this) *= val;}
	fixed operator * (uint8_t val) {return fixed(*this) *= val;}
	fixed operator * (int8_t val) {return fixed(*this) *= val;}
	fixed operator * (double val) {return fixed(*this) *= val;}

	fixed& operator /= (fixed val) {fix /= val.fix; return *this;}
	fixed& operator /= (uint32_t val) {fix /= (val); return *this;}
	fixed& operator /= (int32_t val) {fix /= (val); return *this;}
	fixed& operator /= (uint16_t val) {fix /= (val); return *this;}
	fixed& operator /= (int16_t val) {fix /= (val); return *this;}
	fixed& operator /= (uint8_t val) {fix /= (val); return *this;}
	fixed& operator /= (int8_t val) {fix /= (val); return *this;}
	fixed& operator /= (double val) {fix /= (val); return *this;}

	fixed operator / (fixed val) {return fixed(*this) /= val;}
	fixed operator / (uint32_t val) {return fixed(*this) /= val;}
	fixed operator / (int32_t val) {return fixed(*this) /= val;}
	fixed operator / (uint16_t val) {return fixed(*this) /= val;}
	fixed operator / (int16_t val) {return fixed(*this) /= val;}
	fixed operator / (uint8_t val) {return fixed(*this) /= val;}
	fixed operator / (int8_t val) {return fixed(*this) /= val;}
	fixed operator / (double val) {return fixed(*this) /= val;}

	fixed& operator %= (fixed val) {fix %= val.fix; return *this;}
	fixed& operator %= (uint32_t val) {fix %= FIX_UINT(val); return *this;}
	fixed& operator %= (int32_t val) {fix %= FIX_INT(val); return *this;}
	fixed& operator %= (uint16_t val) {fix %= FIX_UINT(val); return *this;}
	fixed& operator %= (int16_t val) {fix %= FIX_INT(val); return *this;}
	fixed& operator %= (uint8_t val) {fix %= FIX_UINT(val); return *this;}
	fixed& operator %= (int8_t val) {fix %= FIX_INT(val); return *this;}

	fixed operator % (fixed val) {return fixed(*this) %= val;}
	fixed operator % (uint32_t val) {return fixed(*this) %= val;}
	fixed operator % (int32_t val) {return fixed(*this) %= val;}
	fixed operator % (uint16_t val) {return fixed(*this) %= val;}
	fixed operator % (int16_t val) {return fixed(*this) %= val;}
	fixed operator % (uint8_t val) {return fixed(*this) %= val;}
	fixed operator % (int8_t val) {return fixed(*this) %= val;}

	int32_t toInt() {return (fix >= 0) ? (fix >> FIX_SHIFT) : ((int32_t)fix / FIX_MUL);}
	float toFloat() {return (float)fix / FIX_MUL;}  
private:
};

fixed toFix(double val) {fixed buf(val); return buf;}
fixed toFix(uint32_t val) {fixed buf(val); return buf;}
fixed toFix(int32_t val) {fixed buf(val); return buf;}
fixed toFix(uint16_t val) {fixed buf(val); return buf;}
fixed toFix(int16_t val) {fixed buf(val); return buf;}
fixed toFix(uint8_t val) {fixed buf(val); return buf;}
fixed toFix(int8_t val) {fixed buf(val); return buf;}
fixed toFix(int val1, int val2) {fixed buf(val1, val2); return buf;}
#endif