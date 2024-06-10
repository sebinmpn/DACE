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
