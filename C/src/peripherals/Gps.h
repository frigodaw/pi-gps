#ifndef GPS_H
# define GPS_H

// start of includes area
# include "StdTypes.h"
// end of include area


// start of variables area
static const char *Gps_serialDirectory = "/dev/ttyS0";
// end of variable sarea


// start of functions area
Std_ReturnType Gps_Init(void);
// end of functions area

#endif  /* GPS_H */ 