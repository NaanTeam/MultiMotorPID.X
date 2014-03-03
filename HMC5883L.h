/**
 * @file: HMC5883L.h
 * @brief: A set of function for interfacing with the HMC5883L Compass.
 *
 * @author: Connor Martin
 * @date: Oct 23, 2013
 *
 * @preconditions:
  *  -FIFOI2C2 is intialized.
 * @device:
 *      -PIC32MX695F512L
 *
 * @remarks:
 *
 */

#ifndef HMC5883L_H
#define	HMC5883L_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "FIFOI2C2.h"

//******************************************************************************
//Global Variables
//******************************************************************************
/** Contains the HMC5883L's X magnetic reading in gauss*/
extern float HMC5883L_XMagneticVector;
/** Contains the HMC5883L's Z magnetic reading in gauss*/
extern float HMC5883L_ZMagneticVector;
/** Contains the HMC5883L's Y magnetic reading in gauss*/
extern float HMC5883L_YMagneticVector;

/** Contains the HMC5883L's raw X magnetic reading*/
extern int16 HMC5883L_XMagneticVector_Raw;
/** Contains the HMC5883L's raw Z magnetic reading*/
extern int16 HMC5883L_ZMagneticVector_Raw;
/** Contains the HMC5883L's raw Y magnetic reading*/
extern int16 HMC5883L_YMagneticVector_Raw;


//******************************************************************************
//Function Prototypes
//******************************************************************************
/**
 * Turns on the HMC5883L device and has it begin measuring data.
 */
void HMC5883L_startMeasurements();
/**
 * Pushes a request to read X, Y and Z data from an HMC5883L onto
 * the SPI2's TX buffer.
 */
void HMC5883L_pushReadXZY();
/**
 * Pops X, Y, and Z values off the SPI2 stack and stores them
 * in the global raw HMC5883L variables.
 */
void HMC5883L_popXZY();
/**
 * Converts the global raw HMC5883L variables into the global float HMC5883L variables.
 */
void HMC5883L_convertXYZ();




#ifdef	__cplusplus
}
#endif

#endif	/* HMC5883L_H */

