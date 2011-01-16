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
char _temperature[] = "+000.0000";

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
    unsigned char macAddr[6]   = {                      // My MAC address
        0x00, 0x14, 0xA5, 0x76, 0x19, 0x3f
    };
    unsigned char ipAddr[4]    = {  10,  55,  55, 31 }; // My IP address
    unsigned char gwIpAddr[4]  = {  10,  55,  55,  1 }; // Gateway IP address
    unsigned char ipMask[4]    = { 255, 255, 255,  0 }; // Network mask
    unsigned char dnsIpAddr[4] = {  10,  55,  55,  1 }; // DNS server IP address

    SPI1_Init();

    // Set MAC address, IP address and start full-duplex (0x01)
    SPI_Ethernet_Init(macAddr, ipAddr, 0x01);

    // Configure networking
    SPI_Ethernet_confNetwork(ipMask, gwIpAddr, dnsIpAddr);
}

/**
 * Read temperature and set it in global temperature variable
 */
void setTemperature(void)
{
    unsigned int temp_full;
    char temp_whole;
    int temp_fraction;

    Ow_Reset(&PORTE, 7);       // Onewire reset signal
    Ow_Write(&PORTE, 7, 0xCC); // Issue command SKIP_ROM
    Ow_Write(&PORTE, 7, 0x44); // Issue command CONVERT_T

    Ow_Reset(&PORTE, 7);
    Ow_Write(&PORTE, 7, 0xCC); // Issue command SKIP_ROM
    Ow_Write(&PORTE, 7, 0xBE); // Issue command READ_SCRATCHPAD

    // Read 16 bits
    temp_full =  Ow_Read(&PORTE, 7);
    temp_full = (Ow_Read(&PORTE, 7) << 8) + temp_full;

    // Set sign if temperature negative and make positive
    if (temp_full & 0x8000) {
        _temperature[0] = '-';
        temp_full = ~temp_full + 1; // Sign-extended two's complement format
    } else {
        _temperature[0] = '+';
    }

    // Extract temp_whole
    temp_whole = temp_full >> 4;

    // Convert temp_whole to characters
    _temperature[1] = (temp_whole / 100) % 10 + 48; // Extract hundreds
    _temperature[2] = (temp_whole /  10) % 10 + 48; // Extract tens
    _temperature[3] =  temp_whole %  10       + 48; // Extract ones

    // Extract temp_fraction and convert it to unsigned int
    temp_fraction = temp_full & 0x000F;
    temp_fraction *= 625;

    // Convert temp_fraction to characters
    _temperature[5] = (temp_fraction / 1000)      + 48; // Extract thousands
    _temperature[6] = (temp_fraction /  100) % 10 + 48; // Extract hundreds
    _temperature[7] = (temp_fraction /   10) % 10 + 48; // Extract tens
    _temperature[8] =  temp_fraction %   10       + 48; // Extract ones
}

/**
 * TCP request handler routine
 */
unsigned int SPI_Ethernet_UserTCP(
    unsigned char *remoteHost,
    unsigned int remotePort,
    unsigned int localPort,
    unsigned int reqLength,
    TEthPktFlags *flags)
{
    const code unsigned char httpHeader[] =
        "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n";
    unsigned char httpMethodGet[] = "GET /";
    unsigned char requestData[7]; // HTTP request buffer
    unsigned int replyLength;

    // Only serve requests on port 80 (HTTP)
    if (localPort != 80) {
        return(0);
    }

    // Get the first 6 bytes of the request
    for (replyLength = 0; replyLength < 6; replyLength++) {
        requestData[replyLength] = SPI_Ethernet_getByte();
    }
    requestData[replyLength] = '\0'; // Terminate string

    // Only servewith HTTP GET requests
    // method is in the first 5 bytes, compare these
    if (memcmp(requestData, httpMethodGet, 5)) {
        return(0);
    }

    replyLength = 0;
    // Return server API version
    if (requestData[5] == 'v') {
        replyLength =  SPI_Ethernet_putConstString(httpHeader); // Output header
        replyLength += SPI_Ethernet_putConstString(
            "{\"author\":\"Tom Van Looy <tom@ctors.net>\",\"version\":\"0.1\"}"
        );
    }
    // By default, send temperature (and maybe time from RTC over I2C)
    else {
        // If the request is ok, read and save temperature in dyna
        setTemperature();
    
        replyLength =  SPI_Ethernet_putConstString(httpHeader);
        replyLength += SPI_Ethernet_putConstString("{\"temperature\":\"");
        replyLength += SPI_Ethernet_putString(_temperature);
        replyLength += SPI_Ethernet_putConstString("\"}");
    }

    return(replyLength); // return the number of bytes to write to transmit
}

/**
 * UDP request handler routine
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
        // Process incoming packets as quick as possible
        SPI_Ethernet_doPacket();
    }
}

