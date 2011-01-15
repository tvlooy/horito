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
 * This program displays the text or bitmap you send to it over RS232.
 */

/**
 * Buffer should be able to receive text or bitmap
 * Type 1, text   = type flag + 8 x 21 chars + 7 enters / 1 escape = 177
 * Type 2, bitmap = type flag + 1024 values = 1025
 */
#define BUFFERSIZE 1024

volatile unsigned char _rxBuffer[BUFFERSIZE]; // receive data buffer
volatile int _bufferPos = 0;        // position in the buffer while receiving
volatile unsigned char _status = 1; // 1 = idle, 2 = receiving
volatile unsigned char _type = 0;   // 1 = receive data, 2 = display data

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
    UART1_Init_Advanced(          // UART 1
        9600,                     // Baudrate
        _UART_ODDPARITY,          // Odd parity
        _UART_TWO_STOPBITS        // Two stop-bits
    );
    UCSR0B = UCSR0B | 0b10000000; // Enable RX interrupt
}

/**
 * RS232 receive data interrupt
 */
void dataReceived() iv IVT_ADDR_USART0__RX
{
    unsigned char new;

    // Only accept data when not displaying
    if (_status == 1) {
        new = UDR0;
        // First character is data type (text/bitmap)
        if (_type == 0) {
            _type = new;
        } else {
            _rxBuffer[_bufferPos] = new;
            _bufferPos++;
        }
        // Bitmap and char 27 (escape) _or_ bitmap and buffer full
        if ( (_type == 1 && new == 27) ||
             (_type == 2 && _bufferPos == BUFFERSIZE) )
        {
            // Data received, start displaying
            _status = 2;
            _bufferPos = 0;
        }
    }
}

/**
 * Program entry point
 */
void main(void)
{
    unsigned char message[22] = ""; // Line message 21 chars + \0
    int i, j, x, y;
    int line = 1;

    setup();
    _type = 0;
    _status = 1;
    
    while(1) {
        // Only display data when not receiving
        if (_status == 2) {
            Glcd_Fill(0x00);
            
            // Data type text
            if (_type == 1) {
                for (i = 0, line = 0; _rxBuffer[i] != 27; i++) {
                    // Add chars to local message buffer. Display line by line,
                    // new line at 13 (carriage return)
                    if (_rxBuffer[i] != 13) {
                        sprintf(message, "%s%c", message, _rxBuffer[i]);
                    } else {
                        Glcd_Write_Text(message, 0, line, 1);
                        sprintf(message, "");
                        line++;
                    }
                }
                Glcd_Write_Text(message, 0, line, 1);
                sprintf(message, "");
            }
            // Data type bitmap
            else if (_type == 2) {
                // Read buffer byte by byte, display pixel by pixel. Glcd_Image()
                // should work with _rxBuffer, but didn't compile for some reason
                for (i = 0, x = 0, y = 0; i < BUFFERSIZE; i++) {
                    if (i % 128 == 0 && i != 0) {
                        y += 8;
                        x = 0;
                    }
                    for (j = 1; j < 256; j *= 2) {
                        if ((_rxBuffer[i] & j) > 0) {
                            Glcd_Dot(x, y, 1);
                        }
                        y += 1;
                    }
                    y -= 8;
                    x += 1;
                }
            }
            
            // Reset type and listening for new data
            _type = 0;
            _status = 1;
        }
    }
}

