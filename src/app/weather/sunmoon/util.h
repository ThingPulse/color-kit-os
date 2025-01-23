#pragma once

#include "time.h"

#define SECS_PER_MIN  60
#define SECS_PER_HOUR 3600
#define SECS_PER_DAY  86400
#define DAYS_PER_YEAR 365

const double LUNAR_MONTH = 29.530588853;
const uint8_t NUMBER_OF_MOON_IMAGES = 32;

// format specifiers: https://cplusplus.com/reference/ctime/strftime/
#ifdef DATE_TIME_FORMAT_US
  int timePosX = 29;
  #define UI_DATE_FORMAT "%m/%d/%Y"
  #define UI_TIME_FORMAT "%I:%M:%S %P"
  #define UI_TIME_FORMAT_NO_SECONDS "%I:%M %P"
  #define UI_TIMESTAMP_FORMAT (UI_DATE_FORMAT + " " + UI_TIME_FORMAT)
#else
  int timePosX = 68;
  #define UI_DATE_FORMAT "%d.%m.%Y"
  #define UI_TIME_FORMAT "%H:%M:%S"
  #define UI_TIME_FORMAT_NO_SECONDS "%H:%M"
  #define UI_TIMESTAMP_FORMAT (UI_DATE_FORMAT + " " + UI_TIME_FORMAT)
#endif

// Function to check if a year is a leap year
int is_leap_year(int year) {
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
        return 1;
    }
    return 0;
}

// Days in each month for non-leap years
const int days_per_month[] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

// Helper function to calculate total days from year 1970
long days_from_epoch(int year, int month, int day) {
    long days = 0;
    
    // Add days for the years since 1970
    for (int y = 1970; y < year; ++y) {
        days += DAYS_PER_YEAR + is_leap_year(y);
    }
    
    // Add days for the months in the current year
    for (int m = 0; m < month; ++m) {
        days += days_per_month[m];
        // Add one extra day if it's February of a leap year
        if (m == 1 && is_leap_year(year)) {
            days += 1;
        }
    }

    // Add days in the current month
    days += day - 1;
    
    return days;
}

// Native implementation of mkgmtime
time_t mkgmtime(struct tm *timeptr) {
    long days = days_from_epoch(timeptr->tm_year + 1900, timeptr->tm_mon, timeptr->tm_mday);

    // Convert days to seconds
    time_t seconds = days * SECS_PER_DAY;

    // Add hours, minutes, and seconds
    seconds += timeptr->tm_hour * SECS_PER_HOUR;
    seconds += timeptr->tm_min * SECS_PER_MIN;
    seconds += timeptr->tm_sec;

    return seconds;
}

