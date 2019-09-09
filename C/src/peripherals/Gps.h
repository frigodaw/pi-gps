#ifndef GPS_H
# define GPS_H

// start of includes area
# include "StdTypes.h"
# include <stdio.h>
# include <string.h>
# include <fcntl.h>   /* File Control Definitions           */
# include <termios.h> /* POSIX Terminal Control Definitions */
# include <unistd.h>  /* UNIX Standard Definitions 	   */ 
# include <errno.h>   /* ERROR Number Definitions           */
// end of include area


// start of defines area
# define GPS_BUFFERSIZE     127
# define GPS_STOREDRECORDS  5
# define GPS_SENTENCES      2

# define GPS_SENTENCELENGTH 6
# define GPS_GPGGA (char*)("$GPGGA")
# define GPS_GPRMC (char*)("$GPRMC")
// end of defines area


//start of typdefs and enums area
enum Gps_sentencePosition
{
    GPS_ENUM_GPGGA,
    GPS_ENUM_GPRMC
};

typedef struct Gps_readoutData_Tag
{
    uint8 fileDescriptor;                                                    //used for reading from serial port
    char storageBuffer[GPS_SENTENCES][GPS_STOREDRECORDS][GPS_BUFFERSIZE];    //cyclic buffer to store readouts
    uint8 currentIdx[GPS_SENTENCES];                                          //helper 'pointer' to Gps_storageBuffer
} Gps_readoutData_T;

typedef struct Gps_detailedData_Tag
{
    char* time;
    uint16 latitude;
    uint16 longitude;
    uint8 fixQuality;
    uint8 satelitesNum;
    float altitude;
} Gps_detailedData_T;

typedef struct Gps_data_Tag
{
    Gps_detailedData_T data[GPS_STOREDRECORDS];
    uint8 currentIdx;
}  Gps_data_T;
//end of typdefs and enums area


// start of variables area
static const char *Gps_serialDirectory = "/dev/ttyS0";     //serial port directory
static struct termios Gps_serialPortSettings;              //termios structure to store serial settings
static Gps_readoutData_T Gps_rawData;                      //structure to store raw strings received from satellite
static Gps_data_T Gps_mainData;                            //structure with detailed gps data
// end of variable sarea


// start of functions area
Std_ReturnType Gps_Init(void);
Std_ReturnType Gps_SerialInit(void);
Std_ReturnType Gps_resetDataInit(void);
Std_ReturnType Gps_Main(void);
Std_ReturnType Gps_GetData(char* buffer);
void Gps_TerminateConnection(void);
void Gps_FilterData(char* buffer, char* sentence, uint8 sentecePosition);
void Gps_GetFilteredData_GPGGA(char* buffer);
// end of functions area

#endif  /* GPS_H */ 