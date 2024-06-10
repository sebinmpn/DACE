#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#pragma once
#ifndef _max30102_H_
#define _max30102_H_



// sda scl , gp0, gp1
#define MAX_SDA_pin 0
#define MAX_SCL_pin 1

static const uint8_t intrpt_status_1          =  _u(0x00);
static const uint8_t intrpt_status_2          =  _u(0x01);
static const uint8_t intrpt_Enable_1          =  _u(0x02);
static const uint8_t intrpt_Enable_2          =  _u(0x03);
static const uint8_t FIFO_w_ptr               =  _u(0x04);
static const uint8_t OVERFLOW_cnt             =  _u(0x05);
static const uint8_t FIFO_r_ptr               =  _u(0x06);
static const uint8_t FIFO_DATA_Reg            =  _u(0x07);
static const uint8_t FIFO_conf                =  _u(0x08);
static const uint8_t MODE_conf                =  _u(0x09);
static const uint8_t SpO2_conf                =  _u(0x0A);
static const uint8_t LED_pls_Amp1             =  _u(0x0C);
static const uint8_t LED_pls_Amp2             =  _u(0x0D);
static const uint8_t MULTI_LED_Mode_REGcp1    =  _u(0x11);
static const uint8_t MULTI_LED_Mode_REG2      =  _u(0x12);
static const uint8_t REVISION_ID              =  _u(0xFE);
static const uint8_t PART_ID                  =  _u(0xFF);
static const uint8_t MAX_ADDRES               =  _u(0x57);
static const uint8_t DIE_TEMP_INTEGER         =  _u(0x1F);
static const uint8_t DIE_TEMP_FRAC            =  _u(0x20);
static const uint8_t DIE_TEMP_CONFIG          =  _u(0x21);
static const uint8_t INT_DIE_TEMP_RDY_ENABLE  =  _u(0x02);
static const uint8_t INT_DIE_TEMP_RDY_DESABLE =  _u(0x00);

//**********************************************************************//
//**********************************************************************//

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
}intr_status_t; //sEnable_t;   (pg 12,13)



typedef struct 
{
    uint8_t FIFO_A_FULL:4;
    uint8_t FIFO_ROL_LOVER_EN:1;
    uint8_t SMP_AVR:3;
}fifo_conf_t; // sFIFO_t; (pg 17)

typedef struct 
{
    uint8_t LedMode:6;
    uint8_t RESET:1;
    uint8_t SHDN:1;
}mode_conf_t; // sMode_t; (pag 18)

typedef struct 
{
    uint8_t LED_PW:2;
    uint8_t SPO2_SR:3;
    uint8_t SPO2_ADC_REG:2;
    uint8_t NONE:1;
}spo2_conf_t; // 



  typedef struct {
    uint8_t   pulseWidth:2;
    uint8_t   sampleRate:3;
    uint8_t   adcRange:3;
  } sParticle_t; //(pg 18)

#define MAX30102_SENSE_BUF_SIZE 30

  typedef struct {
    uint32_t red[MAX30102_SENSE_BUF_SIZE];
    uint32_t IR[MAX30102_SENSE_BUF_SIZE];
    uint8_t head;
    uint8_t tail;
  } sSenseBuf_t;

  typedef struct {
    uint8_t   SLOT1:4;
    uint8_t   SLOT2:4;
  }  sMultiLED_t; //  (pg 21)

  uint8_t _activeLEDs;
  sSenseBuf_t senseBuf;

//**********************************************************************//
//**********************************************************************//


void max_init();
void get_intept_status();

uint8_t max_reset();
unsigned int get_temp();

void get_FIFO_WR_PTR();
uint8_t read_reg (uint8_t reg, void* data, uint8_t size);
uint8_t  get_reg (uint8_t reg);
uint8_t set_reg(uint8_t reg, uint8_t data);
uint8_t write_reg (uint8_t reg, uint8_t *data, int size);


void sensorConfiguration(uint8_t ledBrightness, uint8_t sampleAverage, uint8_t ledMode, uint8_t sampleRate, uint8_t pulseWidth, uint8_t adcRange);


void setLEDMode(uint8_t ledMode);
void setFIFOAverage(uint8_t numberOfSamples);

void enableFIFORollover(void);
void disableFIFORollover(void);

void setADCRange(uint8_t adcRange); 
void setSampleRate(uint8_t sampleRate);
void setPulseWidth(uint8_t pulseWidth);
void setPulseAmplitudeRed(uint8_t amplitude);
void setPulseAmplitudeIR(uint8_t amplitude);

void enableSlot(uint8_t slotNumber, uint8_t device);
void disableAllSlots(void); 
void resetFIFO(void);
uint8_t getWritePointer(void);
uint8_t getReadPointer(void);
void softReset(void);

uint32_t getRed(void);
uint32_t getIR(void);
void getNewData(void);


//**********************************************************************//
//**********************************************************************//


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

#endif