#include "max30102.h"


// //**********************************************************************//
// //**********************************************************************//

void max_init() {
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(MAX_SDA_pin, GPIO_FUNC_I2C);
    gpio_set_function(MAX_SCL_pin, GPIO_FUNC_I2C);
    gpio_pull_up(MAX_SDA_pin);
    gpio_pull_up(MAX_SCL_pin);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(MAX_SDA_pin, MAX_SCL_pin, GPIO_FUNC_I2C));
    return;
}

//----------------------------------------------------------------------//

uint8_t max_reset()
{
    uint8_t reset[2];
    uint8_t resetstate[1];

    reset[0] = 0x09;
    reset[1] = 0x40;
    i2c_write_blocking(i2c_default, MAX_ADDRES, reset, 2, true);

    while (true)
    {
        i2c_read_blocking(i2c_default, MAX_ADDRES, resetstate, 1, true);
        if(resetstate[0] == 0xf )
            return resetstate[0];
    }
}

//----------------------------------------------------------------------//

unsigned int get_temp()
    {
        uint8_t intenable2[2];
        intenable2[0] = intrpt_Enable_2;
        intenable2[1] = INT_DIE_TEMP_RDY_ENABLE;

        uint8_t dietempconfig[2];
        dietempconfig[0] = DIE_TEMP_CONFIG;
        dietempconfig[1] = 0x01;

        uint8_t intstate2[1];
        uint8_t dietemp[1];

        // get_reg(PART_ID);

        i2c_write_blocking(i2c_default, MAX_ADDRES, intenable2, 2, true);
        i2c_write_blocking(i2c_default, MAX_ADDRES, dietempconfig, 2, true);  

        while (true)
        {
            i2c_write_blocking(i2c_default, MAX_ADDRES, &intrpt_status_2, 1, true);
            i2c_read_blocking(i2c_default, MAX_ADDRES, intstate2, 1, false);

            if ((intstate2[0] & INT_DIE_TEMP_RDY_ENABLE) > 0 )  break; 
        }

        i2c_write_blocking(i2c_default, MAX_ADDRES, &DIE_TEMP_INTEGER, 1, true);
        i2c_read_blocking(i2c_default, MAX_ADDRES, dietemp, 1, false);

        unsigned int int_value = dietemp[0];
        return int_value;

    }

//----------------------------------------------------------------------//

void get_intept_status()
{
    intr_status_t rxdata;
    uint8_t inter_addrs = intrpt_status_1;

    i2c_write_blocking(i2c_default, MAX_ADDRES, &inter_addrs, 1, true);
    i2c_read_blocking(i2c_default, MAX_ADDRES, &rxdata, 1, false);
    printf("interrupt status %x\n", rxdata);
    if(rxdata.PWR_RDY) puts("hihi");
}

//----------------------------------------------------------------------//

void get_FIFO_WR_PTR()
{

    uint8_t rxdata;
    uint8_t inter_addrs = FIFO_w_ptr;

    i2c_write_blocking(i2c_default, MAX_ADDRES, &inter_addrs, 1, true);
    i2c_read_blocking(i2c_default, MAX_ADDRES, &rxdata, 1, false);
    if(rxdata != 0){
    puts("Fifi w ptr");
    puts(rxdata);
    } 
}

//----------------------------------------------------------------------//

uint8_t read_reg (uint8_t reg, void* data, uint8_t size)
{
    i2c_write_blocking(i2c_default, MAX_ADDRES, &reg, 1, true);
    i2c_read_blocking(i2c_default, MAX_ADDRES, data, size, false);

    // i2c_write_timeout_us(i2c_default, MAX_ADDRES, &reg,    1, true, 1000);
    // i2c_read_timeout_us (i2c_default, MAX_ADDRES, &rxdata, 1, false, 1000);
    return;
}

uint8_t get_reg(uint8_t reg)
{
    uint8_t rxdata;
    i2c_write_blocking(i2c_default, MAX_ADDRES, &reg, 1, true);
    i2c_read_blocking(i2c_default, MAX_ADDRES, &rxdata, 1, false);

    return rxdata; 
}

//----------------------------------------------------------------------//

uint8_t set_reg(uint8_t reg, uint8_t data)
{
  uint8_t txdata[3];
  txdata[0] =   MAX_ADDRES;
  txdata[1] = reg;
  txdata[2] = data;
  i2c_write_raw_blocking(i2c_default, &txdata, 3);

  // uint8_t txdata[2];
  // txdata[0] = reg;
  // txdata[1] = data;
  // i2c_write_blocking(i2c_default, MAX_ADDRES, &txdata, 2, false);
}

