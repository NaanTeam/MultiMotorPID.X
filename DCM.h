// Released under Creative Commons 3.0 BY-SA
// Code by Jordi Munoz and William Premerlani, Supported by Chris Anderson and Doug Weibel
// Version 1.0 for flat board updated by Doug Weibel, Jose Julio and Ahmad Byagowi
// Version 1.7 includes support for SCP1000 absolute pressure sensor
// Version 1.8 uses DIYDrones GPS, FastSerial, and Compass libraries
// Version 1.9 Support for ArduIMU V3 Hardware with MPU6000 and HMC5883 magnetometer (SCP1000 absolute pressure sensor is not supported in this version)
// Version 1.9.6 Works on Arduino 1.0.1 IDE
// Version 1.9.7 Roll and Pitch are Calibrates
// Version 1.9.8 Modified Ready For Quadrotor projects Edwin Babaians , Michel Kogan works with MPU6000 vD  ( Thanks to Artem Grigoryev For Solutins )
// for mor info on mpu6000 vD and Artem Grigoryev solution :
// http://diydrones.com/forum/topics/arduimu-v3-all-axes-output-wrong-values

//Modified: 2/22/14 Connor Martin



#ifndef DCM_H
#define	DCM_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include "Hardware.h"
    #include "math.h"
    #include "MathFunctions.h"
    #include "Orientation.h"

    //#define Kp_ROLLPITCH 0.015
    //#define Ki_ROLLPITCH 0.000010

    #define Kp_ROLLPITCH 10.0/GRAVITY
    #define Ki_ROLLPITCH 0.5/GRAVITY

    #define Kp_YAW 10.0
    //#define Kp_YAW 2.5      //High yaw drift correction gain - use with caution!
    #define Ki_YAW 0.5



    
    void DCM_driftCorrection(float* accelVector, float scaledAccelMagnitude, float magneticHeading);
    void DCM_matrixUpdate(float timeDiff, float* gyroScaled);
    void DCM_normalize(void);
    void DCM_eulerAngle(float* roll, float* pitch, float* yaw);




#ifdef	__cplusplus
}
#endif

#endif	/* DCM_H */

