#include "pico/stdlib.h"

#include "ili9341.h"
#include "gfx.h"

#include "max30102.h"

int c;

int main()
{
    //   stdio_init_all();
    max_init();
    LCD_initDisplay();
    LCD_setRotation(1);
    GFX_createFramebuf();
    while (true)
    {
        GFX_clearScreen();
        GFX_setCursor(0, 0);
        GFX_printf("Hello GFX!\n%d", c++);
        GFX_drawFastHLine(20, 10, c, 0xF000);
        
        uint8_t part_id = get_reg(PART_ID);
        GFX_printf("\npart id: %X\n", part_id);

        uint8_t readPointer = getReadPointer();
        uint8_t writePointer = getWritePointer();
        GFX_printf("rp %X, wp %X\n", readPointer, writePointer);

        GFX_printf(" 0x00 %X\n", get_reg (0x00));
        GFX_printf(" 0x01 %X\n", get_reg (0x01));
        GFX_printf(" 0x02 %X\n", get_reg (0x02));
        GFX_printf(" 0x03 %X\n", get_reg (0x03));
        GFX_printf(" 0x04 %X\n", get_reg (0x04));
        GFX_printf(" 0x05 %X\n", get_reg (0x05));
        GFX_printf(" 0x06 %X\n", get_reg (0x06));
        GFX_printf(" 0x07 %X\n", get_reg (0x07));
        GFX_printf(" 0x08 %X\n", get_reg (0x08));
        GFX_printf(" 0x09 %X\n", get_reg (0x09));
        GFX_printf(" 0x0A %X\n", get_reg (0x0A));
        
        GFX_printf(" 0x0C %X\n", get_reg (0x0C));
        GFX_printf(" 0x0D %X\n", get_reg (0x0D));

        GFX_printf(" 0x11 %X\n", get_reg (0x11));
        GFX_printf(" 0x12 %X\n", get_reg (0x12));


        GFX_printf(" 0xFE %X\n", get_reg (0xFE));
        GFX_printf(" 0xFF %X\n", get_reg (0xFF));

        uint8_t txdata = 0x1F; 
        set_reg(LED_pls_Amp1, &txdata);
        sleep_ms(10);
        GFX_printf("LED_pls_Amp1  %X : %X\n",txdata, get_reg(LED_pls_Amp1));

        GFX_flush();
        sleep_ms(500);
    }

        //     // fifo r ptr
        // uint8_t fifo_r_ptr = get_reg(FIFO_w_ptr);
        // uint8_t fifo_w_ptr = get_reg(FIFO_r_ptr);
        // uint8_t part_id = get_reg(PART_ID);

        // printf("PART_ID %x \n" ,part_id);
        // printf("fifo_r_ptr %x, fifo_w_ptr %x\n",fifo_r_ptr, fifo_w_ptr);

}