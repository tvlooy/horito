/**
 * MikroElektronika UNI-DS 3 development system
 * MCU: AVR ATmega128
 * Osc: 10 MHz
 *
 * Author: Tom Van Looy <tom$ctors,net>
 * License: http://ctors.net/isc_license.txt
 *
 * This program plays a simple animation on the LED's on the board
 */

#define F_CPU 10000000UL // 10 MHz

#include <avr/io.h>
#include <util/delay.h>

int main()
{
    unsigned int i;
    DDRA = DDRB = DDRC = DDRD = DDRE = DDRF = 0xFF;

    while (1) {
        PORTA = PORTB = PORTC = PORTD = PORTE = PORTF = 0x00;

        for (i = 1; i <= (0xFF + 1) * 8; i *= 2) {
            _delay_ms(200);
            PORTA = PORTF = i / 4 + (i / 8);
            PORTB = PORTE = i / 2 + (i / 4);
            PORTC = PORTD = i + (i / 2);
        }
    }
}

