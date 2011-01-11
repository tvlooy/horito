/**
 * MikroElektronika UNI-DS 3 development system
 * MCU: AVR ATmega128
 * Osc: 10 MHz
 *
 * Author: Tom Van Looy <tom$ctors,net>
 * License: http://ctors.net/isc_license.txt
 *
 * Set all DIP switches to OFF, except:
 * ETH-INT, ETH-WOL, ETH-CS, ETH-RST, MOSI, MISO and SCK.
 *
 * This program runs an HTTP server on the board using the onboard ENC28J60 chip.
 * It responds GET requests on port 80 with a JSON string containing the current
 * temperature. The temperature is measured by an external Dallas DS18B20 sensor.
 * The board communicates with the sensor over a 1-Wire interface.
 */

#include  "__EthEnc28j60.h"

sfr sbit SPI_Ethernet_Rst at PORTA1_bit;
sfr sbit SPI_Ethernet_CS  at PORTB7_bit;
sfr sbit SPI_Ethernet_Rst_Direction at DDA1_bit;
sfr sbit SPI_Ethernet_CS_Direction  at DDB7_bit;

/**
 * Variables
 */
char temperature[] = "000.0000";

/**
 * Function prototypes
 */
void setup(void);
void setTemperature(void);

/**
 * Hardware setup
 */
void setup(void)
{
    unsigned char myMacAddr[6] = { 0x00, 0x14, 0xA5, 0x76, 0x19, 0x3f };  // my MAC address
    unsigned char myIpAddr[4]  = {  10,  55,  55, 31 };                   // my IP address
    unsigned char gwIpAddr[4]  = {  10,  55,  55,  1 };                   // gateway (router) IP address
    unsigned char ipMask[4]    = { 255, 255, 255,  0 };                   // network mask (for example : 255.255.255.0)
    unsigned char dnsIpAddr[4] = {  10,  55,  55,  1 };                   // DNS server IP address

    SPI1_Init();

    // Set MAC address, IP address and start full-duplex (0x01)
    SPI_Ethernet_Init(myMacAddr, myIpAddr, 0x01);

    // Configure networking
    SPI_Ethernet_confNetwork(ipMask, gwIpAddr, dnsIpAddr);
}

/**
 * Read temperature and set it in global temperature variable
 */
void setTemperature(void)
{
    const unsigned short TEMP_RESOLUTION = 12;
    const unsigned short RES_SHIFT = TEMP_RESOLUTION - 8;
    char temp_whole;
    unsigned int temp_fraction;
    unsigned temp;

    Ow_Reset(&PORTE, 7);       // Onewire reset signal
    Ow_Write(&PORTE, 7, 0xCC); // Issue command SKIP_ROM
    Ow_Write(&PORTE, 7, 0x44); // Issue command CONVERT_T
//    Delay_us(120);

    Ow_Reset(&PORTE, 7);
    Ow_Write(&PORTE, 7, 0xCC); // Issue command SKIP_ROM
    Ow_Write(&PORTE, 7, 0xBE); // Issue command READ_SCRATCHPAD
//    Delay_ms(400);

    temp =  Ow_Read(&PORTE, 7);
    temp = (Ow_Read(&PORTE, 7) << 8) + temp;

    // Check if temperature is negative
    if (temp & 0x8000) {
        temperature[0] = '-';
        temp = ~temp + 1;
    }

    // Extract temp_whole
    temp_whole = temp >> RES_SHIFT ;

    // Convert temp_whole to characters
    if (temp_whole / 100) {
        temperature[0] = temp_whole / 100 + 48;
    }
    else {
        temperature[0] = '0';
    }

    temperature[1] = (temp_whole / 10) % 10 + 48;     // Extract tens digit
    temperature[2] =  temp_whole % 10       + 48;     // Extract ones digit

    // Extract temp_fraction and convert it to unsigned int
    temp_fraction  = temp << (4 - RES_SHIFT);
    temp_fraction &= 0x000F;
    temp_fraction *= 625;

    // Convert temp_fraction to characters
    temperature[4] =  temp_fraction / 1000      + 48; // Extract thousands digit
    temperature[5] = (temp_fraction / 100) % 10 + 48; // Extract hundreds digit
    temperature[6] = (temp_fraction / 10) % 10  + 48; // Extract tens digit
    temperature[7] =  temp_fraction % 10        + 48; // Extract ones digit
}

/**
 * this function is called by the library
 * the user accesses to the HTTP request by successive calls to Spi_Ethernet_getByte()
 * the user puts data in the transmit buffer by successive calls to Spi_Ethernet_putByte()
 * the function must return the length in bytes of the HTTP reply, or 0 if nothing to transmit
 *
 * if you don't need to reply to HTTP requests,
 * just define this function with a return(0) as single statement
 */
unsigned int SPI_Ethernet_UserTCP(
    unsigned char *remoteHost,
    unsigned int remotePort,
    unsigned int localPort,
    unsigned int reqLength,
    TEthPktFlags *flags)
{
    const code unsigned char httpHeader[] = "HTTP/1.1 200 OK\nContent-type: text/json\n\n";
    unsigned char httpMethod[] = "GET /";
    unsigned char getRequest[15]; // HTTP request buffer

    unsigned int len; // my reply length

    // Only serve requests on port 80 (HTTP)
    if (localPort != 80) {
        return(0);
    }

    // Get the first 10 bytes of the request
    for(len = 0; len < 10; len++) {
        getRequest[len] = SPI_Ethernet_getByte();
    }
    getRequest[len] = '\0'; // Terminate string

    // Only servewith HTTP GET requests
    // method is in the first 5 bytes, compare these
    if (memcmp(getRequest, httpMethod, 5)) {
        return(0);
    }

    len = 0;
    // Return server API version
    if (getRequest[5] == 'v') {
        len =  SPI_Ethernet_putConstString(httpHeader); // output HTTP header
        len += SPI_Ethernet_putConstString(
            "[{\"author\":\"Tom Van Looy <tom@ctors.net>\",\"version\":\"0.0.1\"}]"
        );
    }
    // By default, send temperature (and maybe time from RTC over I2C)
    else {
        // If the request is ok, read and save temperature in dyna
        setTemperature();
    
        len =  SPI_Ethernet_putConstString(httpHeader);
        len += SPI_Ethernet_putConstString("[{\"temperature\":\"");
        len += SPI_Ethernet_putString(temperature);
        len += SPI_Ethernet_putConstString("\"}]");
    }

    return(len); // return the number of bytes to write to transmit
}

/**
 * this function is called by the library
 * the user accesses to the UDP request by successive calls to Spi_Ethernet_getByte()
 * the user puts data in the transmit buffer by successive calls to Spi_Ethernet_putByte()
 * the function must return the length in bytes of the UDP reply, or 0 if nothing to transmit
 *
 * if you don't need to reply to UDP requests,
 * just define this function with a return(0) as single statement
 */
unsigned int SPI_Ethernet_UserUDP(
    unsigned char *remoteHost,
    unsigned int remotePort,
    unsigned int destPort,
    unsigned int reqLength,
    TEthPktFlags *flags)
{
    return 0;
}

/**
 * Program entry point
 */
void main(void)
{
    setup();

    while(1) {
        // Don't test for error codes, just keep processing incomping packets
        SPI_Ethernet_doPacket();
    }
}

