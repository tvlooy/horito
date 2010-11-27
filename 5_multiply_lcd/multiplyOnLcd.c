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

// -- global variables ---------------------------------------------------------
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

int main(void)
{
    int old_number = 0;
    char text[255] = "";
    int in = 0;
    int out = 0;
    unsigned char number = 0;
    
    DDRE = 0b00000000; // PORTE is input
    DDRF = 0b11111111; // PORTF is output
    Glcd_Init();       // Initialize GLCD
    Glcd_Fill(0x00);   // Clear GLCD

    while (1) {
        old_number = number;
        number = PINE;

        in = number;
        out = number * 2;
        PORTF = out;
        
        // Only write to the LCD when the values have changed
        if (old_number != number) {
            Glcd_Fill(0x00); // Clear GLCD
            strcpy(text, "IN = 000, OUT = 000");
            
            // I couldn't use sprintf() to assemble the text and had to do
            // it this way. Using spintf() made the binary larger and the MikroE
            // compiler gave a "demo limit" error. WTF MikroE! You suck! I buy
            // a development board of 200 Euro and can't develop for it??
            if (in >= 100) {
                text[5] = ((in / 100) + 48);
                in -= (in / 100 * 100);
            }
            if (in >= 10) {
                text[6] = ((in / 10) + 48);
                in -= (in / 10 * 10);
            }
            text[7] = in + 48;
            if (out >= 100) {
                text[16] = ((out / 100) + 48);
                out -= (out / 100 * 100);
            }
            if (out >= 10) {
                text[17] = ((out / 10) + 48);
                out -= (out / 10 * 10);
            }
            text[18] = out + 48;
            
            Glcd_Write_Text(text, 0, 1, 1);
        }
    }
}

