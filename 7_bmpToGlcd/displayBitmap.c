/**
 * MikroElektronika UNI-DS 3 development system
 * MCU: AVR ATmega128
 * Osc: 10 MHz
 *
 * Author: Tom Van Looy <tom$ctors,net>
 * License: http://ctors.net/isc_license.txt
 *
 * On the board, set dip switches of PORTA and PORTC to OFF.
 *
 * This program displays a bitmap on the GLCD.
 * The bitmap must be converted with the createBitmap.c program.
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
    TIMSK |= 0b00000010; // Interrupt on compare match
    Glcd_Init();         // Initialize GLCD
    Glcd_Fill(0x00);     // Clear GLCD
}

/**
 * Program entry point
 */
void main(void)
{
    const code char image[1024] = {
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 128, 192, 192, 224, 224, 352, 352, 352, 304, 304, 304, 304,
        280, 280, 280, 280, 280, 280, 280, 280, 280, 280, 280, 280, 280, 280,
        280, 280, 280, 304, 304, 304, 304, 304, 352, 224, 224, 224, 192, 128,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 128, 128, 192, 192, 192, 192, 192,
        192, 223, 255, 255, 217, 312, 304, 304, 304, 304, 368, 368, 368, 368,
        368, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240,
        240, 240, 376, 376, 376, 380, 382, 318, 319, 319, 319, 287, 255, 255,
        255, 192, 192, 192, 192, 192, 192, 192, 192, 128, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 255, 243, 257, 376, 252, 270, 270,
        270, 270, 271, 319, 255, 252, 192, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 128, 248, 383, 271,
        270, 270, 270, 270, 142, 252, 252, 256, 129, 255, 383, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 263, 287, 380, 241, 142, 312,
        368, 192, 128, 128, 256, 263, 287, 383, 252, 224, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 192, 248, 319, 257, 256, 128,
        128, 192, 352, 312, 143, 227, 376, 318, 271, 259, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 257, 259,
        258, 270, 268, 281, 315, 306, 374, 356, 364, 233, 223, 254, 252, 240,
        224, 192, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 128, 192, 240, 248, 254, 222, 223, 205, 236, 374, 306, 283, 285,
        268, 270, 262, 259, 257, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 257, 257, 257,
        259, 259, 263, 270, 284, 240, 256, 256, 256, 256, 240, 248, 252, 284,
        263, 259, 259, 257, 257, 257, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 248, 248, 252, 254, 254, 254, 255,
        255, 255, 255, 255, 255, 255, 248, 248, 248, 248, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 254, 254, 382, 380, 252, 240, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 263, 271, 271, 268, 280, 280, 281,
        305, 305, 305, 305, 305, 307, 307, 307, 307, 307, 307, 307, 307, 305,
        305, 305, 305, 273, 281, 280, 280, 268, 268, 271, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256,
        256, 256,
    };

    setup();
    Glcd_Image(image);

    while(1) {}
}
