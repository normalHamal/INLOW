#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

#define SECONDS_PER_DAY (24 * 60 * 60)

static bool isLeapYear(time_t year)
{
	return !(year % 4) && ((year % 100) || !(year % 400));
}

static time_t daysPerYear(time_t year)
{
    return isLeapYear(year) ? 366 : 365;
}

enum
{
    JANUARY = 0,
    FEBRUARY,
    MARCH,
    APRIL,
    MAY,
    JUNE,
    JULY,
    AUGUST,
    SEPTEMBER,
    OCTOBER,
    NOVEMBER,
    DECEMBER
};

static time_t daysPerMonth(int month, time_t year)
{
    switch (month)
	{
    case JANUARY:
		return 31;
    case FEBRUARY:
		return isLeapYear(year) ? 29 : 28;
    case MARCH:
		return 31;
    case APRIL:
		return 30;
    case MAY:
		return 31;
    case JUNE:
		return 30;
    case JULY:
		return 31;
    case AUGUST:
		return 31;
    case SEPTEMBER:
		return 30;
    case OCTOBER:
		return 31;
    case NOVEMBER:
		return 30;
    case DECEMBER:
		return 31;
    default:
		__builtin_unreachable();
    }
}

struct tm* gmtime_r(const time_t* restrict time, struct tm* restrict tm)
{
    time_t secondsLeft = *time;
    time_t year = 1970;
    time_t daysSinceEpoch = 0;

    while (secondsLeft < 0)
	{
        year--;
        secondsLeft += daysPerYear(year) * SECONDS_PER_DAY;
        daysSinceEpoch -= daysPerYear(year);
    }

    while (secondsLeft >= daysPerYear(year) * SECONDS_PER_DAY)
	{
        secondsLeft -= daysPerYear(year) * SECONDS_PER_DAY;
        daysSinceEpoch += daysPerYear(year);
        year++;
    }

    if (year < INT_MIN + 1900 || year > INT_MAX - 1900)
	{
        errno = EOVERFLOW;
        return NULL;
    }

    tm->tm_year = year - 1900;

    tm->tm_mon = JANUARY;
    tm->tm_yday = 0;
    while (secondsLeft >= daysPerMonth(tm->tm_mon, year) * SECONDS_PER_DAY)
	{
        secondsLeft -= daysPerMonth(tm->tm_mon, year) * SECONDS_PER_DAY;
        tm->tm_yday += daysPerMonth(tm->tm_mon, year);
        tm->tm_mon++;
    }

    int day = secondsLeft / SECONDS_PER_DAY;
    tm->tm_mday = day + 1;
    tm->tm_yday += day;
    daysSinceEpoch += day;
    secondsLeft %= SECONDS_PER_DAY;

    tm->tm_hour = secondsLeft / (60 * 60);
    secondsLeft %= (60 * 60);

    tm->tm_min = secondsLeft / 60;
    tm->tm_sec = secondsLeft % 60;
    tm->tm_isdst = 0;

    tm->tm_wday = (4 + daysSinceEpoch) % 7;
    if (tm->tm_wday < 0)
	{
        tm->tm_wday += 7;
    }

    return tm;
}
