/**
 * MikroElektronika UNI-DS 3 development system
 * MCU: AVR ATmega128
 * Osc: 10 MHz
 *
 * Author: Tom Van Looy <tom$ctors,net>
 * License: http://ctors.net/isc_license.txt
 *
 * This program simulates a knight rider using PWM on 8 LED's
 */
 
// Best values are 50, 100, 200
#define SPEED 100
 
void main(void)
{
    // Counter
    unsigned char i = 0;
    
    // PWM
    unsigned char pwm1 = 0xFF;
    unsigned char pwm2 = 0xFF;
    
    // LEDs
    unsigned char led1 = 0b00000001;
    unsigned char led2 = 0b00000010;
    unsigned char led3 = 0b00000100;
    
    // Set the LED pins to output
    DDRA = 0xFF;

    while(1) {
        
        // LED number 1
        if (i > pwm1) {
            PORTA |= led1; // turn on
        }
        else {
            PORTA &= ~led1; // turn off
        }
        
        // LED number 2
        PORTA |= led2; // turn on

        // LED number 3
        if (i > pwm2) {
            PORTA |= led3; // turn on
        }
        else {
            PORTA &= ~led3; // turn off
        }

        // Move leds, and slow down process
        if (i % SPEED == 0) {
            pwm1++;
            pwm2--;
            if (pwm1 == 255) {
                led1 <<= 1;
                led2 <<= 1;
                led3 <<= 1;
            }
            if (led1 == 0) {
               led1 = 1;
            }
            if (led2 == 0) {
               led2 = 1;
            }
            if (led3 == 0) {
               led3 = 1;
            }
        }

        i++; // turns over automatically, 255 + 1 = 0
    }
}

