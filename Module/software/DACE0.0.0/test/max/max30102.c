#include "max30102.h"


//**********************************************************************//
//**********************************************************************//

void max_init()
{
      i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));


    // sensor_config();
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

uint8_t get_reg (uint8_t reg)
{
    uint8_t rxdata;

    i2c_write_blocking(i2c_default, MAX_ADDRES, &reg, 1, true);
    i2c_read_blocking(i2c_default, MAX_ADDRES, &rxdata, 1, false);
    sleep_ms(1);

    return rxdata;
}

//----------------------------------------------------------------------//

void set_reg (uint8_t reg, uint8_t txdata)
{
    i2c_write_blocking(i2c_default, MAX_ADDRES, &reg, 1, true);
    i2c_write_blocking(i2c_default, MAX_ADDRES, &txdata, 1, false);
    sleep_ms(1);
}
//----------------------------------------------------------------------//

uint8_t getWritePointer() { return get_reg(FIFO_w_ptr); }

//----------------------------------------------------------------------//

uint8_t getReadPointer() { return get_reg(FIFO_r_ptr); }

//----------------------------------------------------------------------//
// particleSensor.sensorConfiguration(/*ledBrightness=*/0x1F, /*sampleAverage=*/SAMPLEAVG_4, \
                                  /*ledMode=*/MODE_MULTILED, /*sampleRate=*/SAMPLERATE_400, \
                                  /*pulseWidth=*/PULSEWIDTH_411, /*adcRange=*/ADCRANGE_4096);
void sensor_config()
{
    fifo_conf_t fifo_conf;
    mode_conf_t mode_conf;
    spo2_conf_t spo2_conf;

    fifo_conf.FIFO_A_FULL = 0;
    fifo_conf.FIFO_ROL_LOVER_EN = 0;
    fifo_conf.SMP_AVR = SAMPLEAVG_4;

    mode_conf.MODE = MODE_MULTILED;
    mode_conf.RESET = 0;
    mode_conf.SHDN = 0;

    spo2_conf.LED_PW = PULSEWIDTH_411;
    spo2_conf.SPO2_ADC_REG = ADCRANGE_4096;
    spo2_conf.SPO2_SR = SAMPLERATE_400;

    uint8_t led1_amp = 0x1F; //led amp 1 and 2 , 0xc,0x0d
    uint8_t led2_amp = 0x1F;

    //  FIFO_conf    _u(0x08)
    //  MODE_conf    _u(0x09)
    //  SpO2_conf    _u(0x0A)
    //  LED_pls_Amp1 _u(0x0C)
    //  LED_pls_Amp2 _u(0x0D)
    uint8_t reg = FIFO_conf;
    i2c_write_blocking(i2c_default, MAX_ADDRES, &reg, 1, true);
    i2c_write_blocking(i2c_default, MAX_ADDRES, &fifo_conf, 1, true);
    i2c_write_blocking(i2c_default, MAX_ADDRES, &mode_conf, 1, true);
    i2c_write_blocking(i2c_default, MAX_ADDRES, &spo2_conf, 1, false);

    reg = LED_pls_Amp1;
    i2c_write_blocking(i2c_default, MAX_ADDRES, &reg, 1, true);
    i2c_write_blocking(i2c_default, MAX_ADDRES, &led1_amp, 1, true); 
    i2c_write_blocking(i2c_default, MAX_ADDRES, &led2_amp, 1, false);    
}

//**********************************************************************//
//**********************************************************************//