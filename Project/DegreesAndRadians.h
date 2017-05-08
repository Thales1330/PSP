#ifndef DEGREESANDRADIANS_H
#define DEGREESANDRADIANS_H

#include<wx/math.h>

// Convert between degrees and radians.
inline double wxDegToRad(double deg) { return (deg * M_PI) / 180.0; }
inline double wxRadToDeg(double rad) { return (rad * 180.0) / M_PI; }

#endif // DEGREESANDRADIANS_H
