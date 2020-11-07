#ifndef FASTLED_VERSION		// запрещаем одновременный инклуд с фастледом
#pragma once
#include <Arduino.h>
#include "math8.h"

// вытащил шум Перлина из FastLED

/// 16 bit, fixed point implementation of perlin's Simplex Noise.  Coordinates are
/// 16.16 fixed point values, 32 bit integers with integral coordinates in the high 16
/// bits and fractional in the low 16 bits, and the function takes 1d, 2d, and 3d coordinate
/// values.  These functions are scaled to return 0-65535

uint16_t inoise16(uint32_t x, uint32_t y, uint32_t z);
uint16_t inoise16(uint32_t x, uint32_t y);
uint16_t inoise16(uint32_t x);


/// 16 bit raw versions of the noise functions.  These values are not scaled/altered and have
/// output values roughly in the range (-18k,18k)
int16_t inoise16_raw(uint32_t x, uint32_t y, uint32_t z);
int16_t inoise16_raw(uint32_t x, uint32_t y);
int16_t inoise16_raw(uint32_t x);
///@}


/// 8 bit, fixed point implementation of perlin's Simplex Noise.  Coordinates are
/// 8.8 fixed point values, 16 bit integers with integral coordinates in the high 8
/// bits and fractional in the low 8 bits, and the function takes 1d, 2d, and 3d coordinate
/// values.  These functions are scaled to return 0-255
uint8_t inoise8(uint16_t x, uint16_t y, uint16_t z);
uint8_t inoise8(uint16_t x, uint16_t y);
uint8_t inoise8(uint16_t x);
///@}


/// 8 bit raw versions of the noise functions.  These values are not scaled/altered and have
/// output values roughly in the range (-70,70)
int8_t inoise8_raw(uint16_t x, uint16_t y, uint16_t z);
int8_t inoise8_raw(uint16_t x, uint16_t y);
int8_t inoise8_raw(uint16_t x);

#endif