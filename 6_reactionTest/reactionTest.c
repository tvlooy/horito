/**
 * MikroElektronika UNI-DS 3 development system
 * MCU: AVR ATmega128
 * Osc: 10 MHz
 *
 * Author: Tom Van Looy <tom$ctors,net>
 * License: http://ctors.net/isc_license.txt
 *
 * On the board, set dip switches of PORTA, PORTB, PORTC and PORTE to OFF.
 *
 * This program is a reaction tester game. When the LEDs of PORTD are turned on,
 * the user must press button RD0 as quick as possible. The reaction speed is
 * displayed on the LCD.
 */

volatile enum BOOLEAN { TRUE, FALSE }
    _buttonPressed = FALSE, // The player pressed the button
    _finished = FALSE,      // The game is finished
    _started = FALSE,       // The game is started
    _cheater = FALSE;       // The player pressed prematurely
volatile int _milliseconds = 0;

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
    SREG_I_bit = 1;      // Global interrupts enable
    EIMSK |= 0b00000001; // INT0 enable
    EICRA |= 0b00000011; // Trigger INT0 on rising edge

    TCCR0 = 0b00111100;  // Timer/counter register
    OCR0 = 0x9C;         // Output compare
    TIMSK |= 0b00000010; // Interrupt on compare match
    
    DDRD = 0xFF;         // Set the LED pins to output
    PORTD = 0x00;        // Turn off all the LED's
    Glcd_Init();         // Initialize GLCD
    Glcd_Fill(0x00);     // Clear GLCD
}

/**
 * External interrupt 0
 */
void ButtonPressed() iv IVT_ADDR_INT0
{
    if (_started == TRUE) {
        _buttonPressed = TRUE;
     } else {
        _cheater = TRUE;
     }
}

/**
 * Timer 0 output compare match interrupt
 */
void Timer0CompareMatch() iv IVT_ADDR_TIMER0_COMP
{
    if (_started == TRUE && _finished == FALSE) {
        _milliseconds++;
        if (_milliseconds > 10000) {
            _buttonPressed = TRUE;
        }
    }
}

/**
 * Program entry point
 */
void main(void)
{
    char text[256] = "";
    setup();
    
    Glcd_Write_Text("                     ", 0, 2, 0);
    Glcd_Write_Text("*** ARE YOU READY ***", 0, 3, 0);
    Glcd_Write_Text("                     ", 0, 4, 0);
    _started = FALSE;
    _cheater = FALSE;

    // Wait min 2 sec, max 6 sec
    VDelay_ms(((rand() % 5) + 2) * 1000);

    _buttonPressed = FALSE;
    _finished = FALSE;
    _started = TRUE;
    PORTD = 0b11110000;
    
    // If user didn't cheat, flag start
    if (_cheater == FALSE) {
        Glcd_Fill(0x00);
        Glcd_Write_Text("        o_________", 0, 1, 1);
        Glcd_Write_Text("       / /       /", 0, 2, 1);
        Glcd_Write_Text("      / / START /",  0, 3, 1);
        Glcd_Write_Text("     / /_______/",   0, 4, 1);
        Glcd_Write_Text("    / /",            0, 5, 1);
        Glcd_Write_Text("   / /",             0, 6, 1);
        Glcd_Write_Text("  /_/",              0, 7, 1);
    }

    while(1) {
        // If user cheated, show a message and stop game.
        // Start flickering the LED array
        if (_cheater == TRUE && _finished == FALSE) {
            PORTD = 0b01010101;
            Glcd_Fill(0x00);
            Glcd_Write_Text("                     ", 0, 0, 0);
            Glcd_Write_Text("***** KITTENS *******", 0, 1, 0);
            Glcd_Write_Text("Every time you press ", 0, 3, 1);
            Glcd_Write_Text("early ... God kills a", 0, 4, 1);
            Glcd_Write_Text("kitten. Please, think", 0, 5, 1);
            Glcd_Write_Text("of the kittens!      ", 0, 6, 1);
            _finished = TRUE;
        }
        if (_cheater == TRUE) {
            Delay_ms(500);
            PORTD = ~PORTD;
        }

        // If the user didn't press the button within 10 seconds, show a message
        // and end the game. Start flickering the LED array
        if (_buttonPressed == TRUE && _finished == FALSE && _started == TRUE) {
            Glcd_Fill(0x00);
            if (_milliseconds > 10000) {
                PORTD = 0b01010101;
                Glcd_Write_Text(" __________",            0, 0, 1);
                Glcd_Write_Text("|          |",           0, 1, 1);
                Glcd_Write_Text("| You _so_ |   ____",    0, 2, 1);
                Glcd_Write_Text("| slow !!! |  o o)\\\\", 0, 3, 1);
                Glcd_Write_Text("|_______  _| /<   c",    0, 4, 1);
                Glcd_Write_Text("        \\|   \\o__/(",  0, 5, 1);
                Glcd_Write_Text("                \\  \\", 0, 6, 1);
                _cheater = TRUE;
            }
            // If the user pressed the button, show his reaction time.
            // End the game
            else {
                PORTD = 0b00001111;
                sprintf(text, "Your score: %d ms    ", _milliseconds);
                Glcd_Write_Text("***** FINISHED ****** ",  0, 0, 0);
                Glcd_Write_Text(text,  0, 1, 0);
                Glcd_Write_Text("       _______    |  ",  0, 3, 1);
                Glcd_Write_Text("      (       )  -.- ",  0, 4, 1);
                Glcd_Write_Text("  |  (_\\     /_)  |  ", 0, 5, 1);
                Glcd_Write_Text(" -.-    ) 1 (        ",  0, 6, 1);
                Glcd_Write_Text("  |    /_____\\       ", 0, 7, 1);
            }
            _finished = TRUE;
        }
    }
}

