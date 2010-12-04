/**
 * MikroElektronika UNI-DS 3 development system
 * MCU: AVR ATmega128
 * Osc: 10 MHz
 *
 * Author: Tom Van Looy <tom$ctors,net>
 * License: http://ctors.net/isc_license.txt
 *
 * On the board, set dip switches of PORTA, PORTB, PORTC, PORTD to OFF.
 * This program reads the button array RE0-RE7 and multiplies the value with 2.
 * This value is then displayed on the LCD and on LED array RF0-RF7.
 */

char GLCD_DataPort at PORTC;
char GLCD_DataPort_Direction at DDRC;

sbit GLCD_CS1 at PORTA2_bit;
sbit GLCD_CS2 at PORTA3_bit;
sbit GLCD_RS  at PORTA4_bit;
sbit GLCD_RW  at PORTA5_bit;
sbit GLCD_EN  at PORTA6_bit;
sbit GLCD_RST at PORTA7_bit;

sbit GLCD_CS1_Direction at DDA2_bit;
sbit GLCD_CS2_Direction at DDA3_bit;
sbit GLCD_RS_Direction  at DDA4_bit;
sbit GLCD_RW_Direction  at DDA5_bit;
sbit GLCD_EN_Direction  at DDA6_bit;
sbit GLCD_RST_Direction at DDA7_bit;

void setup(void);

/**
 * Hardware setup
 */
void setup(void)
{
    DDRE = 0b00000000; // PORTE is input
    DDRF = 0b11111111; // PORTF is output
    Glcd_Init();       // Initialize GLCD
    Glcd_Fill(0x00);   // Clear GLCD
}

/**
 * Program entry point
 */
int main(void)
{
    char text[255] = "";
    unsigned int in = 0;
    unsigned int in_old = 0;
    int out = 0;

    setup();

    while (1) {
        in = PINE;

        // Only write to the LCD when the values have changed
        if (in != in_old) {
            out = in * 2;
            in_old = in;

            // Display result on LEDs and LCD
            PORTF = out;
            Glcd_Fill(0x00); // Clear GLCD
            sprintf(text, "IN = %03d, OUT = %03d", in, out);
            Glcd_Write_Text(text, 0, 1, 1);
        }
    }
}

