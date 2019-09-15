#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "Gps.h"

/* Executes only one time to initialize data */
Std_ReturnType Gps_Init(void)
{
   Std_ReturnType retVal = OK;

   retVal |= Gps_SerialInit();
   retVal |= Gps_resetDataOnInit();

   return retVal;
}

/* Main function to call another worker function */
Std_ReturnType Gps_Main(void)
{
   Std_ReturnType retVal = OK;
   char dataBuffer[GPS_BUFFERSIZE];

   retVal |= Gps_GetData(dataBuffer);

   if(OK == retVal)  //data available
   {
      /* Start of data filtering, check whether data starts with given string */
      Gps_FilterData(dataBuffer, GPS_GPGGA, GPS_ENUM_GPGGA);
      //Gps_FilterData(dataBuffer, GPS_GPRMC, GPS_ENUM_GPRMC);

      Gps_GetFilteredData_GPGGA(dataBuffer);
   }
}

/* Configuration on init */
Std_ReturnType Gps_SerialInit(void)
{
   Std_ReturnType retVal = OK;

   /*------------------------------- Opening the Serial Port -------------------------------*/
   Gps_rawData.fileDescriptor = open(Gps_serialDirectory, O_RDONLY | O_NOCTTY);
                  /* O_RDWR   - Read only access to serial port        */
                  /* O_NOCTTY - No terminal will control the process   */
                  /* Open in blocking mode, read will wait             */
                     
   if(Gps_rawData.fileDescriptor == -1)  /* Error Checking */
   {
      printf("Error in opening %s  \n", Gps_serialDirectory);
      retVal |= NOK;
   }

   /*---------- Setting the Attributes of the serial port using termios structure --------- */
   tcgetattr(Gps_rawData.fileDescriptor, &Gps_serialPortSettings);	/* Get the current attributes of the Serial port */

   /* Setting the Baud rate */
   cfsetispeed(&Gps_serialPortSettings,B9600); /* Set Read  Speed as 9600                       */

   /* 8N1 Mode */
   Gps_serialPortSettings.c_cflag &= ~PARENB;                           /* Disables the Parity Enable bit(PARENB),So No Parity   */
   Gps_serialPortSettings.c_cflag &= ~CSTOPB;                           /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
   Gps_serialPortSettings.c_cflag &= ~CSIZE;                            /* Clears the mask for setting the data size             */
   Gps_serialPortSettings.c_cflag |=  CS8;                              /* Set the data bits = 8                                 */
   Gps_serialPortSettings.c_cflag &= ~CRTSCTS;                          /* No Hardware flow Control                              */
   Gps_serialPortSettings.c_cflag |= CREAD | CLOCAL;                    /* Enable receiver,Ignore Modem Control lines            */
   Gps_serialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);           /* Disable XON/XOFF flow control both i/p and o/p        */
   Gps_serialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);   /* Non Cannonical mode                                   */
   Gps_serialPortSettings.c_oflag &= ~OPOST;                            /* No Output Processing                                  */

   /* Setting Time outs */
   Gps_serialPortSettings.c_cc[VMIN] = 10;  /* Read at least 10 characters */
   Gps_serialPortSettings.c_cc[VTIME] = 0;  /* Wait indefinetly   */

   /* Set the attributes to the termios structure */
   if((tcsetattr(Gps_rawData.fileDescriptor,TCSANOW, &Gps_serialPortSettings)) != 0) 
   {
      printf("Error in setting attributes  \n");
      retVal |= NOK;
   }
   else
   {
      /* ok */
   }

   return retVal;
}

/* Set data to specyfic values during init */
Std_ReturnType Gps_resetDataOnInit(void)   
{
   Std_ReturnType retVal = OK;
   for(uint8 idx; idx < GPS_STOREDRECORDS; idx++)
   {
      for(uint8 j=0; j<6; j++)
         Gps_mainData.data[idx].time[j] = '0';
      Gps_mainData.data[idx].latitude = 0.f;
      Gps_mainData.data[idx].NS = '0';
      Gps_mainData.data[idx].longitude = 0.f;
      Gps_mainData.data[idx].WE = '0';
      Gps_mainData.data[idx].fixQuality = 0u;
      Gps_mainData.data[idx].satelitesNum = 0u;
      Gps_mainData.data[idx].dilution = 0.f;
      Gps_mainData.data[idx].altitude = 0.f;
   }
   Gps_mainData.currentIdx = 0u;

   return retVal;
}

