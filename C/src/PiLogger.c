#include <stdio.h>
#include "PiLogger.h"
#include "Gps.h"


void PiLogger_Init(void)
{
   printf("\n +----------------------------------+");
   printf("\n |          Pi-Gps start            |");
   printf("\n +----------------------------------+ \n\n");

   Gps_Init();
}

void PiLogger_Cyclic(void)
{
   Gps_Main();
}

void PiLogger_End(void)
{
   Gps_TerminateConnection();
}

int main()
{
   PiLogger_Init();


   while(1)
   {
      PiLogger_Cyclic();
   }


   PiLogger_End();

   return 0;
}