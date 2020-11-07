#ifndef FASTLED_VERSION		// запрещаем одновременный инклуд с фастледом

// X(n+1) = (2053 * X(n)) + 13849)

/// random number seed
uint16_t rand16seed;// = RAND16_SEED;

/// Generate an 8-bit random number
static uint8_t random8()
{
    rand16seed = ((uint16_t)rand16seed * 2053) + 13849;
    // return the sum of the high and low bytes, for better
    //  mixing and non-sequential correlation
    return (uint8_t)(((uint8_t)(rand16seed & 0xFF)) +
                     ((uint8_t)(rand16seed >> 8)));
}

/// Generate a 16 bit random number
static uint16_t random16()
{
    rand16seed = ((uint16_t)rand16seed * 2053) + 13849;
    return rand16seed;
}

/// Generate an 8-bit random number between 0 and lim
/// @param lim the upper bound for the result
static uint8_t random8(uint8_t lim)
{
    uint8_t r = random8();
    r = (r*lim) >> 8;
    return r;
}

/// Generate an 8-bit random number in the given range
/// @param min the lower bound for the random number
/// @param lim the upper bound for the random number
static uint8_t random8(uint8_t min, uint8_t lim)
{
    uint8_t delta = lim - min;
    uint8_t r = random8(delta) + min;
    return r;
}

/// Generate an 16-bit random number between 0 and lim
/// @param lim the upper bound for the result
static uint16_t random16( uint16_t lim)
{
    uint16_t r = random16();
    uint32_t p = (uint32_t)lim * (uint32_t)r;
    r = p >> 16;
    return r;
}

/// Generate an 16-bit random number in the given range
/// @param min the lower bound for the random number
/// @param lim the upper bound for the random number
static uint16_t random16( uint16_t min, uint16_t lim)
{
    uint16_t delta = lim - min;
    uint16_t r = random16( delta) + min;
    return r;
}

/// Set the 16-bit seed used for the random number generator
static void random16_set_seed( uint16_t seed)
{
    rand16seed = seed;
}

/// Get the current seed value for the random number generator
static uint16_t random16_get_seed()
{
    return rand16seed;
}

/// Add entropy into the random number generator
static void random16_add_entropy( uint16_t entropy)
{
    rand16seed += entropy;
}

#endif