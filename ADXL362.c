
#include "ADXL362.h"

//******************************************************************************
//Public Variable Declarations
//******************************************************************************
float ADXL362_XAcceleration = 0;
float ADXL362_YAcceleration = 0;
float ADXL362_ZAcceleration = 0;
float ADXL362_Temperature = 0;

int16 ADXL362_XAcceleration_Raw = 0;
int16 ADXL362_YAcceleration_Raw = 0;
int16 ADXL362_ZAcceleration_Raw = 0;
int16 ADXL362_Temperature_Raw = 0;


//******************************************************************************
//Public Function Definitions
//******************************************************************************
void ADXL362_startMeasurements()
{
    //Reset the device
    
    char func_rslt, read_rslt1;
    char reset[3];
    reset[0] = ADXL362_REG_WRITE;
    reset[1] = ADXL362_SOFT_RESET;
    reset[2] = ADXL362_SOFT_RESET_KEY; //Represents the letter r
    FIFOSPI2_pushTxQueue(reset, 3, ADXL362_SLAVE_SELECT_LINE);
    //Wait for the device to reset
    int i = 0;
    while (i < (1000))
    {
        i++;
    }
    func_rslt = FIFOSPI2_popRxQueue(&read_rslt1);
    func_rslt = FIFOSPI2_popRxQueue(&read_rslt1);
    func_rslt = FIFOSPI2_popRxQueue(&read_rslt1);


    //Turns on Measurement Mode
    char measurementMode[3];
    measurementMode[0] = ADXL362_REG_WRITE;
    measurementMode[1] = ADXL362_POWER_CTL;
    measurementMode[2] = 0x00 |
            ADXL362_MEASURE_3D; //Turn on Measurement mode
    FIFOSPI2_pushTxQueue(measurementMode, 3, ADXL362_SLAVE_SELECT_LINE);


    //Immediately Follow it with a read X register
    char readReg[3];
    readReg[0] = ADXL362_REG_READ; //Read
    readReg[1] = ADXL362_XDATA8; //read X-reg
    readReg[2] = 0x00; //fluff for the read
    FIFOSPI2_pushTxQueue(measurementMode, 3, ADXL362_SLAVE_SELECT_LINE);


    //Wait for all data to be sent
    while (FIFOSPI2_rxBufferIndex() < 6) {}
    func_rslt = FIFOSPI2_popRxQueue(&read_rslt1);
    func_rslt = FIFOSPI2_popRxQueue(&read_rslt1);
    func_rslt = FIFOSPI2_popRxQueue(&read_rslt1);
    func_rslt = FIFOSPI2_popRxQueue(&read_rslt1);
    func_rslt = FIFOSPI2_popRxQueue(&read_rslt1);
    func_rslt = FIFOSPI2_popRxQueue(&read_rslt1);
    //k = FIFOSPI2_ReadQueue(&j);
}

