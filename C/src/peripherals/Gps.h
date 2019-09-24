#ifndef GPS_H
# define GPS_H

// start of includes area
# include "StdTypes.h"
# include <stdio.h>
# include <string.h>
# include<math.h>
# include <fcntl.h>   /* File Control Definitions           */
# include <termios.h> /* POSIX Terminal Control Definitions */
# include <unistd.h>  /* UNIX Standard Definitions 	   */ 
# include <errno.h>   /* ERROR Number Definitions           */
// end of include area


// start of defines area
# define GPS_BUFFERSIZE          127u
# define GPS_STOREDRECORDS       5u
# define GPS_SENTENCES           2u

# define GPS_SENTENCELENGTH      6u
# define GPS_GPGGA               (char*)("$GPGGA")
# define GPS_GPRMC               (char*)("$GPRMC")

# define GPS_GPPGA_DIV_VALUE     100.f
# define GPS_GPPGA_MUL_VALUE     (double)(100.f/60.f)
// end of defines area


//start of typdefs and enums area
enum Gps_sentencePosition
{
   GPS_ENUM_GPGGA,
   GPS_ENUM_GPRMC
};

enum Gps_gpggaDataSequence
{
   GPS_GPGGA_ID,
   GPS_GPGGA_TIME,
   GPS_GPGGA_LATITUDE,
   GPS_GPGGA_NS,
   GPS_GPGGA_LONGITUDE,
   GPS_GPGGA_WE,
   GPS_GPGGA_FIXQUALITY,
   GPS_GPGGA_SATELITESNUM,
   GPS_GPGGA_DILUTION,
   GPS_GPGGA_ALTITUDE
};

typedef struct Gps_detailedData_Tag
{
   char time[6];
   double latitude;
   char NS;
   double longitude;
   char WE;
   uint8 fixQuality;
   uint8 satelitesNum;
   float dilution;
   float altitude;
} Gps_detailedData_T;

typedef struct Gps_data_Tag
{
   Gps_detailedData_T data[GPS_STOREDRECORDS];
   uint8 currentIdx;
   uint8 fileDescriptor;
   boolean isActive;
}  Gps_data_T;
//end of typdefs and enums area


// start of variables area
static const char *Gps_serialDirectory = "/dev/ttyS0";     //serial port directory
static uint8 fileDescriptor;                               //used for open and close serial 
static struct termios Gps_serialPortSettings;              //termios structure to store serial settings
static Gps_data_T Gps_mainData;                            //structure with detailed gps data
// end of variable sarea


// start of functions area
Std_ReturnType Gps_Init(void);
Std_ReturnType Gps_SerialInit(void);
Std_ReturnType Gps_ResetDataOnInit(void);
Std_ReturnType Gps_Main(void);
Std_ReturnType Gps_GetData(char* buffer);
void Gps_TerminateConnection(void);
void Gps_FilterData_GPGGA(char* buffer);
void Gps_FormatConverter(double *coordinate);
// end of functions area

#endif  /* GPS_H */ 