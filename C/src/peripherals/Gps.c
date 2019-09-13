#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "Gps.h"

/* Executes only one time to initialize data */
Std_ReturnType Gps_Init(void)
{
   Std_ReturnType retVal = OK;

   retVal |= Gps_SerialInit();
   retVal |= Gps_resetDataInit();

   return retVal;
}

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

      //Gps_GetFilteredData_GPGGA(Gps_gpggaOverride);
      Gps_GetFilteredData_GPGGA(dataBuffer);
   }
}

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
   Gps_serialPortSettings.c_cc[VTIME] = 0; /* Wait indefinetly   */

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

Std_ReturnType Gps_resetDataInit(void)
{
   Std_ReturnType retVal = OK;


   return retVal;
}

/* Read raw data from serial */
Std_ReturnType Gps_GetData(char* buffer)
{
   Std_ReturnType retVal = OK;
   uint16 bytesRead = 0;    /* Number of bytes read by the read() system call */

   memset(buffer, 0, GPS_BUFFERSIZE);                                         /* Set zeros to buffer */
   tcflush(Gps_rawData.fileDescriptor, TCIFLUSH);                             /* Discards old data in the rx buffer */
   bytesRead = read(Gps_rawData.fileDescriptor, buffer, GPS_BUFFERSIZE);      /* Read the data                      */

   if(bytesRead == - 1)
   {
      printf("Error in reading data from serial port!  \n");
      retVal |= NOK;
   }
   else
   {
      /* ok */
      //printf("%s  \n", buffer);       //debug
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
   if(!strncmp(buffer, sentence, GPS_SENTENCELENGTH))
   {
      /* Select place to put new gps data */
      if(Gps_rawData.currentIdx[sentecePosition] < GPS_STOREDRECORDS)
      {
         /* ok */
      }
      else
      {
         Gps_rawData.currentIdx[sentecePosition] = 0;
      }

      /* Replace the oldest readout with the current one */
      strncpy(Gps_rawData.storageBuffer[sentecePosition][Gps_rawData.currentIdx[sentecePosition]], buffer, GPS_BUFFERSIZE);
      printf("%d) %s ", Gps_rawData.currentIdx[sentecePosition], Gps_rawData.storageBuffer[sentecePosition][Gps_rawData.currentIdx[sentecePosition]]); //debug
      
      Gps_rawData.currentIdx[sentecePosition]++;
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
      buffer = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47";

      uint8 *idx = &Gps_mainData.currentIdx;
      char tmpBuffer[GPS_BUFFERSIZE] = {0};

      *idx = ((Gps_mainData.currentIdx < GPS_STOREDRECORDS) ? Gps_mainData.currentIdx : 0);
      printf("*idx=%d \nGps_mainData.currentIdx = %d\n", *idx, Gps_mainData.currentIdx);    //debug


      sint8 i = 0;
      uint8 sequence = GPS_GPGGA_TIME;

      for(uint8 j = 0; j < 66; j++)
      {
         /* Add chars to temporary buffer if conditions are met */
         if((isdigit(*buffer)) || (*buffer == '.'))
         {
            tmpBuffer[i] = *buffer;
            i++;
         }
         /* When all data have arrived, check in which place it should be located */
         else if((*buffer == ',') && (isdigit(*tmpBuffer)))
         {
            switch (sequence)
            {
               case GPS_GPGGA_TIME:
                  strncpy(Gps_mainData.data[*idx].time, tmpBuffer, 9);
                  break;
               case GPS_GPGGA_LATITUDE:
                  printf("tmpBuffer = %s\n",tmpBuffer);
                  Gps_mainData.data[*idx].latitude = atoi(tmpBuffer);
                  break;
               default:
                  break;
            }
            sequence++;
            memset(tmpBuffer, 0, GPS_BUFFERSIZE);
         }
         *buffer++;
      }

      printf("Gps_mainData.data[%d].time: %s\n", *idx, Gps_mainData.data[*idx].time);              //debug
      printf("Gps_mainData.data[%d].latitude: %s\n", *idx, Gps_mainData.data[*idx].latitude);      //debug
      printf("\n");
      Gps_mainData.currentIdx++;
   }
}