/************************************************************************************
* Developed for AlexGyver https://github.com/AlexGyver/  by Egor 'Nich1con' Zaharov *
* Distributed under a free license indicating the source                            *
* BOSCH BME280 [I2C] Arduino library                                                * 
* V1.2 from 02.03.2020                                                              *
************************************************************************************/

#ifndef GyverBME280_h
#define GyverBME280_h

#define NORMAL_MODE	0x03
#define FORCED_MODE	0x02

#define STANDBY_500US  0x00
#define STANDBY_10MS   0x06
#define STANDBY_20MS   0x07
#define STANDBY_6250US 0x01
#define STANDBY_125MS  0x02
#define STANDBY_250MS  0x03
#define STANDBY_500MS  0x04
#define STANDBY_1000MS 0x05

#define MODULE_DISABLE		  0x00
#define OVERSAMPLING_1        0x01
#define OVERSAMPLING_2        0x02
#define OVERSAMPLING_4        0x03
#define OVERSAMPLING_8        0x04
#define OVERSAMPLING_16       0x05

#define FILTER_DISABLE  0x00
#define FILTER_COEF_2   0x01
#define FILTER_COEF_4   0x02
#define FILTER_COEF_8   0x03
#define FILTER_COEF_16  0x04

#include <Arduino.h>
#include <microWire.h>
// #include <Wire.h> // For other platforms


// ========================================================= CLASS ========================================================= //

class GyverBME280 {

public:

	GyverBME280();								// Create an object of class BME280
	bool begin(void);							// Initialize sensor with standart 0x76 address	
	bool begin(uint8_t address);				// Initialize sensor with not standart 0x76 address	
	bool isMeasuring(void);						// Returns 'true' while the measurement is in progress					
	float readPressure(void);					// Read and calculate atmospheric pressure [float , Pa]
	float readHumidity(void);					// Read and calculate air humidity [float , %]
	void oneMeasurement(void);					// Make one measurement and go back to sleep [FORCED_MODE only]
	void setMode(uint8_t mode);
	float readTemperature(void);				// Read and calculate air temperature [float , *C]
	void setFilter(uint8_t mode);				// Adjust the filter ratio other than the standard one [before begin()]
	void setStandbyTime(uint8_t mode);			// Adjust the sleep time between measurements [NORMAL_MODE only][before begin()]
	void setHumOversampling(uint8_t mode);		// Set oversampling or disable humidity module [before begin()]
	void setTempOversampling(uint8_t mode);		// Set oversampling or disable temperature module [before begin()]
	void setPressOversampling(uint8_t mode);	// Set oversampling or disable pressure module [before begin()]

private:

	//============================== DEFAULT SETTINGS ========================================|
	uint8_t _i2c_address = 0x76;				// BME280 address on I2C bus                  |
	uint8_t _operating_mode = NORMAL_MODE;		// Sensor operation mode                      |
	uint8_t _standby_time = STANDBY_250MS;		// Time between measurements in NORMAL_MODE	  |
	uint8_t _filter_coef = FILTER_COEF_16; 		// Filter ratio IIR                           |
	uint8_t _temp_oversampl = OVERSAMPLING_4;	// Temperature module oversampling parameter  |
	uint8_t _hum_oversampl = OVERSAMPLING_1;	// Humidity module oversampling parameter     |
	uint8_t _press_oversampl = OVERSAMPLING_2;	// Pressure module oversampling parameter     |
	//========================================================================================|
	
	bool reset(void);                                   	// BME280 software reset 
	int32_t readTempInt();                              	// Temperature reading in integers for the function of reading
	void readCalibrationData(void);                     	// Read all cells containing calibration values
	uint8_t readRegister(uint8_t address);					// Read one 8-bit BME280 register
	uint32_t readRegister24(uint8_t address);        		// Read and combine three BME280 registers
	bool writeRegister(uint8_t address , uint8_t data);		// Write one 8-bit BME280 register

	struct {												// Structure to store all calibration values
		uint16_t _T1;
		int16_t _T2;
		int16_t _T3;
		uint16_t _P1;
		int16_t _P2;
		int16_t _P3;
		int16_t _P4;
		int16_t _P5;
		int16_t _P6;
		int16_t _P7;
		int16_t _P8;
		int16_t _P9;
		uint8_t _H1;
		int16_t _H2;
		uint8_t _H3;
		int16_t _H4;
		int16_t _H5;
		int8_t _H6;
	} CalibrationData;

};

float pressureToMmHg(float pressure);		// Convert [Pa] to [mm Hg]  
float pressureToAltitude(float pressure);	// Convert pressure to altitude

// ========================================================= SOURCE CODE ========================================================= //



/* ============ Utilities ============ */

float pressureToAltitude(float pressure) 
{
	if(!pressure) return 0;										// If the pressure module has been disabled return '0'
	pressure /= 100.0F;											// Convert [Pa] to [hPa]
	return 44330.0 * (1.0 - pow(pressure / 1013.25, 0.1903));	// Сalculate altitude
}

