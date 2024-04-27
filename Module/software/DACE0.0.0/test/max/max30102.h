#pragma once

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#define intrpt_status_1 _u(0x00)
#define intrpt_status_2 _u(0x01)
#define intrpt_Enable_1 _u(0x02)
#define intrpt_Enable_2 _u(0x03)

#define FIFO_w_ptr    _u(0x04)
#define OVERFLOW_cnt  _u(0x05)
#define FIFO_r_ptr    _u(0x06)
#define FIFO_DATA_Reg _u(0x07)

#define FIFO_conf    _u(0x08)
#define MODE_conf    _u(0x09)
#define SpO2_conf    _u(0x0A)
#define LED_pls_Amp1 _u(0x0C)
#define LED_pls_Amp2 _u(0x0D)

#define MULTI_LED_Mode_REGcp1 _u(0x11)
#define MULTI_LED_Mode_REG2 _u(0x12)

#define REVISION_ID _u(0xFE)
#define PART_ID     _u(0xFF)



#define MAX_ADDRES _u(0x57)

typedef struct
{
    uint8_t PWR_RDY:1;    // Internal Temperature Ready Flag
    uint8_t NONE:4;
    uint8_t ALCOverflow:1; // Ambient Light Cancellation Overflow
    uint8_t dataReady:1;   // New FIFO Data Ready
    uint8_t almostFull:1;
    uint8_t NONE3:1;
    uint8_t DIE_TEMP_RDY:1;    
    uint8_t NONE2:6;
}intr_status_t;


//**********************************************************************//
//**********************************************************************//

typedef struct 
{
    uint8_t FIFO_A_FULL:4;
    uint8_t FIFO_ROL_LOVER_EN:1;
    uint8_t SMP_AVR:3;
}fifo_conf_t;

typedef struct 
{
    uint8_t MODE:3;
    uint8_t NONE:3;
    uint8_t RESET:1;
    uint8_t SHDN:1;
}mode_conf_t;

typedef struct 
{
    uint8_t LED_PW:2;
    uint8_t SPO2_SR:3;
    uint8_t SPO2_ADC_REG:2;
    uint8_t NONE:1;
}spo2_conf_t;



//--------------------adichumattiyate-----------------------------------//
//Configuration Options 
//FIFO Configuration(Register address 0x08)
//sampleAverage(Table 3. Sample Averaging)
#define SAMPLEAVG_1     0
#define SAMPLEAVG_2     1
#define SAMPLEAVG_4     2
#define SAMPLEAVG_8     3
#define SAMPLEAVG_16    4
#define SAMPLEAVG_32    5

//Mode configuration(Register address 0x09)
//ledMode(Table 4. Mode Control)
#define MODE_REDONLY    2
#define MODE_RED_IR     3
#define MODE_MULTILED   7

//Particle sensing configuration(Register address 0x0A)
//adcRange(Table 5. SpO2 ADC Range Control)
#define ADCRANGE_2048   0
#define ADCRANGE_4096   1
#define ADCRANGE_8192   2
#define ADCRANGE_16384  3
//sampleRate(Table 6. SpO2 Sample Rate Control)
#define SAMPLERATE_50   0 
#define SAMPLERATE_100  1
#define SAMPLERATE_200  2
#define SAMPLERATE_400  3
#define SAMPLERATE_800  4
#define SAMPLERATE_1000 5
#define SAMPLERATE_1600 6
#define SAMPLERATE_3200 7
//pulseWidth(Table 7. LED Pulse Width Control)
#define PULSEWIDTH_69   0 
#define PULSEWIDTH_118  1
#define PULSEWIDTH_215  2
#define PULSEWIDTH_411  3

//Multi-LED Mode Control Registers(Register address 0x011)
#define SLOT_NONE       0
#define SLOT_RED_LED    1
#define SLOT_IR_LED     2

//data buffer 
#define MAX30102_SENSE_BUF_SIZE  30

  typedef struct {
    uint32_t red[MAX30102_SENSE_BUF_SIZE];
    uint32_t IR[MAX30102_SENSE_BUF_SIZE];
    uint8_t head;
    uint8_t tail;
  } Sense_Data_Buf_t;

//**********************************************************************//
//**********************************************************************//

void sensor_config();
void get_FIFO_WR_PTR();
void max_init();
void get_intept_status();

uint8_t get_reg (uint8_t reg);
void set_reg (uint8_t reg, uint8_t txdata);

uint8_t getReadPointer();
uint8_t getWritePointer();

//**********************************************************************//
//**********************************************************************//