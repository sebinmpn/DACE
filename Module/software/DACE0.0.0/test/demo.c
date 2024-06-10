/*

arm with 2 liver

|0---------0-
            \
             \
              \
               0-----o

each arm have one motor set angle in axial and to motors to set 2 angles;


*/ 

#define GPIO_address 0
#define GPIO_output_dir_address 0

#define pwm_gpio_pin 0
#define num_of_tick_per_ms 0

#define arm1_retation_pin  0 // define gpio pin number
#define arm1_axis1_pin     0 // define gpio pin number
#define arm1_axis2_pin     0 // define gpio pin number
#define arm2_retation_pin  0 // define gpio pin number
#define arm2_axis1_pin     0 // define gpio pin number
#define arm2_axis2_pin     0 // define gpio pin number


void sleep_ms (int cnt);
void gpio_high (int pin_num);
void gpio_dir(int pin_num, bool _dir);
void gpio_low (int pin_num);
void pwm( int pin, int width);
void speed (int pin, int speed_value, int min, int max);

void arm1 (int rotation, int axis1, int axis2);
void arm1 (int rotation, int axis1, int axis2);
void arm_init();

void main()
{
    arm_init();
    
    while (true)
    {
        for(int i = 0; i < 10; i++)
        {
            arm1(10, i, i);
            for(int j = 0; j < 10; j++) 
                arm2(10, j, j+i);  
        }
    }
    
}

void arm_init()
{
    gpio_dir(arm1_retation_pin, 1);
    gpio_dir(arm2_retation_pin, 1);
    gpio_dir(arm1_axis1_pin, 1);
    gpio_dir(arm1_axis2_pin, 1);
    gpio_dir(arm2_axis1_pin, 1);
    gpio_dir(arm2_axis2_pin, 1);
}

void arm1 (int rotation, int axis1, int axis2)
{
    pwm(arm1_retation_pin, rotation);
    pwm(arm1_axis1_pin, axis1);
    pwm(arm1_axis2_pin, axis2);
}


void arm2 (int rotation, int axis1, int axis2)
{
    pwm(arm1_retation_pin, rotation);
    pwm(arm1_axis1_pin, axis1);
    pwm(arm1_axis2_pin, axis2);
}




void sleep_ms (int cnt)
{
    for (int i = 0; i < cnt * num_of_tick_per_ms / 3 - 4; i++)
    {
        NOP;
    }
}

void gpio_dir(int pin_num, bool _dir)
{
    uint32_t mask = (1 << pin_num);
    if(_dir) GPIO_output_dir_address = (GPIO_address |  mask); 
    else     GPIO_output_dir_address = (GPIO_address & !mask); 
}

void gpio_high (int pin_num)
{
    uint32_t mask = (1 << pin_num);

    GPIO_address = GPIO_address | mask; 

}

void gpio_low (int pin_num)
{
    uint32_t mask = (1 << pin_num);

    GPIO_address = (GPIO_address & !mask); 
}

 void pwm( int pin, int width) // width from 0 to 255
 {
    gpio_high(pin);
    sleep(width);
    gpio_low(pin);
    sleep(255 - width);
 }


void speed (int pin, int speed_value, int min, int max)
{
    int step = max - min / speed_value;

    for (int _width = min; _width < max; _width++)
    {
        pwm (pin, _width);
    }
    
}