float pressureToMmHg(float pressure) 
{							
	return (float)(pressure * 0.00750061683f);					// Convert [Pa] to [mm Hg]
}



/* ============ Setup & begin ============ */

bool GyverBME280::begin(void) 
{
  return GyverBME280::begin(0x76);
}

bool GyverBME280::begin(uint8_t address) 
{	
	_i2c_address = address;
	/* === Start I2C bus & check BME280 === */
	Wire.begin();                             					// Start I2C bus 
	if(!GyverBME280::reset()) return false;                    	// BME280 software reset & ack check
	if(GyverBME280::readRegister(0xD0) != 0x60) return false;	// Check chip ID
	GyverBME280::readCalibrationData();       					// Read all calibration values

	/* === Load settings to BME280 === */
	GyverBME280::writeRegister(0xF2 , _hum_oversampl);                                              			// write hum oversampling value
	GyverBME280::writeRegister(0xF2 , GyverBME280::readRegister(0xF2));                             			// rewrite hum oversampling register
	GyverBME280::writeRegister(0xF4 , ((_temp_oversampl << 5) | (_press_oversampl << 2) | _operating_mode));    // write temp & press oversampling value , normal mode
	GyverBME280::writeRegister(0xF5 , ((_standby_time << 5) | (_filter_coef << 2)));                			// write standby time & filter coef
	return true;
}

void GyverBME280::setMode(uint8_t mode) 
{
	_operating_mode = mode;
}


void GyverBME280::setFilter(uint8_t mode)
{
	_filter_coef = mode;
}

void GyverBME280::setStandbyTime(uint8_t mode)
{
	_standby_time = mode;
}

void GyverBME280::setHumOversampling(uint8_t mode) 
{
	_hum_oversampl = mode;
}

void GyverBME280::setTempOversampling(uint8_t mode) 
{
	_temp_oversampl = mode;
}

void GyverBME280::setPressOversampling(uint8_t mode) 
{
	_press_oversampl = mode;
}



/* ============ Reading ============ */

int32_t GyverBME280::readTempInt(void) 
{
	uint32_t temp_raw = GyverBME280::readRegister24(0xFA);			// Read 24-bit value
	if (temp_raw == 0x800000) return 0;								// If the temperature module has been disabled return '0'

	temp_raw >>= 4;													// Start temperature reading in integers
	int32_t value_1 = ((((temp_raw >> 3) - ((int32_t)CalibrationData._T1 << 1))) *
	((int32_t)CalibrationData._T2)) >> 11;
	int32_t value_2 = (((((temp_raw >> 4) - ((int32_t)CalibrationData._T1)) * 
	((temp_raw >> 4) - ((int32_t)CalibrationData._T1))) >> 12) * ((int32_t)CalibrationData._T3)) >> 14;

	return (value_1 + value_2);										// Return temperature in integers
}


float GyverBME280::readTemperature(void) 
{
	float T = (GyverBME280::readTempInt() * 5 + 128) >> 8;			// Сonvert temperature from integer to float
	return T / 100;													// Return temperature in float
}


float GyverBME280::readPressure(void)
{
	uint32_t press_raw = GyverBME280::readRegister24(0xF7);			// Read 24-bit value
	if (press_raw == 0x800000) return 0;							// If the pressure module has been disabled return '0'

	press_raw >>= 4;												// Start pressure converting
	int64_t value_1 = ((int64_t)GyverBME280::readTempInt()) - 128000;
	int64_t value_2 = value_1 * value_1 * (int64_t)CalibrationData._P6;
	value_2 = value_2 + ((value_1 * (int64_t)CalibrationData._P5) << 17);
	value_2 = value_2 + (((int64_t)CalibrationData._P4) << 35);
	value_1 = ((value_1 * value_1 * (int64_t)CalibrationData._P3) >> 8) + ((value_1 * (int64_t)CalibrationData._P2) << 12);
	value_1 = (((((int64_t)1) << 47) + value_1)) * ((int64_t)CalibrationData._P1) >> 33;

	if (!value_1) return 0; 										// Avoid division by zero

	int64_t p = 1048576 - press_raw;
	p = (((p << 31) - value_2) * 3125) / value_1;
	value_1 = (((int64_t)CalibrationData._P9) * (p >> 13) * (p >> 13)) >> 25;
	value_2 = (((int64_t)CalibrationData._P8) * p) >> 19;
	p = ((p + value_1 + value_2) >> 8) + (((int64_t)CalibrationData._P7) << 4);

	return (float)p / 256;											// Return pressure in float				
}


