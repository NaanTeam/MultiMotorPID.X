
#include "L3G4200D.h"

//******************************************************************************
//Public Variable Declarations
//******************************************************************************
float L3G4200D_XAngularRate = 0;
float L3G4200D_YAngularRate = 0;
float L3G4200D_ZAngularRate = 0;
float L3G4200D_Temperature = 0;

int16 L3G4200D_XAngularRate_Raw = 0;
int16 L3G4200D_YAngularRate_Raw = 0;
int16 L3G4200D_ZAngularRate_Raw = 0;
int16 L3G4200D_Temperature_Raw = 0;

//******************************************************************************
//Public Function Definitions
//******************************************************************************
int L3G4200D_startMeasurements()
{
    //Check to see if we are communicating correctly.
    if (L3G4200D_readRegister_Blocking(L3G4200D_Reg_WHOAMI) != 0xD3)
    {
        return -1;
    }


    unsigned char CtrlReg1 = 0;
    //Basicaly turn the device on.
    CtrlReg1 |= 0x00 |
            (0 << L3G4200D_RegBit_DR) | //100 hz ODR
            (0 << L3G4200D_RegBit_BW) | //12.5 Cutoff
            (1 << L3G4200D_RegBit_PD) | //Turn's off Power Down
            (1 << L3G4200D_RegBit_Zen) | //Enables Z axis
            (1 << L3G4200D_RegBit_Yen) | //Enables Y axis
            (1 << L3G4200D_RegBit_Xen); //Enables X axis
    L3G4200D_writeRegister_Blocking(L3G4200D_Reg_CTRLREG1, CtrlReg1);


    //High Pass filter Settings
    unsigned char CtrlReg2 = 0;
    CtrlReg2 |= 0;
    L3G4200D_writeRegister_Blocking(L3G4200D_Reg_CTRLREG2, CtrlReg2);


    //Interupt and FIFO Stuff
    unsigned char CtrlReg3 = 0;
    CtrlReg3 |= 0;
    L3G4200D_writeRegister_Blocking(L3G4200D_Reg_CTRLREG3, CtrlReg3);


    //Data update, endian mode, scale selection, self test, SPI wire mode
    unsigned char CtrlReg4 = 0;
    CtrlReg4 |= 0x00 |
            (0 << L3G4200D_RegBit_FS); //Full scale selection of 250 dps.
    L3G4200D_writeRegister_Blocking(L3G4200D_Reg_CTRLREG4, CtrlReg4);

    
    //reboot, fifo enable, interupt stuff
    unsigned char CtrlReg5 = 0;
    CtrlReg5 |= 0;
    L3G4200D_writeRegister_Blocking(L3G4200D_Reg_CTRLREG5, CtrlReg5);

}

void L3G4200D_pushWriteRegister(unsigned char reg, unsigned char value)
{
    //Control Register one
    char buff[3];
    buff[0] = reg; //Register ADDR.. Defualt return: 11010011 .. 0xD3
    buff[0] &= 0x7F; //Sets the write flag
    buff[1] = value;
    FIFOSPI2_pushTxQueue(buff, 2, L3G4200D_SLAVE_SELECT_LINE);
}
void L3G4200D_pushReadRegister(unsigned char reg)
{
    char buff[3];
    buff[0] = reg;
    //Sets the read flag
    buff[0] |= (1 << 7);
    buff[1] = 0x00; //Fluff
    FIFOSPI2_pushTxQueue(buff, 2, L3G4200D_SLAVE_SELECT_LINE);
}

//TODO: Fix so I can get rid of ReceiveBufferIndex function
void L3G4200D_writeRegister_Blocking(unsigned char reg, unsigned char value)
{
    //Control Register one
    char buff[3];
    buff[0] = reg; //Register ADDR.. Defualt return: 11010011 .. 0xD3
    buff[0] &= 0x7F; //Sets the write flag
    buff[1] = value;
    FIFOSPI2_pushTxQueue(buff, 2, L3G4200D_SLAVE_SELECT_LINE);


    char func_rslt, read_rslt;
    while (FIFOSPI2_rxBufferIndex() < 2 || FIFOSPI2_isRunnning != 0) {}
    func_rslt = FIFOSPI2_popRxQueue(&read_rslt);
    func_rslt = FIFOSPI2_popRxQueue(&read_rslt);
}


