/* 
 * File:   Hardware.h
 * Author: Connor
 *
 * Created on November 9, 2013, 9:39 PM
 */

#ifndef HARDWARE_H
#define	HARDWARE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    #if defined (__32MX695F512L__) || (__32MX320F128H__)

        #define SYS_FREQ (80000000uL)
        #define GetSystemClock()           (SYS_FREQ)
        #define GetInstructionClock()      (SYS_FREQ)
        #define GetPeripheralClock()       (SYS_FREQ/(1 << OSCCONbits.PBDIV))

    #endif
    


    typedef unsigned char uint8;
    typedef signed char int8;
    typedef unsigned short uint16;
    typedef signed short int16;
    typedef unsigned int uint32;
    typedef signed int int32;
    
    typedef unsigned char boolean;

   

#ifdef	__cplusplus
}
#endif

#endif	/* HARDWARE_H */

