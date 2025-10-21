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

void test_receive_transmit() {
    while (1) {
        // Receive from computer and send back
        unsigned char data_from_computer = uart_receive();
        uart_transmit(data_from_computer);
    }
}

void test_write(){
    uint8_t val = rand();
    printf("Writing %d to PA1...\n", val);
    xmem_write(val, PA1);
    printf("Write function completed\n");
    uint8_t storedVal = xmem_read(PA1);
    printf("Stored value: %d\n\n", storedVal);
}

void test_stdio() {
    char input[50];
    printf("Write something:\n");
    scanf("%49s", input);
    printf("You wrote: %s\n", input);
}

void test_latch() {
    uint8_t val = 0;
    while (1) {
        printf("Setting PA0 to %d\n", val);
        // Count 0 -> 7
        xmem_write(val, PA0);
        _delay_ms(3000);
        
        val++;
        val %= 8;
    }
}

void test_adc() {
    // printf("X-pos: %4d, Y-pos: %4d\n", x, y);
    for (uint8_t i = 0; i < 4; i++){
        volatile uint8_t val = ADC[i];
        printf("Channel: %02X\tValue: %4d\n", i, val);
    }
}


// void test_can(){
//     can_message test_message;
//     test_message.id = 1;
//     test_message.data_length = 3;
//     test_message.data = {'h', 'e', 'i'}
// }


int main() {
    uart_init(UBRR);
    xmem_init();
    while (1){
        printf("Hei\n");
        _delay_ms(50);
    }
    // adc_init();
    // volatile pos_t joystick;
    // volatile pos_t slider;

    // spi_master_init();
    // oled_init_minimal();
    // _delay_ms(200);

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