//TODO: Change things into unsigned char
unsigned char L3G4200D_readRegister_Blocking(unsigned char reg)
{
    
    char buff[3];
    buff[0] = reg; 
    //Sets the read flag
    buff[0] |= (1 << 7);
    buff[1] = 0x00; //Fluff
    FIFOSPI2_pushTxQueue(buff, 2, L3G4200D_SLAVE_SELECT_LINE);

    
    //Force wait for the end of the transmission
    char func_rslt, read_rslt;
    while (FIFOSPI2_rxBufferIndex() < 2 || FIFOSPI2_isRunnning != 0) {}
    func_rslt = FIFOSPI2_popRxQueue(&read_rslt);
    func_rslt = FIFOSPI2_popRxQueue(&read_rslt);

    return read_rslt;
}


void L3G4200D_pushReadXYZT()
{
    
    L3G4200D_pushReadRegister(L3G4200D_Reg_OUTXL);
    L3G4200D_pushReadRegister(L3G4200D_Reg_OUTXH);
    L3G4200D_pushReadRegister(L3G4200D_Reg_OUTYL);
    L3G4200D_pushReadRegister(L3G4200D_Reg_OUTYH);
    L3G4200D_pushReadRegister(L3G4200D_Reg_OUTZL);
    L3G4200D_pushReadRegister(L3G4200D_Reg_OUTZH);
    L3G4200D_pushReadRegister(L3G4200D_Reg_OUTTEMP);
    
}

void L3G4200D_popXYZT()
{
    uint8 func_rslt, fluff;
    
    uint8 x_msb = 0, x_lsb = 0;
    uint8 y_msb = 0, y_lsb = 0;    
    uint8 z_msb = 0, z_lsb = 0;
    uint8 temp;
    
    short x_16b = 0, y_16b = 0, z_16b= 0;



    //X axis
    func_rslt = FIFOSPI2_popRxQueue(&fluff); //fluff
    func_rslt = FIFOSPI2_popRxQueue(&x_lsb); //X low
    func_rslt = FIFOSPI2_popRxQueue(&fluff); //fluff
    func_rslt = FIFOSPI2_popRxQueue(&x_msb); //X hi
    //Y axis
    func_rslt = FIFOSPI2_popRxQueue(&fluff); //fluff
    func_rslt = FIFOSPI2_popRxQueue(&y_lsb); //Y low
    func_rslt = FIFOSPI2_popRxQueue(&fluff); //fluff
    func_rslt = FIFOSPI2_popRxQueue(&y_msb); //Y hi
    //Z axis
    func_rslt = FIFOSPI2_popRxQueue(&fluff); //fluff
    func_rslt = FIFOSPI2_popRxQueue(&z_lsb); //Z low
    func_rslt = FIFOSPI2_popRxQueue(&fluff); //fluff
    func_rslt = FIFOSPI2_popRxQueue(&z_msb); //Z hi
    //Temperature
    func_rslt = FIFOSPI2_popRxQueue(&fluff); //fluff
    func_rslt = FIFOSPI2_popRxQueue(&temp); //Temp
    
    
    x_16b = (x_msb << 8) | x_lsb;
    y_16b = (y_msb << 8) | y_lsb;
    z_16b = (z_msb << 8) | z_lsb;


    L3G4200D_XAngularRate_Raw = x_16b;
    L3G4200D_YAngularRate_Raw = -1*y_16b;
    L3G4200D_ZAngularRate_Raw = -1*z_16b;
    L3G4200D_Temperature_Raw = temp;
}

void L3G4200D_convertXYZT()
{
    float dps_per_LSB = 0;
    float C_per_LSB = 0;

    //'degress per second' per least significant bit.
    dps_per_LSB = 8.75e-3;//From L3G4200D data sheet (250dps mode)
    //celsius per least significant bit (Check L3G4200D datasheet)
    C_per_LSB = 1;
    
    L3G4200D_XAngularRate = dps_per_LSB * (float)L3G4200D_XAngularRate_Raw;
    L3G4200D_YAngularRate = dps_per_LSB * (float)L3G4200D_YAngularRate_Raw;
    L3G4200D_ZAngularRate = dps_per_LSB * (float)L3G4200D_ZAngularRate_Raw;
    L3G4200D_Temperature = C_per_LSB * (float)L3G4200D_Temperature_Raw;
    
}
