/*
 * file         : SPI_Ethernet_libdef.c
 * project      : Ethernet controller SPI_Ethernet mini library
 * author       : Bruno Gavand
 * compiler     : mikroC V8.0.0.0
 * ctime        : jan. 2008
 * mtime        : jan. 2008
 * release      : 1.0
 *
 * description  :
 *      definitions for SPI_Ethernet mini lib
 *
 * Small typographic cleanups by Tom Van Looy <tom$ctors,net>
 */

/*
 * registers
 */
#define ECON1    0x1f
#define ECON2    0x1e
#define MIREGADR 0x14
#define MIWRL    0x16
#define MIWRH    0x17

#define ERDPT    0x00
#define ERDPTH   0x01
#define EWRPT    0x02
#define EWRPTH   0x03
#define ETXST    0x04
#define ETXSTH   0x05
#define ETXND    0x06
#define ETXNDH   0x07
#define ERXST    0x08
#define ERXSTH   0x09
#define ERXND    0x0a
#define ERXNDH   0x0b
#define ERXRDPT  0x0c
#define ERXRDPTH 0x0d
#define EDMAST   0x10
#define EDMASTH  0x11
#define EDMAND   0x12
#define EDMANDH  0x13
#define EDMADST  0x14
#define EDMADSTH 0x15
#define EDMACS   0x16
#define EDMACSH  0x17


#define ERXFCON  0x18
#define MACON1   0x00
#define MACON2   0x01
#define MACON3   0x02
#define MACON4   0x03
#define MAMXFL   0x0a
#define MAMXFLH  0x0b
#define MABBIPG  0x04
#define MAIPGL   0x06
#define MAIPGH   0x07
#define MAADR1   0x04
#define MAADR2   0x05
#define MAADR3   0x02
#define MAADR4   0x03
#define MAADR5   0x00
#define MAADR6   0x01
#define EREVID   0x12
#define ECOCON   0x15
#define EPKTCNT  0x19
#define EIR      0x1c
#define EIE      0x1b
#define ESTAT    0x1d

#define MACLCON1 0x08
#define MACLCON2 0x09

#define MICMD    0x12

#define MIIRD    0x01

/*
 * PHY registers
 */
#define PHLCON  0x14
#define PHCON2  0x10
#define PHCON1  0x00

/*
 * SPI commands
 */
#define WBMCMD  0b01111010      // write buffer memory
#define RBMCMD  0b00111010      // read buffer memory
#define BFSCMD  0b10000000      // bit field set
#define BFCCMD  0b10100000      // bit field clear
#define WCRCMD  0b01000000      // write control register
#define RCRCMD  0b00000000      // read control register
#define RSTCMD  0b11111111      // reset

/*
 * maximum packet length
 */
#define BUF_SIZE        1518

/*
 * ENC memory allocation
 */
#define RAM_SIZE        8192                            // 8kb RAM available
#define RECEIVE_START   0                               // receive buffer start address : Should be an even memory address; must be 0 for errata
#define RECEIVE_END     (TRANSMIT_START - 1)            // receive buffer end address   : Odd for errata workaround
#define TRANSMIT_START  (RAM_SIZE - (BUF_SIZE + 100))   // transmit buffer start address, a few more bytes for padding pseudo header + transmit status : Even memory address
#define REPLY_START     (TRANSMIT_START + 1)            // reply buffer starts after per packet control byte
#define RECEIVE_SIZE    (RECEIVE_END-RECEIVE_START+1)   // receive buffer size

#define SPI_Ethernet_HALFDUPLEX     0
#define SPI_Ethernet_FULLDUPLEX     1

#define NO_ADDR 0xFFFF

/*
 * library globals
 */
typedef struct
        {
        unsigned char   valid;                             // valid/invalid entry flag
        unsigned long   tmr;                               // timestamp
        unsigned char   ip[4];                             // IP address
        unsigned char   mac[6];                            // MAC address behind the IP address
        } SPI_Ethernet_arpCacheStruct;

extern SPI_Ethernet_arpCacheStruct SPI_Ethernet_arpCache[]; // ARP cache, 3 entries max

extern unsigned char    SPI_Ethernet_macAddr[6];           // MAC address of the controller
extern unsigned char    SPI_Ethernet_ipAddr[4];            // IP address of the device
extern unsigned char    SPI_Ethernet_gwIpAddr[4];          // GW
extern unsigned char    SPI_Ethernet_ipMask[4];            // network mask
extern unsigned char    SPI_Ethernet_dnsIpAddr[4];         // DNS serveur IP
extern unsigned char    SPI_Ethernet_rmtIpAddr[4];         // remote IP Address of host (DNS server reply)

extern unsigned long    SPI_Ethernet_userTimerSec;         // must be incremented by user 1 time per second

typedef struct {
  unsigned canCloseTCP: 1;
  unsigned isBroadcast: 1;
} TEthPktFlags;

/*
 * prototypes for public functions
 */
extern  void            SPI_Ethernet_Init(unsigned char *resetPort, unsigned char resetBit, unsigned char *CSport, unsigned char CSbit, unsigned char *mac, unsigned char *ip, unsigned char fullDuplex);
extern  unsigned char   SPI_Ethernet_doPacket();
extern  void            SPI_Ethernet_putByte(unsigned char b);
extern  void            SPI_Ethernet_putBytes(unsigned char *ptr, unsigned int n);
extern  void            SPI_Ethernet_putConstBytes(const unsigned char *ptr, unsigned int n);
extern  unsigned char   SPI_Ethernet_getByte();
extern  void            SPI_Ethernet_getBytes(unsigned char *ptr, unsigned int addr, unsigned int n);
extern  unsigned int    SPI_Ethernet_UserUDP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags * flags);
extern  unsigned int    SPI_Ethernet_UserTCP(unsigned char *remoteHost, unsigned int remotePort, unsigned int localPort, unsigned int reqLength, TEthPktFlags * flags);
extern  void            SPI_Ethernet_confNetwork(char *ipMask, char *gwIpAddr, char *dnsIpAddr);

