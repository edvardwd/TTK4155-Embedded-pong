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


void init_all(){
    uart_init(UBRR);
    printf("Node 1 starting..\r\n");
    
    xmem_init();
    
    adc_init();

    spi_master_init();
    oled_init_minimal();
    _delay_ms(200);
    can_init();
}


int main() {
    init_all();

    menu_t* menu = oled_menu_init();
    oled_menu_print(menu);
    while (1){
        oled_menu_navigate(menu);
 
    }
    return 0;
}
