/* 
 * File:   Orientation.h
 * Author: Connor
 *
 * Created on February 13, 2014, 5:22 PM
 */

#ifndef ORIENTATION_H
#define	ORIENTATION_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include "math.h"
    #include "ADXL362.h"

    extern float Orientation_Pitch;
    extern float Orientation_Yaw;
    extern float Orientation_Roll;

    void Orientation_Calculate();
    


#ifdef	__cplusplus
}
#endif

#endif	/* ORIENTATION_H */

