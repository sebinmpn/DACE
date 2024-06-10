/*
  -vcc
  |
  R
  |---voiut pin_leight.
  ldr
  |
  gng

mpu 6055  accelerometer with 3 pin digital axis and one interrupt pin

x--
y--
z--
inr--

audio record with condenser mice
--


each arm have one motor set angle in axial and to motors to set 2 angles;


*/ 

#define GPIO_address 0
#define GPIO_output_dir_address 0

#define pwm_gpio_pin 0
#define num_of_tick_per_ms 0


void sleep_ms (int cnt);
void gpio_high (int pin_num);
void gpio_dir(int pin_num, bool _dir);
void gpio_low (int pin_num);
void pwm( int pin, int width);

// void audio_init();
int read_audio();

int read_led();
void read_led_init();
void axis_init();
int get_axis();
void temp_init();
int get_temp();

//----------------------------------------------------------------------//

void main()
{

    HAL_Init();
    SystemClock_Config();
    MX_ADC1_Init();
    read_led_init();
    axis_init();

    while (1)
    {
        int _audio = read_audio();
        int _led = read_led();
        int _axis = get_axis();

    }
    
    
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
void temp_init();
int get_temp();

int get_temp()
{
        uint32_t temp_pin = 0;
        HAL_ADC_Start(&temp_pin);
        HAL_ADC_PollForConversion(&temp_pin,1000);
        int readValue = HAL_ADC_GetValue(&temp_pin);
        HAL_ADC_Stop(&temp_pin);
        return readValue;
}

//----------------------------------------------------------------------//

void axis_init()
{
    gpio_dir(GPIO_PIN_9, 1); //  sensor pins
    gpio_dir(GPIO_PIN_9, 1);
    gpio_dir(GPIO_PIN_9, 1);
    return;
}

int get_axis()
{
    bool _x = HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_9);
    bool _y = HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_9);
    bool _z = HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_9);
    if(_x&!_y&!_z) return 1;
    if(!_x&!y&!_z) return 2;
    if(_x&_y&!_z) return 3;
    if(_x&!_y&!_z) return 4;
    if(_x&_y&_z) return 5;
    if(_x&_y&!_z) return 6;

}

//----------------------------------------------------------------------//
void read_led_init()
{
    gpio_dir(GPIO_PIN_9, 1);
    return;
}

int read_led()
{
    return HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_9);

}
//----------------------------------------------------------------------//

int read_audio()
{
    HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1,1000);
        int readValue = HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Stop(&hadc1);
        return readValue;
}

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
void sleep_ms (int cnt)
{
    for (int i = 0; i < cnt * num_of_tick_per_ms / 3 - 4; i++)
    {
        NOP;
    }
}

//----------------------------------------------------------------------//

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

//----------------------------------------------------------------------//

 void pwm( int pin, int width) // width from 0 to 255
 {
    gpio_high(pin);
    sleep(width);
    gpio_low(pin);
    sleep(255 - width);
 }

//----------------------------------------------------------------------//
//----------------------------------------------------------------------//