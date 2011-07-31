/**
 * MikroElektronika UNI-DS 3 development system
 * MCU: AVR ATmega128
 * Osc: 10 MHz
 *
 * Author: Tom Van Looy <tom$ctors,net>
 * License: http://ctors.net/isc_license.txt
 *
 * Set DIP switches RX232A and TX232A to ON.
 * Set DIP switches PORTA, PORTB, PORTC, PORTD and PORTE to OFF.
 * This program displays the text you send to it over RS232.
 *   - DEL to go back
 *   - CR for a new line
 *   - ESC to restart
 */

#define BUFFERSIZE 184 // full screen = 22 chars per row, 8 rows + 8 returns

volatile unsigned char _rxbuffer[BUFFERSIZE];
volatile int _bufferSize = 0;
volatile int _bufferPos = 0;

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
    Glcd_Init();                  // Initialize GLCD
    Glcd_Fill(0x00);              // Clear GLCD

    SREG_I_bit = 1;               // Global interrupts enable
    UART1_Init(9600);             // UART 1 baudrate
    UCSR0B = UCSR0B | 0b10000000; // Enable RX interrupt
}

/**
 * RS232 receive data interrupt
 */
void dataReceived() iv IVT_ADDR_USART0__RX
{
    if (_bufferSize < BUFFERSIZE) {
        _rxbuffer[_bufferPos] = UDR0;
        _bufferPos++;
        _bufferSize++;
        if (_bufferPos == BUFFERSIZE) {
            _bufferPos = 0;
        }
    }
    //else if (_bufferSize == BUFFERSIZE) {
    //    _rxBuffer[_bufferPos] = UDR0;
    //}
}

/**
 * Program entry point
 */
void main(void)
{
    char message[64] = "";     // Line message
    unsigned char line = 0;    // GLCD line number
    unsigned char newChar = 0; // New received character
    int bufferPos = 0;         // Position in the buffer

    setup();

    while(1) {
        if (_bufferSize > 0) {
            newChar = _rxBuffer[bufferPos];
            bufferPos++;
            _bufferSize--;
            if (bufferPos == BUFFERSIZE) {
                bufferPos = 0;
            }

            // DEL = go back one character
            if (newChar == 127) {
                message[strlen(message) - 1] = ' ';
                Glcd_Write_Text(message, 0, line, 1);
                message[strlen(message) - 1] = '\0';
                newChar = ' ';
            }
            // CR = go to the next line
            else if (newChar == 13) {
                if (line < 7) {
                  line++;
                  sprintf(message, "");
                }
            }
            // ESC = clear screen and restart
            else if (newChar == 27) {
                line = 0;
                sprintf(message, "");
                Glcd_Fill(0x00);
            }
            // Display character
            else {
                sprintf(message, "%s%c", message, newChar);
            }
            Glcd_Write_Text(message, 0, line, 1);
        }
    }
}

