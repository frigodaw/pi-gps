/**************************************************************************************
 ***********************************  StdTypes.h  *************************************
 *************************************************************************************/

#ifndef STD_TYPES_H
# define STD_TYPES_H

typedef unsigned char           boolean;    /*                FALSE .. TRUE                 */

typedef unsigned char           uint8;      /*                    0 .. 255                  */
typedef signed short            sint16;     /*      -         32768 .. +32767               */
typedef signed char             sint8;      /*                 -128 .. +127                 */
typedef unsigned short          uint16;     /*                    0 .. 65535                */
typedef signed long             sint32;     /*          -2147483648 .. +2147483647          */
typedef unsigned long           uint32;     /*                    0 .. 4294967296           */
typedef signed long long        sint64;     /* -9223372036854775808 .. +9223372036854775807 */
typedef unsigned long long      uint64;     /*                    0 .. 18446744073709551615 */

typedef float                   float32;
typedef double                  float64;

typedef uint8   Std_ReturnType;
# define OK     0u
# define NOK    1u

# define TRUE   1u
# define FALSE  0u



#endif  /* STD_TYPES_H */ 