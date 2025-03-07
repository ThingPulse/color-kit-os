/**The MIT License (MIT)

Copyright (c) 2018 by ThingPulse Ltd., https://thingpulse.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _SUN_MOON_CALC_H
#define _SUN_MOON_CALC_H
#include <time.h>

#ifdef ARDUINO
#include <Arduino.h>
#else
#include "math.h"

typedef u_int8_t              uint8_t;   /* u_int8_t is defined in <machine/types.h> */
typedef u_int16_t            uint16_t;   /* u_int16_t is defined in <machine/types.h> */
typedef u_int32_t            uint32_t;   /* u_int32_t is defined in <machine/types.h> */
typedef u_int64_t            uint64_t;   /* u_int64_t is defined in <machine/types.h> */

#define _abs(x)                   ((x) > 0 ? (x) : -(x))  // abs() comes from STL
#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#define _round(x)                 ((x) >= 0 ? (long)((x) + 0.5) : (long)((x) - 0.5))  // round() comes from STL
#define radians(deg)              ((deg) * DEG_TO_RAD)
#define degrees(rad)              ((rad) * RAD_TO_DEG)
#define sq(x)                     ((x) * (x))
#define DEG_TO_RAD                (PI / 180.0)
#define RAD_TO_DEG                (180.0 / PI)

#endif

class SunMoonCalc {
public:
    typedef struct Sun {
        double azimuth;
        double elevation;
        double riseJd; // jd = Julian Day
        double setJd;
        double transitJd;
        time_t rise; // timestamp in UTC
        time_t set;
        time_t transit;
        double transitElevation;
        double distance;
    } Sun;

    typedef struct MoonPhase {
        uint8_t index;
        char* name;
    } MoonPhase;

    typedef struct Moon {
        double azimuth;
        double elevation;
        double riseJd; // jd = Julian Day
        double setJd;
        double transitJd;
        time_t rise; // timestamp in UTC
        time_t set;
        time_t transit;
        double age;
        double transitElevation;
        double distance;
        double illumination;
        double axisPositionAngle;
        double brightLimbAngle;
        double parallacticAngle;
        MoonPhase phase;
    } Moon;

    typedef struct Result {
        Sun sun;
        Moon moon;
    } Result;

    SunMoonCalc(time_t timestamp, double lat, double lon);
    SunMoonCalc(int16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, double lat,
                double lon);

    Result calculateSunAndMoonData();

private:
    enum TWILIGHT {
        /**
         * Event ID for calculation of rising and setting times for astronomical
         * twilight. In this case, the calculated time will be the time when the
         * center of the object is at -18 degrees of geometrical elevation below the
         * astronomical horizon. At this time astronomical observations are possible
         * because the sky is dark enough.
         */
         TWILIGHT_ASTRONOMICAL,
        /**
         * Event ID for calculation of rising and setting times for nautical
         * twilight. In this case, the calculated time will be the time when the
         * center of the object is at -12 degrees of geometric elevation below the
         * astronomical horizon.
         */
         TWILIGHT_NAUTICAL,
        /**
         * Event ID for calculation of rising and setting times for civil twilight.
         * In this case, the calculated time will be the time when the center of the
         * object is at -6 degrees of geometric elevation below the astronomical
         * horizon.
         */
         TWILIGHT_CIVIL,
        /**
         * The standard value of 34' for the refraction at the local horizon.
         */
         HORIZON_34arcmin
    };

    typedef struct PositionalData {
        double longitude;
        double latitude;
        double distance;
        double angularRadius;
    } PositionalData;

    double t;
    double jd_UT;
    double TTminusUT;
    double lat; // internal value is in radians!
    double lon; // internal value is in radians!
    double slongitude; // sun longitude
    double sanomaly; // sun anomaly
    double moonAge; // this is calculated as a by-product in getMoonPosition()
    TWILIGHT twilight = HORIZON_34arcmin;

    void setInternalTime(int16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
    void setUTDate(double jd);
    PositionalData getSunPosition();
    PositionalData getMoonPosition();
    double *doCalc(PositionalData position);
    double normalizeRadians(double r);
    double calculateTwilightAdjustment(PositionalData position) const;
    double obtainAccurateRiseSetTransit(double riseSetJd, int index, int niter, bool sun);
    double *getMoonDiskOrientationAngles(double lst, double sunRA, double sunDec, double moonLon, double moonLat,
                                         double moonRA, double moonDec);
    SunMoonCalc::MoonPhase calculateMoonPhase(double lunarAge) const;
    Result translateToHumanReadable(Result result) const;
    time_t fromJulian(double julianDays) const;
    double toJulian(int16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) const;
};
#endif