uint8_t write_reg (uint8_t reg, uint8_t *data, int size)
{

    i2c_write_blocking(i2c_default, MAX_ADDRES, &reg, 1, true);
    i2c_write_raw_blocking(i2c_default, data, size);

    return;
    // i2c_write_timeout_us(i2c_default, MAX_ADDRES, &reg,    1, true, 1000);
    // i2c_read_timeout_us (i2c_default, MAX_ADDRES, &rxdata, 1, false, 1000);

}
//----------------------------------------------------------------------//

void sensorConfiguration(uint8_t ledBrightness, uint8_t sampleAverage, uint8_t ledMode, uint8_t sampleRate, uint8_t pulseWidth, uint8_t adcRange)
{

  setFIFOAverage(sampleAverage);


  setADCRange(adcRange);
  setSampleRate(sampleRate);
  setPulseWidth(pulseWidth);
  setPulseAmplitudeRed(ledBrightness);
  setPulseAmplitudeIR(ledBrightness);

  enableSlot(1, SLOT_RED_LED);
  if (ledMode > MODE_REDONLY) enableSlot(2, SLOT_IR_LED);


  setLEDMode(ledMode);

  if (ledMode == MODE_REDONLY) {
    _activeLEDs = 1;
  } else {
    _activeLEDs = 2;
  }

  enableFIFORollover(); 
  resetFIFO(); 
}

//----------------------------------------------------------------------//

void setLEDMode(uint8_t ledMode)
{
  mode_conf_t modeReg;
  read_reg(MODE_conf, &modeReg, 1);
  modeReg.LedMode = ledMode;
  write_reg(MODE_conf, &modeReg, 1);
}

//----------------------------------------------------------------------//

void setFIFOAverage(uint8_t numberOfSamples)
{
  fifo_conf_t FIFOReg;
  read_reg(FIFO_conf, &FIFOReg, 1);
  FIFOReg.SMP_AVR = numberOfSamples;
  write_reg(FIFO_conf, &FIFOReg, 1);
}
//----------------------------------------------------------------------//

void enableFIFORollover(void)
{
  fifo_conf_t FIFOReg;
  read_reg(FIFO_conf, &FIFOReg, 1);
  FIFOReg.FIFO_ROL_LOVER_EN = 1;
  write_reg(FIFO_conf, &FIFOReg, 1);
}

//----------------------------------------------------------------------//

void disableFIFORollover(void)
{
  fifo_conf_t FIFOReg;
  read_reg(FIFO_conf, &FIFOReg, 1);
  FIFOReg.FIFO_ROL_LOVER_EN = 0;
  write_reg(FIFO_conf, &FIFOReg, 1);
}

//----------------------------------------------------------------------//

void setADCRange(uint8_t adcRange)
{
  sParticle_t particleReg;
  read_reg(SpO2_conf, &particleReg, 1);
  particleReg.adcRange = adcRange;
  write_reg(SpO2_conf, &particleReg, 1);
}

//----------------------------------------------------------------------//

void setSampleRate(uint8_t sampleRate)
{
  sParticle_t particleReg;
  read_reg(SpO2_conf, &particleReg, 1);
  particleReg.sampleRate = sampleRate;
  write_reg(SpO2_conf, &particleReg, 1);
}

//----------------------------------------------------------------------//

void setPulseWidth(uint8_t pulseWidth)
{
  sParticle_t particleReg;
  read_reg(SpO2_conf, &particleReg, 1);
  particleReg.pulseWidth = pulseWidth;
  write_reg(SpO2_conf, &particleReg, 1);
}

//----------------------------------------------------------------------//

void setPulseAmplitudeRed(uint8_t amplitude)
{
  uint8_t byteTemp = amplitude;
  write_reg(LED_pls_Amp1, &byteTemp, 1);
}

//----------------------------------------------------------------------//

void setPulseAmplitudeIR(uint8_t amplitude)
{
  uint8_t byteTemp = amplitude;
  write_reg(LED_pls_Amp2, &byteTemp, 1);
}

//----------------------------------------------------------------------//