void ADXL362_pushReadXYZT()
{
    //Burst read
    char read[10];
    read[0] = ADXL362_REG_READ; //Read
    read[1] = ADXL362_XDATAL; //read X-reg
    read[2] = 0x00; //ADXL362_XDATAL
    read[3] = 0x00; //ADXL362_XDATAH
    read[4] = 0x00; //ADXL362_YDATAL
    read[5] = 0x00; //ADXL362_YDATAH
    read[6] = 0x00; //ADXL362_ZDATAL
    read[7] = 0x00; //ADXL362_ZDATAH
    read[8] = 0x00; //ADXL362_TEMPL
    read[9] = 0x00; //ADXL362_TEMPH
    FIFOSPI2_pushTxQueue(read, 10, ADXL362_SLAVE_SELECT_LINE);
}
void ADXL362_interpretXYZT()
{    
    uint8 func_rslt, fluff;
    
    uint8 x_msb = 0, x_lsb = 0;
    uint8 y_msb = 0, y_lsb = 0;    
    uint8 z_msb = 0, z_lsb = 0;
    uint8 temp_msb = 0, temp_lsb = 0;
    
    short x_16b = 0, y_16b = 0, z_16b= 0, temp_16b = 0;




    func_rslt = FIFOSPI2_popRxQueue(&fluff); //fluff
    func_rslt = FIFOSPI2_popRxQueue(&fluff); //fluff

    func_rslt = FIFOSPI2_popRxQueue(&x_lsb); //ADXL362_XDATAL
    func_rslt = FIFOSPI2_popRxQueue(&x_msb); //ADXL362_XDATAH

    func_rslt = FIFOSPI2_popRxQueue(&y_lsb); //ADXL362_YDATAL
    func_rslt = FIFOSPI2_popRxQueue(&y_msb); //ADXL362_YDATAH

    func_rslt = FIFOSPI2_popRxQueue(&z_lsb); //ADXL362_ZDATAL
    func_rslt = FIFOSPI2_popRxQueue(&z_msb); //ADXL362_ZDATAH

    func_rslt = FIFOSPI2_popRxQueue(&temp_lsb); //ADXL362_TEMPL
    func_rslt = FIFOSPI2_popRxQueue(&temp_msb); //ADXL362_TEMPH
    
    
    x_16b = (x_msb << 8) | x_lsb;
    y_16b = (y_msb << 8) | y_lsb;
    z_16b = (z_msb << 8) | z_lsb;
    temp_16b = (temp_msb << 8) | temp_lsb;

    ADXL362_XAcceleration_Raw = x_16b;
    ADXL362_YAcceleration_Raw = y_16b;
    ADXL362_ZAcceleration_Raw = z_16b;
    ADXL362_Temperature_Raw = temp_16b;
    
}
void ADXL362_convertXYZT()
{
        float G_per_LSB = 0;
    float C_per_LSB = 0;

    //gravitiys per least significant bit (Check ADXL362 datasheet)
    G_per_LSB = 1e-3; //For the 2g range
    //celsius per least significant bit (Check ADXL362 datasheet)
    C_per_LSB = 0.065;

    ADXL362_XAcceleration = G_per_LSB * (float)ADXL362_XAcceleration_Raw;
    ADXL362_YAcceleration = G_per_LSB * (float)ADXL362_YAcceleration_Raw;
    ADXL362_ZAcceleration = G_per_LSB * (float)ADXL362_ZAcceleration_Raw;
    ADXL362_Temperature = C_per_LSB * (float)ADXL362_Temperature_Raw;
}

//
//void ADXL362_queueReadMSBX()
//{
//    char read[3];
//    read[0] = ADXL362_REG_READ; //Read
//    read[1] = ADXL362_XDATA8; //read X-reg
//    read[2] = 0x00; //fluff for read (8 MSB of X accel reg)
//    FIFOSPI2_addQueue(read, 3, 1);
//}
//void ADXL362_interpretMSBX()
//{
//    char func_rslt, read_rslt1, read_rslt2;
//
//    func_rslt = FIFOSPI2_readQueue(&read_rslt1); //fluff
//    func_rslt = FIFOSPI2_readQueue(&read_rslt2); //fluff
//
//    func_rslt = FIFOSPI2_readQueue(&read_rslt1); //ADXL362_XDATA8 (8 MSB)
//    ADXL362_XAcceleration = read_rslt1 ;
//
//}
//
//void ADXL362_queueReadMSBY()
//{
//    char read[3];
//    read[0] = ADXL362_REG_READ; //Read
//    read[1] = ADXL362_YDATA8; //read Y-reg
//    read[2] = 0x00; //fluff
//    FIFOSPI2_addQueue(read, 3, 1);
//}
//void ADXL362_interpretMSBY()
//{
//    char func_rslt, read_rslt1, read_rslt2;
//
//    func_rslt = FIFOSPI2_readQueue(&read_rslt1); //fluff
//    func_rslt = FIFOSPI2_readQueue(&read_rslt2); //fluff
//
//    func_rslt = FIFOSPI2_readQueue(&read_rslt1); //ADXL362_YDATA8 (Y 8-MSB)
//    ADXL362_YAcceleration = read_rslt1 ;
//
//}
//
//void ADXL362_queueReadMSBZ()
//{
//    char read[3];
//    read[0] = ADXL362_REG_READ; //Read
//    read[1] = ADXL362_ZDATA8; //read Z-reg
//    read[2] = 0x00; //fluff
//    FIFOSPI2_addQueue(read, 3, 1);
//}
//void ADXL362_InterpretMSBZ()
//{
//    char func_rslt, read_rslt1, read_rslt2;
//    func_rslt = FIFOSPI2_readQueue(&read_rslt1); //fluff
//    func_rslt = FIFOSPI2_readQueue(&read_rslt2); //fluff
//
//    func_rslt = FIFOSPI2_readQueue(&read_rslt1); //ADXL362_ZDATA8 (Z 8-MSB)
//    ADXL362_ZAcceleration = read_rslt1 ;
//
//}