/* Read raw data from serial */
Std_ReturnType Gps_GetData(char* buffer)
{
   Std_ReturnType retVal = OK;
   uint16 bytesRead = 0;    /* Number of bytes read by the read() system call */

   memset(buffer, 0, GPS_BUFFERSIZE);                                      /* Set zeros to buffer */
   tcflush(Gps_rawData.fileDescriptor, TCIFLUSH);                          /* Discards old data in the rx buffer */
   bytesRead = read(Gps_rawData.fileDescriptor, buffer, GPS_BUFFERSIZE);   /* Read the data                      */

   if(bytesRead == - 1)
   {
      printf("Error in reading data from serial port!  \n");
      retVal |= NOK;
   }
   else
   {
      /* ok */
   }
   return retVal;
}

/* Closes the serial port */
void Gps_TerminateConnection(void)
{
   close(Gps_rawData.fileDescriptor); 
}

/* Selects demanded string from serial and puts it to the array */
void Gps_FilterData(char* buffer, char* sentence, uint8 sentecePosition)
{
   static uint8 idx;
   if(!strncmp(buffer, sentence, GPS_SENTENCELENGTH))
   {
      /* Replace the oldest readout with the current one */
      strncpy(Gps_rawData.storageBufferGpgga[idx], buffer, GPS_BUFFERSIZE);
      printf("%d) %s", idx, Gps_rawData.storageBufferGpgga[idx]); //debug

      /* Increment index */
      idx = (idx < GPS_STOREDRECORDS-1) ? idx+1 : 0u;
   }
   else
   {
      /* no correct data */
   }
}

/* Selects data from array and puts it to the final logging structure */
void Gps_GetFilteredData_GPGGA(char* buffer)
{
   if(!strncmp(buffer, GPS_GPGGA, GPS_SENTENCELENGTH))
   {
      //buffer = "$GPGGA,160915.00,5003.42292,N,01855.72499,E,1,08,1.01,240.5,M,40.7,M,,*5F";

      char* tmpBuffer = strtok(buffer, ",");
      uint8 sequence = GPS_GPGGA_TIME;
      uint8 *idx = &Gps_mainData.currentIdx;
      *idx = ((Gps_mainData.currentIdx < GPS_STOREDRECORDS) ? Gps_mainData.currentIdx : 0u);

      while (tmpBuffer != NULL) 
      { 
         tmpBuffer = strtok(NULL, ",");
         switch (sequence)
         {
            case GPS_GPGGA_TIME:
               strncpy(Gps_mainData.data[*idx].time, tmpBuffer, 6);
               break;
            case GPS_GPGGA_LATITUDE:
               sscanf(tmpBuffer, "%f", &Gps_mainData.data[*idx].latitude);
               break;
            case GPS_GPGGA_NS:
               sscanf(tmpBuffer, "%c", &Gps_mainData.data[*idx].NS);
               break;
            case GPS_GPGGA_LONGITUDE:
               sscanf(tmpBuffer, "%f", &Gps_mainData.data[*idx].longitude);
               break;
            case GPS_GPGGA_WE:
               sscanf(tmpBuffer, "%c", &Gps_mainData.data[*idx].WE);
               break;
            case GPS_GPGGA_FIXQUALITY:
               sscanf(tmpBuffer, "%d", &Gps_mainData.data[*idx].fixQuality);
               break;
            case GPS_GPGGA_SATELITESNUM:
               sscanf(tmpBuffer, "%d", &Gps_mainData.data[*idx].satelitesNum);
               break;
            case GPS_GPGGA_DILUTION:
               sscanf(tmpBuffer, "%f", &Gps_mainData.data[*idx].dilution);
               break;
            case GPS_GPGGA_ALTITUDE:
               sscanf(tmpBuffer, "%f", &Gps_mainData.data[*idx].altitude);
               break;
            default:
               break;
         }
         sequence++;
      }
      
      printf("Gps_mainData.data[%d].\n", *idx);
      printf("          time: %s\n",     Gps_mainData.data[*idx].time);
      printf("      latitude: %f\n",     Gps_mainData.data[*idx].latitude);
      printf("            NS: %c\n",     Gps_mainData.data[*idx].NS);      
      printf("     longitude: %f\n",     Gps_mainData.data[*idx].longitude);
      printf("            WE: %c\n",     Gps_mainData.data[*idx].WE);
      printf("    fixQuality: %d\n",     Gps_mainData.data[*idx].fixQuality);
      printf("  satelitesNum: %d\n",     Gps_mainData.data[*idx].satelitesNum);
      printf("      dilution: %f\n",     Gps_mainData.data[*idx].dilution);
      printf("      altitude: %f\n",     Gps_mainData.data[*idx].altitude);
      printf("\n");

      Gps_mainData.currentIdx++;
   }
}