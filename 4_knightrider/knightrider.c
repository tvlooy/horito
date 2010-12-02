/**
 * MikroElektronika UNI-DS 3 development system
 * MCU: AVR ATmega128
 * Osc: 10 MHz
 *
 * Author: Tom Van Looy <tom$ctors,net>
 * License: http://ctors.net/isc_license.txt
 *
 * This program simulates a knight rider using PWM on 8 LED's.
 * Use the TCCR0 prescaler and OCR0 to modify animation speed.
 */

enum BOOLEAN { TRUE, FALSE } _doMove = FALSE;

void setup(void);

/**
 * Hardware setup
 */
void setup(void)
{
    DDRA = 0xFF;         // Set the LED array pins to output
    PORTA = 0b00000010;  // LED 2 must initially be illuminated

    SREG_I_bit = 1;      // Global interrupts enable

    TCCR0 = 0b00101101;  // Timer/counter register
    OCR0 = 0x250;        // Output compare
    TIMSK |= 0b00000010; // Interrupt on compare match
}

/**
 * Timer 0 output compare match interrupt
 */
void Timer0CompareMatch() iv IVT_ADDR_TIMER0_COMP
{
    _doMove = TRUE;
}

/**
 * Program entry point
 */
void main(void)
{
    // PWM counter
    unsigned char pwm = 0;

    // Illumination value for LED 1 and 3, initially illuminated
    unsigned char led1 = 0xFF;
    unsigned char led3 = 0xFF;
    
    // LED positions in PORTA array
    unsigned char position1 = 0b00000001;
    unsigned char position3 = 0b00000100;
    
    setup();

    while(1) {
        // LED number 1 compare
        if (pwm > led1) {
            PORTA |= position1;  // Turn on
        } else {
            PORTA &= ~position1; // Turn off
        }
        // LED number 3 compare
        if (pwm > led3) {
            PORTA |= position3;  // Turn on
        } else {
            PORTA &= ~position3; // Turn off
        }

        // Move leds, slowed down by TIMER0
        if (_doMove == TRUE) {
            led1++; // Increase illumination
            led3--; // Decrease illumination
            
            // If the first LED is completely illuminated, move to the next LED.
            // LED in the middle always remains completely illuminated
            if (led1 == 0xFF) {
                position1 <<= 1;
                position3 <<= 1;

                // If a position reaches the end of the LED array,
                // restart at the beginning of the LED array
                if (position1 == 0x00) {
                    position1 = 0x01;
                }
                if (position3 == 0x00) {
                    position3 = 0x01;
                }
            }

            _doMove = FALSE;
        }

        pwm++; // turns over automatically, 255 + 1 = 0
    }
}

