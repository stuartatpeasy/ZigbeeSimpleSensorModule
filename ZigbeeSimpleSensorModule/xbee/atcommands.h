#ifndef ATCOMMANDS_H_INC
#define ATCOMMANDS_H_INC
/*
    atcommands.h:   declarations relating to the XBee AT command-set

    Stuart Wallace <stuartw@atom.net>, July 2018.
*/


// Enumeration containing a list of AT commands supported by the XBee modules
//
// The value of each enumerated constant is equal to the command string in little-endian byte
// order.  E.g. AT_CMD_ATVR has value 0x5256 ('RV'); when written to memory as a little-endian
// uint16_t, this will be stored as the characters {'V', 'R'}.
//
typedef enum AT_CMD
{
    // Addressing commands
    AT_CMD_ATDH             = 0x4844,   // Set/get upper 32 bits of the 64-bit destination addr
    AT_CMD_ATDL             = 0x4c44,   // Set/get lower 32 bits of the 64-bit destination addr
    AT_CMD_ATMY             = 0x594d,   // Get the 16-bit network address of the device
    AT_CMD_ATMP             = 0x504d,   // Get the 16-bit network address of the parent device
    AT_CMD_ATNC             = 0x434e,   // Get remaining # of end-devices that can join this device
    AT_CMD_ATSH             = 0x4853,   // Get upper 32 bits of 64-bit IEEE unique source address
    AT_CMD_ATSL             = 0x4c53,   // Get lower 32 bits of 64-bit IEEE unique source address
    AT_CMD_ATNI             = 0x494e,   // Store node identifier string (20 char max)
    AT_CMD_ATSE             = 0x4553,   // Set/get application layer source endpoint value
    AT_CMD_ATDE             = 0x4544,   // Set/get application layer destination endpoint value
    AT_CMD_ATCI             = 0x4943,   // Set/get application layer ID (cluster ID)
    AT_CMD_ATTO             = 0x4f54,   // Set/get application layer transmit options
    AT_CMD_ATNP             = 0x504e,   // Get maximum packet payload in bytes
    AT_CMD_ATDD             = 0x4444,   // Set Digi device type identifier value
    AT_CMD_ATCR             = 0x5243,   // Set # of PAN ID conflicts/min to trigger a PAN ID change

    // Network commands
    AT_CMD_ATCH             = 0x4843,   // Read the operating channel (IEEE 802.15.4 channel #)
    AT_CMD_ATCE             = 0x4543,   // Set/get co-ordinator status for this device
    AT_CMD_ATID             = 0x4449,   // Set/get extended (64-bit) PAN ID
    AT_CMD_ATII             = 0x4949,   // Get pre-configured 16-bit PAN ID for network-forming
    AT_CMD_ATOP             = 0x504f,   // Get 64-bit "operating" (=in-use) extended PAN ID
    AT_CMD_ATNH             = 0x484e,   // Set/get maximum network hops
    AT_CMD_ATBH             = 0x4842,   // Set/get maximum broadcast hops
    AT_CMD_ATOI             = 0x494f,   // Read 16-bit "operating" (=in-use) PAN ID
    AT_CMD_ATNT             = 0x544e,   // Set/get node discovery timeout (units of 100ms)
    AT_CMD_ATNO             = 0x4f4e,   // Set/get network discovery options
    AT_CMD_ATSC             = 0x4353,   // Set/get the list of channels to scan (as a bitfield)
    AT_CMD_ATSD             = 0x4453,   // Set/get scan-duration exponent
    AT_CMD_ATED             = 0x4445,   // Start an energy-detect scan
    AT_CMD_ATZS             = 0x535a,   // Set/get Zigbee stack profile value
    AT_CMD_ATNJ             = 0x4a4e,   // Set/get co-ordinator node join time
    AT_CMD_ATJV             = 0x564a,   // Set/get channel verification parameter
    AT_CMD_ATNW             = 0x574e,   // Set/get network watchdog timeout
    AT_CMD_ATJN             = 0x4e4a,   // Set/get join notification setting
    AT_CMD_ATAR             = 0x5241,   // Set/get aggregate routing notification interval

    // Security commands
    AT_CMD_ATEE             = 0x4545,   // Set/get encryption enable setting
    AT_CMD_ATEO             = 0x4f45,   // Set encryption options
    AT_CMD_ATNK             = 0x4b4e,   // Set AES network-encryption key
    AT_CMD_ATKY             = 0x594b,   // Set AES link key

    // RF interfacing commands
    AT_CMD_ATPL             = 0x4c50,   // Set/get transmit power level
    AT_CMD_ATPM             = 0x4d50,   // Set/get power mode
    AT_CMD_ATDB             = 0x4244,   // Get received signal strength of last packet (in -dBm)
    AT_CMD_ATPP             = 0x5050,   // Get output power in dBm at maximum transmit power (PL4)

    // Serial interfacing commands
    AT_CMD_ATAO             = 0x4f41,   // Set API options
    AT_CMD_ATBD             = 0x4442,   // Set interface data (baud) rate
    AT_CMD_ATNB             = 0x424e,   // Set/get serial parity mode
    AT_CMD_ATSB             = 0x4253,   // Set/get serial stop bits
    AT_CMD_ATRO             = 0x4f52,   // Set/get inter-character silence length to start TX
    AT_CMD_ATD7             = 0x3744,   // Set/get pin DIO7 config (DIO7/nCTS)
    AT_CMD_ATD6             = 0x3644,   // Set/get pin DIO6 config (DIO6/nRTS)

    // I/O settings commands
    AT_CMD_ATIR             = 0x5249,   // Set/get I/O sample rate for periodic sampling
    AT_CMD_ATIC             = 0x4349,   // Set/get bitfield of DIOx pins to watch for state changes
    AT_CMD_ATP0             = 0x3050,   // Set/get P0 config (RSSI/PWM0)
    AT_CMD_ATP1             = 0x3150,   // Set/get P1 config (DIO11/PWM1)
    AT_CMD_ATP2             = 0x3250,   // Set/get P2 config (DIO12)
    AT_CMD_ATP3             = 0x3350,   // Set/get P3 config (DIO13/DOUT)
    AT_CMD_ATP4             = 0x3450,   // Set/get P4 config (DIO14/DIN)
    AT_CMD_ATP5             = 0x3550,   // Set/get P5 config (DIO15/SPI_MISO)
    AT_CMD_ATP6             = 0x3650,   // Set/get P6 config (SPI_MOSI)
    AT_CMD_ATP7             = 0x3750,   // Set/get P7 config (DIO17/SPI_nSSEL)
    AT_CMD_ATP8             = 0x3850,   // Set/get P8 config (DIO18/SPI_SCLK)
    AT_CMD_ATP9             = 0x3950,   // Set/get P9 config (DIO19/SPI_nATTN/PTI_DATA)
    AT_CMD_ATD0             = 0x3044,   // Set/get D0 config (AD0/DIO0)
    AT_CMD_ATD1             = 0x3144,   // Set/get D1 config (AD1/DIO1/PTI_EN)
    AT_CMD_ATD2             = 0x3244,   // Set/get D2 config (AD2/DIO2)
    AT_CMD_ATD3             = 0x3344,   // Set/get D3 config (AD3/DIO3)
    AT_CMD_ATD4             = 0x3444,   // Set/get D4 config (DIO4)
    AT_CMD_ATD5             = 0x3544,   // Set/get D5 config (DIO5/ASSOCIATE)
    AT_CMD_ATD8             = 0x3844,   // Set/get D8 config (DIO8/nDTR/SLP_RQ)
    AT_CMD_ATD9             = 0x3944,   // Set/get D9 config (DIO9/ON_nSLEEP)
    AT_CMD_ATLT             = 0x544c,   // Set/get Associate LED blink time
    AT_CMD_ATPR             = 0x5250,   // Set/get pull-up/-down resistor enable bitfield
    AT_CMD_ATPD             = 0x4450,   // Set/get pull-up/-down resistor direction bitfield
    AT_CMD_ATRP             = 0x5052,   // Set/get PWM timer expiry in units of 0.1s
    AT_CMD_ATDC             = 0x4344,   // Set miscellaneous behaviours
    AT_CMD_ATDO             = 0x4f44,   // Set/get device options
    AT_CMD_AT_PERCENT_V     = 0x5625,   // Get voltage on Vcc pin in mV
    AT_CMD_AT_V_PLUS        = 0x2b56,   // Set Vcc supply monitoring threshold
    AT_CMD_ATTP             = 0x5054,   // Get module temperature in deg C

    // Diagnostic commands
    AT_CMD_ATVR             = 0x5256,   // Get firmware version
    AT_CMD_ATVL             = 0x4c56,   // Get detailed version information
    AT_CMD_ATHV             = 0x5648,   // Get hardware version number
    AT_CMD_ATAI             = 0x4941,   // Get information relating to last node join request

    // Command-mode option commands
    AT_CMD_ATCT             = 0x5443,   // Set/get command-mode timeout in units of 0.1s
    AT_CMD_ATCN             = 0x4e43,   // Exit command mode and apply pending changes
    AT_CMD_ATGT             = 0x5447,   // Set command-sequence pre-/post-silence in milliseconds
    AT_CMD_ATCC             = 0x4343,   // Set/get command-mode entry character

    // Sleep commands
    AT_CMD_ATSM             = 0x4d53,   // Set/get sleep mode
    AT_CMD_ATSN             = 0x4e53,   // Set/get # of sleep periods between ON/nSLEEP assertions
    AT_CMD_ATSP             = 0x5053,   // Set sleep duration (10ms units, 0.25s resolution)
    AT_CMD_ATST             = 0x5453,   // Set/get time-before-sleep in milliseconds
    AT_CMD_ATSO             = 0x4f53,   // Set/get sleep options
    AT_CMD_ATWH             = 0x4857,   // Set/get wake-host timer value in milliseconds
    AT_CMD_ATPO             = 0x4f50,   // Set/get end-device poll rate (10ms units)

    // Execution commands
    AT_CMD_ATAC             = 0x4341,   // Apply changes
    AT_CMD_ATAS             = 0x5341,   // Scan neighbourhood for beacon responses
    AT_CMD_ATWR             = 0x5257,   // Write parameter values to non-volatile memory
    AT_CMD_ATRE             = 0x4552,   // Reset all parameters (except ZS and KY) to default
    AT_CMD_ATFR             = 0x5246,   // Perform software reset
    AT_CMD_ATNR             = 0x524e,   // Perform network reset
    AT_CMD_ATSI             = 0x4953,   // Start cyclic sleep immediately
    AT_CMD_ATCB             = 0x4243,   // Simulate commissioning-button press
    AT_CMD_AT_AMPERSAND_X   = 0x5826,   // Clear binding and group tables
    AT_CMD_ATND             = 0x444e,   // Node discovery report
    AT_CMD_ATDN             = 0x4e44,   // Resolve node identifier (NI) string to physical address
    AT_CMD_ATDJ             = 0x4a44,   // Disable joining of local device to a network
    AT_CMD_ATIS             = 0x5349    // Force read of enabled digital and analogue inputs
} AT_CMD_t;


#endif
