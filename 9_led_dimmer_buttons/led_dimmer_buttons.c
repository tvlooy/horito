/**
 * MikroElektronika UNI-DS 3 development system
 * MCU: AVR ATmega128
 * Osc: 10 MHz
 *
 * Author: Tom Van Looy <tom$ctors,net>
 * License: http://ctors.net/isc_license.txt
 */

void setup(void);

/**
 * Hardware setup
 */
void setup(void)
{
    DDRC = 0xFF;
    PORTC = 0x00;
}

/**
 * Program entry point
 */
void main(void)
{
    unsigned char i = 0;      // counter
    unsigned char pwm[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    unsigned char buttonUp = 0;
    unsigned char buttonDown = 0;
    unsigned char lock = 0;
    
    setup();

    while(1) {
        buttonUp = PINA;
        buttonDown = PINB;

        // compare values of up/down button array
        switch (buttonUp) {
            case 1:
                buttonUp = 0;
                break;
            case 2:
                buttonUp = 1;
                break;
            case 4:
                buttonUp = 2;
                break;
            case 8:
                buttonUp = 3;
                break;
            case 16:
                buttonUp = 4;
                break;
            case 32:
                buttonUp = 5;
                break;
            case 64:
                buttonUp = 6;
                break;
            case 128:
                buttonUp = 7;
                break;
            default:
                buttonUp = 8; // disable
        }
        switch (buttonDown) {
            case 1:
                buttonDown = 0;
                break;
            case 2:
                buttonDown = 1;
                break;
            case 4:
                buttonDown = 2;
                break;
            case 8:
                buttonDown = 3;
                break;
            case 16:
                buttonDown = 4;
                break;
            case 32:
                buttonDown = 5;
                break;
            case 64:
                buttonDown = 6;
                break;
            case 128:
                buttonDown = 7;
                break;
            default:
                buttonDown = 8; // disable
        }

// -- up -----------------------------------------------------------------------
        if (buttonUp < 8 && lock == 0) {
            lock = 1;
            if (pwm[buttonUp] < 255) {
                pwm[buttonUp] += 1;
            }
        }
        if (lock == 1) {
            Delay_ms(100);
        }
//        if (buttonUp == 8) {
//            lock = 0;
//        }

// -- down ---------------------------------------------------------------------
        if (buttonDown < 8 && lock == 0) {
            lock = 1;
            if (pwm[buttonUp] > 0) {
                pwm[buttonDown] -= 1;
            }
        }
        if (lock == 1) {
            Delay_ms(100);
        }
//        if (buttonDown == 8) {
//            lock = 0;
//        }

// -- LEDs ---------------------------------------------------------------------

        // LED number 1
        if (i > pwm[0]) {
            PORTC |= 0b00000001; // turn on
        }
        else {
            PORTC &= 0b11111110; // turn off
        }
        // LED number 2
        if (i > pwm[1]) {
            PORTC |= 0b00000010; // turn on
        }
        else {
            PORTC &= 0b11111101; // turn off
        }
        // LED number 3
        if (i > pwm[2]) {
            PORTC |= 0b00000100; // turn on
        }
        else {
            PORTC &= 0b11111011; // turn off
        }
        // LED number 4
        if (i > pwm[3]) {
            PORTC |= 0b00001000; // turn on
        }
        else {
            PORTC &= 0b11110111; // turn off
        }
        // LED number 5
        if (i > pwm[4]) {
            PORTC |= 0b00010000; // turn on
        }
        else {
            PORTC &= 0b11101111; // turn off
        }
        // LED number 6
        if (i > pwm[5]) {
            PORTC |= 0b00100000; // turn on
        }
        else {
            PORTC &= 0b11011111; // turn off
        }
        // LED number 7
        if (i > pwm[6]) {
            PORTC |= 0b01000000; // turn on
        }
        else {
            PORTC &= 0b10111111; // turn off
        }
        // LED number 8
        if (i > pwm[7]) {
            PORTC |= 0b10000000; // turn on
        }
        else {
            PORTC &= 0b01111111; // turn off
        }

        i++; // turns over automatically, 255 + 1 = 0
        
    }
}

