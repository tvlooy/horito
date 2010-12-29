/**
 * MikroElektronika UNI-DS 3 development system
 * MCU: AVR ATmega128
 * Osc: 10 MHz
 *
 * Author: Tom Van Looy <tom$ctors,net>
 * License: http://ctors.net/isc_license.txt
 *
 * Set DIP switches PORTA and PORTB to OFF.
 * This program makes it possible to dim 8 leds (array C) with
 * buttons of array B for up and buttons of array A for down.
 */

#define STEP 10         // Value to go up/down
#define DENDERCOUNT 500 // Anti-dender timeout counter
#define PWM_TOP 100     // Maximum value of PWM counter (scale)

void setup(void);

/**
 * Hardware setup
 */
void setup(void)
{
    DDRC = 0xFF;  // LED array C is output
    PORTC = 0x00; // Turn LEDs initially off
}

/**
 * Program entry point
 */
void main(void)
{
    unsigned char pwmValues[8] = { 0x00 };
    unsigned char ledPos = 0;        // Led position (loop counter)
    unsigned char buttonUp = 0;      // LED position in pwmValues array for up action
    unsigned char buttonDown = 0;    // LED position in pwmValues array for down action
    unsigned char oldButtonUp = 0;   // Old buttonUp value
    unsigned char oldButtonDown = 0; // Old buttonDown value
    unsigned char pwm = 0;           // Software PWM counter
    unsigned long int lock = 0;      // Anti-dender lock
    
    setup();

    while(1) {
        buttonDown = PINA; // Read button array A (down)
        buttonUp = PINB;   // Read button array B (up)
        
        // Compare values of up/down button array
        switch (buttonUp) {
            case 1:   buttonUp = 0; break;
            case 2:   buttonUp = 1; break;
            case 4:   buttonUp = 2; break;
            case 8:   buttonUp = 3; break;
            case 16:  buttonUp = 4; break;
            case 32:  buttonUp = 5; break;
            case 64:  buttonUp = 6; break;
            case 128: buttonUp = 7; break;
            default:  buttonUp = 8; // Not pressed / disable
        }
        switch (buttonDown) {
            case 1:   buttonDown = 0; break;
            case 2:   buttonDown = 1; break;
            case 4:   buttonDown = 2; break;
            case 8:   buttonDown = 3; break;
            case 16:  buttonDown = 4; break;
            case 32:  buttonDown = 5; break;
            case 64:  buttonDown = 6; break;
            case 128: buttonDown = 7; break;
            default:  buttonDown = 8; // Not pressed / disable
        }

        // If a button is pressed (and lock is off), change up/down values
        if (lock == 0) {
            if (buttonUp < 8) {
                if (pwmValues[buttonUp] < (PWM_TOP - STEP)) {
                    pwmValues[buttonUp] += STEP;
                } else {
                    pwmValues[buttonUp] = PWM_TOP;
                }
            }
            if (buttonDown < 8) {
                if (pwmValues[buttonDown] > STEP) {
                    pwmValues[buttonDown] -= STEP;
                } else {
                    pwmValues[buttonDown] = 0;
                }
            }
        }

        // Apply anti-dender lock when values change
        if (oldButtonDown != buttonDown || oldButtonUp != buttonUp) {
           lock = 1;
           oldButtonDown = buttonDown;
           oldButtonUp = buttonUp;
        }
        // Anti-dender locking
        if (lock > 0 && lock < DENDERCOUNT) {
            lock++;
        }
        if (lock == DENDERCOUNT && buttonUp == 8 && buttonDown == 8) {
            lock = 0;
        }

        // Set the LED's PWM duty cycle
        for (ledPos = 0; ledPos < 8; ledPos++) {
            if (pwm < pwmValues[ledPos]) {
                PORTC |= (0b00000001 << ledPos);
            } else {
                PORTC &= ~(0b00000001 << ledPos);
            }
        }

        pwm++; // Turns over automatically, 255 + 1 = 0
        if (pwm >= PWM_TOP) {
            pwm = 0;
        }
    }
}

