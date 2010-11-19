/**
 * MikroElektronika UNI-DS 3 development system
 * MCU: AVR ATmega128
 * Osc: 10 MHz
 *
 * Author: Tom Van Looy <tom$ctors,net>
 * License: http://ctors.net/isc_license.txt
 *
 * This program fades a LED in and out using a timer to generate PWM
 */

void main(void)
{
    /**
     * Timer/counter control register. The bits stand for:
     * FOC0 WGM00 COM01 COM00 WGM01 CS02 CS01 CS00
     */
    TCCR0 = 0b01101001;
    
    /**
     * Output compare register is used as counter
     */
    OCR0 = 0;
    
    /**
     * OC0 is on pin PB4, which has to be set to output
     */
    DDRB = 0b00010000;
    
    while(1) {
        while(OCR0 < 255) {
            Delay_ms(5);
            OCR0++;
        }
        while(OCR0 > 0) {
            Delay_ms(10);
            OCR0--;
        }
    }
}

