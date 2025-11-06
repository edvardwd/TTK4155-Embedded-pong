#include "drivers/uart.h"
#include "drivers/xmem.h"
#include "drivers/adc.h"
#include "drivers/joystick.h"
#include "drivers/oled.h"
#include "drivers/spi.h"
#include "drivers/oled_menu.h"
#include "drivers/mcp2515.h"
#include "drivers/can.h"
#include "avr/delay.h"
/////////////////////////////////////////////


int main() {
    //printf("main node1");
    uart_init(UBRR);
    xmem_init();
    
    adc_init();
    volatile pos_t joystick;
    volatile pos_t slider;

    spi_master_init();
    oled_init_minimal();
    _delay_ms(200);
    can_init();


    while(1){
        joystick_send_pos_can();
        _delay_ms(1);
        touch_send_pos_can();
        _delay_ms(1);

        // can_send_message(&msg, 0);
    }

    // menu_t* menu = init_menu();

    // for (uint8_t i = 0; i < 8; i++)
    // {
    //     menu_t* child = (menu->sub_menus[i]);

    //     oled_print(i*8, 0, child->title);
    // }

    // while (1){
    //     navigate_menu(menu);

    //     _delay_ms(10);

    // }
    return 0;
}
