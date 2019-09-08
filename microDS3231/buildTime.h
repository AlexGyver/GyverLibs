// source http://qaru.site/questions/186859/how-to-use-date-and-time-predefined-macros-in-as-two-integers-then-stringify

#ifndef BUILD_DEFS_H
#define BUILD_DEFS_H
// Example of __DATE__ string: "Jul 27 2012"
//                              01234567890

#define BUILD_YEAR_CH0 (__DATE__[7]-'0')
#define BUILD_YEAR_CH1 (__DATE__[8]-'0')
#define BUILD_YEAR_CH2 (__DATE__[9]-'0')
#define BUILD_YEAR_CH3 (__DATE__[10]-'0')
#define BUILD_YEAR (BUILD_YEAR_CH0*1000+BUILD_YEAR_CH1*100 + BUILD_YEAR_CH2*10+BUILD_YEAR_CH3)

#define BUILD_MONTH_IS_JAN (__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_FEB (__DATE__[0] == 'F')
#define BUILD_MONTH_IS_MAR (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')
#define BUILD_MONTH_IS_APR (__DATE__[0] == 'A' && __DATE__[1] == 'p')
#define BUILD_MONTH_IS_MAY (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')
#define BUILD_MONTH_IS_JUN (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_JUL (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
#define BUILD_MONTH_IS_AUG (__DATE__[0] == 'A' && __DATE__[1] == 'u')
#define BUILD_MONTH_IS_SEP (__DATE__[0] == 'S')
#define BUILD_MONTH_IS_OCT (__DATE__[0] == 'O')
#define BUILD_MONTH_IS_NOV (__DATE__[0] == 'N')
#define BUILD_MONTH_IS_DEC (__DATE__[0] == 'D')

#define BUILD_MONTH \
  ( \
    (BUILD_MONTH_IS_JAN) ? 1 : \
    (BUILD_MONTH_IS_FEB) ? 2 : \
    (BUILD_MONTH_IS_MAR) ? 3 : \
    (BUILD_MONTH_IS_APR) ? 4 : \
    (BUILD_MONTH_IS_MAY) ? 5 : \
    (BUILD_MONTH_IS_JUN) ? 6 : \
    (BUILD_MONTH_IS_JUL) ? 7 : \
    (BUILD_MONTH_IS_AUG) ? 8 : \
    (BUILD_MONTH_IS_SEP) ? 9 : \
    (BUILD_MONTH_IS_OCT) ? 10 : \
    (BUILD_MONTH_IS_NOV) ? 11 : \
    (BUILD_MONTH_IS_DEC) ? 12 : \
    /* error default */    '?' \
  )

#define BUILD_DAY_CH0 (((__DATE__[4] >= '0') ? (__DATE__[4]) : '0')-'0')
#define BUILD_DAY_CH1 (__DATE__[5]-'0')
#define BUILD_DAY (BUILD_DAY_CH0*10+BUILD_DAY_CH1)

// Example of __TIME__ string: "21:06:19"
//                              01234567

#define BUILD_HOUR_CH0 (__TIME__[0]-'0')
#define BUILD_HOUR_CH1 (__TIME__[1]-'0')
#define BUILD_HOUR (BUILD_HOUR_CH0*10+BUILD_HOUR_CH1)

#define BUILD_MIN_CH0 (__TIME__[3]-'0')
#define BUILD_MIN_CH1 (__TIME__[4]-'0')
#define BUILD_MIN (BUILD_MIN_CH0*10+BUILD_MIN_CH1)

#define BUILD_SEC_CH0 (__TIME__[6]-'0')
#define BUILD_SEC_CH1 (__TIME__[7]-'0')
#define BUILD_SEC (BUILD_SEC_CH0*10+BUILD_SEC_CH1)

#endif // BUILD_DEFS_H