void enableSlot(uint8_t slotNumber, uint8_t device)
{
  sMultiLED_t multiLEDReg;
  switch (slotNumber) {
  case (1):
    read_reg(MULTI_LED_Mode_REGcp1, &multiLEDReg, 1);
    multiLEDReg.SLOT1 = device;
    write_reg(MULTI_LED_Mode_REGcp1, &multiLEDReg, 1);
    break;
  case (2):
    read_reg(MULTI_LED_Mode_REGcp1, &multiLEDReg, 1);
    multiLEDReg.SLOT2 = device;
    write_reg(MULTI_LED_Mode_REGcp1, &multiLEDReg, 1);
    break;
  default:
    break;
  }
}

//----------------------------------------------------------------------//

void disableAllSlots(void)
{
  sMultiLED_t multiLEDReg;
  multiLEDReg.SLOT1 = 0;
  multiLEDReg.SLOT2 = 0;
  write_reg(MULTI_LED_Mode_REGcp1, &multiLEDReg, 1);
}

//----------------------------------------------------------------------//

void resetFIFO(void)
{
  uint8_t byteTemp = 0;
  write_reg(FIFO_w_ptr, &byteTemp, 1);
  write_reg(OVERFLOW_cnt, &byteTemp, 1);
  write_reg(FIFO_r_ptr, &byteTemp, 1);
}

//----------------------------------------------------------------------//

uint8_t getWritePointer(void)
{
  uint8_t byteTemp;
  read_reg(FIFO_w_ptr, &byteTemp, 1);
  return byteTemp;
}

//----------------------------------------------------------------------//

uint8_t getReadPointer(void)
{
  uint8_t byteTemp;
  read_reg(FIFO_r_ptr, &byteTemp, 1);
  return byteTemp;
}

//----------------------------------------------------------------------//

void softReset(void)
{
  mode_conf_t modeReg;
  read_reg(MODE_conf, &modeReg, 1);
  modeReg.RESET = 1;
  write_reg(MODE_conf, &modeReg, 1);
  uint32_t startTime = time_us_32();
  while (time_us_32() - startTime < 100 * 1000) {
    read_reg(MODE_conf, &modeReg, 1);
    if (modeReg.RESET == 0) break; 
    sleep_ms(1);
  }
}

//----------------------------------------------------------------------//



uint32_t getRed(void)
{
  getNewData();
  return (senseBuf.red[senseBuf.head]);
}

uint32_t getIR(void)
{
  getNewData();
  return (senseBuf.IR[senseBuf.head]);
}

void getNewData(void)
{
  int32_t numberOfSamples = 0;
  uint8_t readPointer = 0;
  uint8_t writePointer = 0;
  while (1) {
    readPointer = getReadPointer();
    writePointer = getWritePointer();

    if (readPointer == writePointer) {
    //   DBG("no data");
    ;
    } else {
      numberOfSamples = writePointer - readPointer;
      if (numberOfSamples < 0) numberOfSamples += 32;
      int32_t bytesNeedToRead = numberOfSamples * _activeLEDs * 3;
   
        while (bytesNeedToRead > 0) {
          senseBuf.head++;
          senseBuf.head %= MAX30102_SENSE_BUF_SIZE;
          uint32_t tempBuf = 0;
          if (_activeLEDs > 1) { 
            uint8_t temp[6];
            uint8_t tempex;

            read_reg(FIFO_DATA_Reg, temp, 6);

            for(uint8_t i = 0; i < 3; i++){
              tempex = temp[i];
              temp[i] = temp[5-i];
              temp[5-i] = tempex;
            }

            memcpy(&tempBuf, temp, 3*sizeof(temp[0]));
            tempBuf &= 0x3FFFF;
            senseBuf.IR[senseBuf.head] = tempBuf;
            memcpy(&tempBuf, temp+3, 3*sizeof(temp[0]));
            tempBuf &= 0x3FFFF;
            senseBuf.red[senseBuf.head] = tempBuf;
          } else { 
            uint8_t temp[3];
            uint8_t tempex;


            read_reg(FIFO_DATA_Reg, temp, 3);
            tempex = temp[0];
            temp[0] = temp[2];
            temp[2] = tempex;

            memcpy(&tempBuf, temp, 3*sizeof(temp[0]));
            tempBuf &= 0x3FFFF;
            senseBuf.red[senseBuf.head] = tempBuf;
          }
          bytesNeedToRead -= _activeLEDs * 3;
        }
      return;
    }
    sleep_ms(1);
  }
}

//**********************************************************************//
//**********************************************************************//
