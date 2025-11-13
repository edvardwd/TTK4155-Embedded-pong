#include "drivers/uart.h"
#include "drivers/xmem.h"
#include "drivers/adc.h"
#include "drivers/joystick.h"
#include "drivers/oled.h"
#include "drivers/spi.h"
#include "drivers/oled_menu.h"
#include "drivers/mcp2515.h"
#include "drivers/can.h"
#include "game.h"
#include "util/delay.h"
/////////////////////////////////////////////



int main() {
    uart_init(UBRR);
    printf("Node 1 starting..\r\n");
    
    xmem_init();
    
    adc_init();

    spi_master_init();
    oled_init_minimal();
    _delay_ms(200);
    can_init();


    // while(1){
    //     can_send_x_pos();
    //     _delay_ms(1);

    //     uint8_t btn_pressed = joystick_get_button_pressed();
    //     if (btn_pressed){
    //         // printf("Button pressed\r\n");
    //         can_send_button_pressed();
    //     }
    //     // can_send_message(&msg, 0);
    // }

    menu_t* menu = init_menu();
    oled_print_menu(menu);

    can_send_button_pressed();

    while (1){
        navigate_menu(menu);

        _delay_ms(10);

    }
    return 0;
}