float GyverBME280::readHumidity(void) 
{
	Wire.beginTransmission(_i2c_address);									// Start I2C transmission
	Wire.write(0xFD);												   		// Request humidity data register	
	if(Wire.endTransmission() != 0) return 0;										
	Wire.requestFrom(_i2c_address, 2);										// Request humidity data 
	int32_t hum_raw = ((uint16_t)Wire.read() << 8) | (uint16_t)Wire.read();	// Read humidity data 
	if (hum_raw == 0x8000) return 0;										// If the humidity module has been disabled return '0'

	int32_t value  = (GyverBME280::readTempInt() - ((int32_t)76800));		// Start humidity converting
	value = (((((hum_raw << 14) - (((int32_t)CalibrationData._H4) << 20) -
	(((int32_t)CalibrationData._H5) * value)) +((int32_t)16384)) >> 15) * 
	(((((((value * ((int32_t)CalibrationData._H6)) >> 10) *(((value * 
	((int32_t)CalibrationData._H3)) >> 11) + ((int32_t)32768))) >> 10) +
	((int32_t)2097152)) * ((int32_t)CalibrationData._H2) + 8192) >> 14));
	value = (value - (((((value >> 15) * (value >> 15)) >> 7) * ((int32_t)CalibrationData._H1)) >> 4));
	value = (value < 0) ? 0 : value;
	value = (value > 419430400) ? 419430400 : value;
	float h = (value >> 12);

	return h / 1024.0;														// Return humidity in float
}



/* ============ Misc ============ */

bool GyverBME280::isMeasuring(void)									 // Returns 'true' while the measurement is in progress
{								
	return (bool)((GyverBME280::readRegister(0xF3) & 0x08) >> 3);  	 // Read status register & mask bit "measuring"
}

void GyverBME280::oneMeasurement(void)
{
	GyverBME280::writeRegister(0xF4 , ((GyverBME280::readRegister(0xF4) & 0xFC) | 0x02));   // Set the operating mode to FORCED_MODE
}

GyverBME280::GyverBME280() {}



/* ============ Private ============ */

/* = BME280 software reset = */
bool GyverBME280::reset(void)
{
	if(!GyverBME280::writeRegister(0x0E , 0xB6)) return false; 
	delay(10);
	return true;
}


/* = Read and combine three BME280 registers = */
uint32_t GyverBME280::readRegister24(uint8_t address) 
{	 
	Wire.beginTransmission(_i2c_address);
	Wire.write(address);
	if(Wire.endTransmission() != 0) return 0x800000;
	Wire.requestFrom(_i2c_address, 3);
	return (((uint32_t)Wire.read() << 16) | ((uint32_t)Wire.read() << 8) | (uint32_t)Wire.read());
}


/* = Write one 8-bit BME280 register = */
bool GyverBME280::writeRegister(uint8_t address , uint8_t data)
{
	Wire.beginTransmission(_i2c_address);
	Wire.write(address);
	Wire.write(data);
	if(Wire.endTransmission() != 0) return false;
	return true;
}


/* = Read one 8-bit BME280 register = */
uint8_t GyverBME280::readRegister(uint8_t address)
{
	Wire.beginTransmission(_i2c_address);
	Wire.write(address);
	if(Wire.endTransmission() != 0) return 0;
	Wire.requestFrom(_i2c_address , 1);
	return Wire.read();
}

/* = Structure to store all calibration values = */
void GyverBME280::readCalibrationData(void)
{
	/* first part request*/
	Wire.beginTransmission(_i2c_address);
	Wire.write(0x88);
	if(Wire.endTransmission() != 0) return;
	Wire.requestFrom(_i2c_address , 25);
	/* reading */
	CalibrationData._T1 = (Wire.read() | (Wire.read() << 8));
	CalibrationData._T2 = (Wire.read() | (Wire.read() << 8));
	CalibrationData._T3 = (Wire.read() | (Wire.read() << 8));
	CalibrationData._P1 = (Wire.read() | (Wire.read() << 8));
	CalibrationData._P2 = (Wire.read() | (Wire.read() << 8));
	CalibrationData._P3 = (Wire.read() | (Wire.read() << 8));
	CalibrationData._P4 = (Wire.read() | (Wire.read() << 8));
	CalibrationData._P5 = (Wire.read() | (Wire.read() << 8));
	CalibrationData._P6 = (Wire.read() | (Wire.read() << 8));
	CalibrationData._P7 = (Wire.read() | (Wire.read() << 8));
	CalibrationData._P8 = (Wire.read() | (Wire.read() << 8));
	CalibrationData._P9 = (Wire.read() | (Wire.read() << 8));
	CalibrationData._H1 = Wire.read();

	/* second part request*/
	Wire.beginTransmission(_i2c_address);
	Wire.write(0xE1);
	Wire.endTransmission();
	Wire.requestFrom(_i2c_address , 8);
	/* reading */
	CalibrationData._H2 = (Wire.read() | (Wire.read() << 8));
	CalibrationData._H3 = Wire.read();
	CalibrationData._H4 = (Wire.read() << 4);
	uint8_t interVal = Wire.read();
	CalibrationData._H4 |= (interVal & 0xF);
	CalibrationData._H5 = (((interVal & 0xF0) >> 4) | (Wire.read() << 4));
	CalibrationData._H6 = Wire.read();
}


#endif