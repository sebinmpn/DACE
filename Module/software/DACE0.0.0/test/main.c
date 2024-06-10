#include "pico/stdlib.h"
#include "hardware/irq.h"

#include "ili9341.h"
#include "gfx.h"

#include "max30102.h"
#include "mlx90614.h"
 

int c;

int main()
{
    stdio_init_all();
    max_init();
    mlx_90614_init();
    LCD_initDisplay();
    LCD_setRotation(1);
    GFX_createFramebuf();

    GFX_clearScreen();
    GFX_setCursor(0, 0);
    GFX_printf("Started.......");
    GFX_flush();
   
        max_reset();

    while (true)
    {
        GFX_clearScreen();
        GFX_setCursor(0, 0);
        GFX_printf("Hello GFX!\n%d", c++);
        GFX_drawFastHLine(20, 10, c, 0xF000);
       
        GFX_printf("\n PART_ID : %x \n", get_reg(0xff)); 
        GFX_printf("\n get_temp : %d \n", get_temp()); 
       
        GFX_flush();
        sleep_ms(50);
    }

}