
#include "Orientation.h"

float Orientation_Pitch = 0.0;
float Orientation_Yaw = 0.0;
float Orientation_Roll = 0.0;



void Orientation_Calculate()
{
//    float pitch = 0.0;
//    float roll = 0.0;

    //Calc Pitch
    Orientation_Pitch = atan(ADXL362_XAcceleration /
                sqrt(ADXL362_YAcceleration*ADXL362_YAcceleration +
                ADXL362_ZAcceleration*ADXL362_ZAcceleration));
    
    
    //Calc Yaw

    //Calculate Roll
    Orientation_Roll = atan(ADXL362_YAcceleration /
                sqrt(ADXL362_XAcceleration*ADXL362_XAcceleration +
                ADXL362_ZAcceleration*ADXL362_ZAcceleration));


}



