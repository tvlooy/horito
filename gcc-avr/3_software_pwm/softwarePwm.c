/**
 * MikroElektronika UNI-DS 3 development system
 * MCU: AVR ATmega128
 * Osc: 10 MHz
 *
 * Author: Tom Van Looy <tom$ctors,net>
 * License: http://ctors.net/isc_license.txt
 *
 * This program just dims a few LED's, deliberately not using timers but
 * by generating PWM by itself.
 */

#define F_CPU 10000000UL // 10 MHz

#include <avr/io.h>
#include <util/delay.h>

void main() 
{
    unsigned char i = 0;      // counter
    unsigned char pwm1 = 200; // short duty cycle
    unsigned char pwm2 = 10;  // big duty cycle
    
    /**
     * Set the LED pins to output
     */
    DDRA = 0b00000011;
     
    while(1) {
        // LED number 1
        if (i > pwm1) {
            PORTA |= 0b00000001; // turn on
        }
        else {
            PORTA &= 0b11111110; // turn off
        }

        // LED number 2
        if (i > pwm2) {
            PORTA |= 0b00000010; // turn on
        }
        else {
            PORTA &= 0b11111101; // turn off
        }

        i++; // turns over automatically, 255 + 1 = 0
    }
}

