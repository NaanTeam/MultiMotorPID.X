/*
 * File:   HMC5883L.h
 * Author: Connor
 *
 * Created on November 10, 2013, 11:27 PM
 * Compass
 */

#ifndef HMC5883L_H
#define	HMC5883L_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include "FIFOI2C2.h"

    extern float HMC5883L_XMagneticVector;
    extern float HMC5883L_ZMagneticVector;
    extern float HMC5883L_YMagneticVector;


    void HMC5883L_startMeasurements();
    void HMC5883L_queueReadXZY();
    void HMC5883L_interpretXZY();


#ifdef	__cplusplus
}
#endif

#endif	/* HMC5883L_H */
