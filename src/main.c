#include "drivers/uart.h"
#include "drivers/xmem.h"
#include "drivers/adc.h"
#include "drivers/joystick.h"
#include "drivers/oled.h"
#include "drivers/spi.h"
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
    // volatile uint8_t x = ADC[0];
    // volatile uint8_t y = ADC[0];
    // volatile uint8_t slider1 = ADC[0];
    // volatile uint8_t slider2 = ADC[0];


    // printf("X-pos: %4d, Y-pos: %4d\n", x, y);
    for (uint8_t i; i < 4; i++){
        volatile uint8_t val = ADC[i];
        printf("Channel: %02X\tValue: %4d\n", i, val);
    }
}
//
int main() {
    uart_init(UBRR);
    xmem_init();
    // SRAM_test();
    // test_write();
    // test_receive_transmit();
    // test_stdio();
    adc_init();
    volatile pos_t joystick;
    volatile pos_t slider;

    spi_master_init();
    oled_init();
    oled_write_cmd(0xa5); // Entire display ON

    //char* STRING = "abcdefghijklmnopqrstuvwxyz";


    // oled_write_cmd(0x05);

    // Trigger LEDs
    spi_master_select_slave(IO_SS_PIN);
    spi_master_transmit_byte(0x05);
    _delay_us(40);
    spi_master_transmit_byte(2);
    _delay_us(1);
    spi_master_transmit_byte(2);
    PORTB |= (1 << IO_SS_PIN); // Deselect

    


    // for (uint8_t i = 0; i < sizeof(BYTES); i++){
    //     spi_master_select_slave(IO_SS_PIN);
    //     spi_master_transmit_byte(BYTES[i]);
    //     PORTB |= (1 << IO_SS_PIN); // Deselect
    // }

    while (1){
        // oled_print_char('A');
        // spi_master_transmit_bytes(STRING, 26);
        // _delay_ms(2000);
        // update_pos(&joystick, &slider);  
        // printf("X: %d Y: %d \nX: %d Y: %d\n", joystick.x, joystick.y, slider.x, slider.y);
        // printf("################################\n");
    }
    return 0;
}