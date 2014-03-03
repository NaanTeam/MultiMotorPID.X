/* 
 * File:   MathFunctions.h
 * Author: Connor
 *
 * Created on February 18, 2014, 8:36 PM
 */

#ifndef MATHFUNCTIONS_H
#define	MATHFUNCTIONS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    
#define ToRad(x) (x*0.01745329252)  // *pi/180
#define ToDeg(x) (x*57.2957795131)  // *180/pi

    
//Computes the dot product of two vectors
float Vector_Dot_Product(float vector1[3],float vector2[3]);

//Computes the cross product of two vectors
void Vector_Cross_Product(float vectorOut[3], float v1[3],float v2[3]);
//Multiply the vector by a scalar.
void Vector_Scale(float vectorOut[3],float vectorIn[3], float scale2);

void Vector_Add(float vectorOut[3],float vectorIn1[3], float vectorIn2[3]);
void Matrix_Multiply(float a[3][3], float b[3][3],float mat[3][3]);


#ifdef	__cplusplus
}
#endif

#endif	/* MATHFUNCTIONS_H */

