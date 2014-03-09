
#include "SensorLoop.h"

//******************************************************************************
//Public Function Definitions
//******************************************************************************
void SensorLoop_SetupAll()
{
    //Setup SPI and I2C
    FIFOSPI2_initialize();
    FIFOI2C2_initialize();

    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableInterrupts();
    
////    //Setup Accelerometer
    ADXL362_startMeasurements();
////    //Setup Gyroscope
    L3G4200D_startMeasurements();
////   //Setup 3-axis compass
    HMC5883L_startMeasurements();

    INTDisableInterrupts();


    //Setup Timer1
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_3);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);
    //Turn on clock
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_64, 0x4FFF);
    
}


//******************************************************************************
//Interrupt Request Routines
//******************************************************************************
//TODO KNOWN BUG: with SPI. Reordering ADXl and L3G will make L3G's reading bad.
void __ISR(_TIMER_1_VECTOR, IPL3AUTO) Timer1Handler(void)
{

    //Interpret the previously read data.
    ADXL362_popXYZT();
    L3G4200D_popXYZT();
    HMC5883L_popXZY();
    
    //Convert Raw data into meaningful data(optional and potential optimization)
    ADXL362_convertXYZT();
    L3G4200D_convertXYZT();
    HMC5883L_convertXYZ();


    //Que reads to the sensors for next timer tick.
    ADXL362_pushReadXYZT();
    L3G4200D_pushReadXYZT();
    HMC5883L_pushReadXZY();


    INTClearFlag(INT_T1);// Be sure to clear the Timer1 interrupt status
}
