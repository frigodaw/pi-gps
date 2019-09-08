#include <stdio.h>
#include "Gps.h"

Std_ReturnType Gps_Init(void)
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


Std_ReturnType Gps_ReadData(void)
{
   Std_ReturnType retVal = OK;
   uint16 bytesRead = 0;    /* Number of bytes read by the read() system call */
   char dataBuffer[GPS_BUFFERSIZE];

   memset(dataBuffer, 0, GPS_BUFFERSIZE);                                        /* Set zeros to buffer */
   tcflush(Gps_rawData.fileDescriptor, TCIFLUSH);                                /* Discards old data in the rx buffer */
   bytesRead = read(Gps_rawData.fileDescriptor, &dataBuffer, GPS_BUFFERSIZE);    /* Read the data                      */

   if(bytesRead == - 1)
   {
      printf("Error in reading data from serial port!  \n");
      retVal |= NOK;
   }
   else
   {
      //printf("%s  \n", dataBuffer);       //debug

      /* Start of data filtering, check whether data starts with given string */
      Gps_GetData(dataBuffer, GPS_GPGGA, GPS_ENUM_GPGGA);
      //Gps_GetData(dataBuffer, GPS_GPRMC, GPS_ENUM_GPRMC);

   }
   return retVal;
}


void Gps_TerminateConnection(void)
{
   /* Close the serial port */
   close(Gps_rawData.fileDescriptor); 
}

void Gps_GetData(char* buffer, char* sentence, uint8 sentecePosition)
{
   if(!strncmp(buffer, sentence, GPS_SENTENCELENGTH))
   {
      /* Select place to put new gps data */
      if(Gps_rawData.bufferPtr[sentecePosition] < GPS_STOREDRECORDS)
      {
         /* ok */
      }
      else
      {
         Gps_rawData.bufferPtr[sentecePosition] = 0;
      }

      /* Replace the oldest readout with the current one */
      strncpy(Gps_rawData.storageBuffer[sentecePosition][Gps_rawData.bufferPtr[sentecePosition]], buffer, GPS_BUFFERSIZE);
      printf("%d) %s \n", Gps_rawData.bufferPtr[sentecePosition], Gps_rawData.storageBuffer[sentecePosition][Gps_rawData.bufferPtr[sentecePosition]]); //debug
      
      Gps_rawData.bufferPtr[sentecePosition]++;
   }
   else
   {
      /* no correct data */
   }
}

void Gps_FilterDetailedData_GPGGA()
{
   
}