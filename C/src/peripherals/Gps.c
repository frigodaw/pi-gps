#include <stdio.h>
#include "Gps.h"

Std_ReturnType Gps_Init(void)
{
    printf("Gps_Init\n");
    printf("Dir: %s\n", Gps_serialDirectory);
    
    return 0;
}