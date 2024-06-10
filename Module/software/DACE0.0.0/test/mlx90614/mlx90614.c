#include "mlx90614.h"


//**********************************************************************//
//**********************************************************************//

uint8_t get_reg_mlx (uint8_t reg)
{
    uint8_t rxdata;

    i2c_write_timeout_us(i2c_default, MLX90614_DEFAULT_ADDRESS, &reg,    1, true, 1000);
    i2c_read_timeout_us (i2c_default, MLX90614_DEFAULT_ADDRESS, &rxdata, 1, false, 1000);
    // i2c_write_blocking(i2c_default, MLX90614_DEFAULT_ADDRESS, &reg,    1, true);
    // i2c_read_blocking (i2c_default, MLX90614_DEFAULT_ADDRESS, &rxdata, 1, false);

    return rxdata;
}

uint8_t write_mlx (uint8_t reg)
{
    return i2c_write_timeout_us(i2c_default, MLX90614_DEFAULT_ADDRESS, &reg,    1, false, 1000);
    // i2c_write_blocking(i2c_default, MLX90614_DEFAULT_ADDRESS, &reg,    1, true);
}

uint8_t read_mlx ()
{
    uint8_t rxdata = 0xff;
    i2c_read_timeout_us (i2c_default, MLX90614_DEFAULT_ADDRESS, &rxdata, 1, false, 1000);
    // i2c_read_blocking (i2c_default, MLX90614_DEFAULT_ADDRESS, &rxdata, 1, false);
    return rxdata;
}
//----------------------------------------------------------------------//


void mlx_90614_init()
{
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(MLX_SDA_pin, GPIO_FUNC_I2C);
    gpio_set_function(MLX_SCL_pin, GPIO_FUNC_I2C);
    gpio_pull_up(MLX_SDA_pin);
    gpio_pull_up(MLX_SCL_pin);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(MLX_SDA_pin, MLX_SCL_pin, GPIO_FUNC_I2C));

}

//**********************************************************************//
//**********************************************************************//  

uint8_t I2CReadWord(uint8_t reg, int16_t * dest)
{
    
	// write_mlx(reg);

	// uint8_t lsb = read_mlx();
	// uint8_t msb = read_mlx();
	// uint8_t pec = read_mlx();

    #define lsb 0
    #define msb 1
    #define pec 2

    uint8_t data[3];

    i2c_write_timeout_us(i2c_default, MLX90614_DEFAULT_ADDRESS, &reg,    1, false, 1000);
    i2c_read_timeout_us (i2c_default, MLX90614_DEFAULT_ADDRESS, &data, 3, false, 1000);
    

    dest = (data[msb] << 8) | data[lsb];

    // i2c_write_blocking(i2c_default, MLX90614_DEFAULT_ADDRESS, &reg,    1, true);
    // i2c_read_blocking (i2c_default, MLX90614_DEFAULT_ADDRESS, &rxdata, 1, false);

	

	uint8_t crc = crc8(0, (MLX90614_DEFAULT_ADDRESS << 1));
	crc = crc8(crc, reg);
	crc = crc8(crc, (MLX90614_DEFAULT_ADDRESS << 1) + 1);
	crc = crc8(crc, lsb);
	crc = crc8(crc, msb);
	
	if (crc == pec)
	{
		*dest = (msb << 8) | lsb;
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t writeEEPROM(uint8_t reg, int16_t data)
{	
	// Clear out EEPROM first:
	if (I2CWriteWord(reg, 0) != 0)
		return 0; // If the write failed, return 0
	sleep_ms(5); // Delay tErase
	
	uint8_t i2cRet = I2CWriteWord(reg, data);
	sleep_ms(5); // Delay tWrite
	
	if (i2cRet == 0)
		return 1;
	else
		return 0;	
}

uint8_t I2CWriteWord(uint8_t reg, int16_t data)
{
    uint8_t _data[4];
    _data[0] = reg;

	uint8_t crc;
	_data[1] = data & 0x00FF; //lsb
	_data[2] = (data >> 8); // msb
	
	crc = crc8(0, (MLX90614_DEFAULT_ADDRESS << 1));
	crc = crc8(crc, reg);
	crc = crc8(crc, lsb);
	crc = crc8(crc, msb);

    _data[3] = crc; // crc

     return i2c_write_timeout_us (i2c_default, MLX90614_DEFAULT_ADDRESS, &_data, 4, false, 1000);    
	

}

uint8_t crc8 (uint8_t inCrc, uint8_t inData)
{
	uint8_t i;
	uint8_t data;
	data = inCrc ^ inData;
	for ( i = 0; i < 8; i++ )
	{
		if (( data & 0x80 ) != 0 )
		{
			data <<= 1;
			data ^= 0x07;
		}
		else
		{
			data <<= 1;
		}
	}
	return data;
}


uint8_t readObject2()
{
	int16_t rawObj;
	// Read from the TOBJ2 register, store into the rawObj variable
	if (I2CReadWord(MLX90614_REGISTER_TOBJ2, &rawObj))
	{
		// If the read succeeded
		if (rawObj & 0x8000) // If there was a flag error
		{
			return 0; // Return fail
		}
		// Store the object2 temperature into the class variable
		_rawObject2 = rawObj;
		return 1;
	}
	return 0;	
}