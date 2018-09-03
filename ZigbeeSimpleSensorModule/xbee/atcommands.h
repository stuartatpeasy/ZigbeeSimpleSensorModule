#ifndef ATCOMMANDS_H_INC
#define ATCOMMANDS_H_INC
/*
    atcommands.h:   declarations relating to the XBee AT command-set

    Stuart Wallace <stuartw@atom.net>, July 2018.
*/


// XBeeATCmd_t - enumeration containing a list of AT commands supported by the XBee modules
//
// The value of each enumerated constant is equal to the command string in little-endian byte
// order.  E.g. XBeeATCmdATVR has value 0x5256 ('RV'); when written to memory as a little-endian
// uint16_t, this will be stored as the characters {'V', 'R'}.
//
typedef enum XBeeATCmd
{
    // Addressing commands
    XBeeATCmdATDH           = 0x4844,   // Set/get upper 32 bits of the 64-bit destination addr
    XBeeATCmdATDL           = 0x4c44,   // Set/get lower 32 bits of the 64-bit destination addr
    XBeeATCmdATMY           = 0x594d,   // Get the 16-bit network address of the device
    XBeeATCmdATMP           = 0x504d,   // Get the 16-bit network address of the parent device
    XBeeATCmdATNC           = 0x434e,   // Get remaining # of end-devices that can join this device
    XBeeATCmdATSH           = 0x4853,   // Get upper 32 bits of 64-bit IEEE unique source address
    XBeeATCmdATSL           = 0x4c53,   // Get lower 32 bits of 64-bit IEEE unique source address
    XBeeATCmdATNI           = 0x494e,   // Store node identifier string (20 char max)
    XBeeATCmdATSE           = 0x4553,   // Set/get application layer source endpoint value
    XBeeATCmdATDE           = 0x4544,   // Set/get application layer destination endpoint value
    XBeeATCmdATCI           = 0x4943,   // Set/get application layer ID (cluster ID)
    XBeeATCmdATTO           = 0x4f54,   // Set/get application layer transmit options
    XBeeATCmdATNP           = 0x504e,   // Get maximum packet payload in bytes
    XBeeATCmdATDD           = 0x4444,   // Set Digi device type identifier value
    XBeeATCmdATCR           = 0x5243,   // Set # of PAN ID conflicts/min to trigger a PAN ID change

    // Network commands
    XBeeATCmdATCH           = 0x4843,   // Read the operating channel (IEEE 802.15.4 channel #)
    XBeeATCmdATCE           = 0x4543,   // Set/get co-ordinator status for this device
    XBeeATCmdATID           = 0x4449,   // Set/get extended (64-bit) PAN ID
    XBeeATCmdATII           = 0x4949,   // Get pre-configured 16-bit PAN ID for network-forming
    XBeeATCmdATOP           = 0x504f,   // Get 64-bit "operating" (=in-use) extended PAN ID
    XBeeATCmdATNH           = 0x484e,   // Set/get maximum network hops
    XBeeATCmdATBH           = 0x4842,   // Set/get maximum broadcast hops
    XBeeATCmdATOI           = 0x494f,   // Read 16-bit "operating" (=in-use) PAN ID
    XBeeATCmdATNT           = 0x544e,   // Set/get node discovery timeout (units of 100ms)
    XBeeATCmdATNO           = 0x4f4e,   // Set/get network discovery options
    XBeeATCmdATSC           = 0x4353,   // Set/get the list of channels to scan (as a bitfield)
    XBeeATCmdATSD           = 0x4453,   // Set/get scan-duration exponent
    XBeeATCmdATED           = 0x4445,   // Start an energy-detect scan
    XBeeATCmdATZS           = 0x535a,   // Set/get Zigbee stack profile value
    XBeeATCmdATNJ           = 0x4a4e,   // Set/get co-ordinator node join time
    XBeeATCmdATJV           = 0x564a,   // Set/get channel verification parameter
    XBeeATCmdATNW           = 0x574e,   // Set/get network watchdog timeout
    XBeeATCmdATJN           = 0x4e4a,   // Set/get join notification setting
    XBeeATCmdATAR           = 0x5241,   // Set/get aggregate routing notification interval

    // Security commands
    XBeeATCmdATEE           = 0x4545,   // Set/get encryption enable setting
    XBeeATCmdATEO           = 0x4f45,   // Set encryption options
    XBeeATCmdATNK           = 0x4b4e,   // Set AES network-encryption key
    XBeeATCmdATKY           = 0x594b,   // Set AES link key

    // RF interfacing commands
    XBeeATCmdATPL           = 0x4c50,   // Set/get transmit power level
    XBeeATCmdATPM           = 0x4d50,   // Set/get power mode
    XBeeATCmdATDB           = 0x4244,   // Get received signal strength of last packet (in -dBm)
    XBeeATCmdATPP           = 0x5050,   // Get output power in dBm at maximum transmit power (PL4)

    // Serial interfacing commands
    XBeeATCmdATAO           = 0x4f41,   // Set API options
    XBeeATCmdATBD           = 0x4442,   // Set interface data (baud) rate
    XBeeATCmdATNB           = 0x424e,   // Set/get serial parity mode
    XBeeATCmdATSB           = 0x4253,   // Set/get serial stop bits
    XBeeATCmdATRO           = 0x4f52,   // Set/get inter-character silence length to start TX
    XBeeATCmdATD7           = 0x3744,   // Set/get pin DIO7 config (DIO7/nCTS)
    XBeeATCmdATD6           = 0x3644,   // Set/get pin DIO6 config (DIO6/nRTS)

    // I/O settings commands
    XBeeATCmdATIR           = 0x5249,   // Set/get I/O sample rate for periodic sampling
    XBeeATCmdATIC           = 0x4349,   // Set/get bitfield of DIOx pins to watch for state changes
    XBeeATCmdATP0           = 0x3050,   // Set/get P0 config (RSSI/PWM0)
    XBeeATCmdATP1           = 0x3150,   // Set/get P1 config (DIO11/PWM1)
    XBeeATCmdATP2           = 0x3250,   // Set/get P2 config (DIO12)
    XBeeATCmdATP3           = 0x3350,   // Set/get P3 config (DIO13/DOUT)
    XBeeATCmdATP4           = 0x3450,   // Set/get P4 config (DIO14/DIN)
    XBeeATCmdATP5           = 0x3550,   // Set/get P5 config (DIO15/SPI_MISO)
    XBeeATCmdATP6           = 0x3650,   // Set/get P6 config (SPI_MOSI)
    XBeeATCmdATP7           = 0x3750,   // Set/get P7 config (DIO17/SPI_nSSEL)
    XBeeATCmdATP8           = 0x3850,   // Set/get P8 config (DIO18/SPI_SCLK)
    XBeeATCmdATP9           = 0x3950,   // Set/get P9 config (DIO19/SPI_nATTN/PTI_DATA)
    XBeeATCmdATD0           = 0x3044,   // Set/get D0 config (AD0/DIO0)
    XBeeATCmdATD1           = 0x3144,   // Set/get D1 config (AD1/DIO1/PTI_EN)
    XBeeATCmdATD2           = 0x3244,   // Set/get D2 config (AD2/DIO2)
    XBeeATCmdATD3           = 0x3344,   // Set/get D3 config (AD3/DIO3)
    XBeeATCmdATD4           = 0x3444,   // Set/get D4 config (DIO4)
    XBeeATCmdATD5           = 0x3544,   // Set/get D5 config (DIO5/ASSOCIATE)
    XBeeATCmdATD8           = 0x3844,   // Set/get D8 config (DIO8/nDTR/SLP_RQ)
    XBeeATCmdATD9           = 0x3944,   // Set/get D9 config (DIO9/ON_nSLEEP)
    XBeeATCmdATLT           = 0x544c,   // Set/get Associate LED blink time
    XBeeATCmdATPR           = 0x5250,   // Set/get pull-up/-down resistor enable bitfield
    XBeeATCmdATPD           = 0x4450,   // Set/get pull-up/-down resistor direction bitfield
    XBeeATCmdATRP           = 0x5052,   // Set/get PWM timer expiry in units of 0.1s
    XBeeATCmdATDC           = 0x4344,   // Set miscellaneous behaviours
    XBeeATCmdATDO           = 0x4f44,   // Set/get device options
    XBeeATCmdATPercentV     = 0x5625,   // Get voltage on Vcc pin in mV
    XBeeATCmdATVPlus        = 0x2b56,   // Set Vcc supply monitoring threshold
    XBeeATCmdATTP           = 0x5054,   // Get module temperature in deg C

    // Diagnostic commands
    XBeeATCmdATVR           = 0x5256,   // Get firmware version
    XBeeATCmdATVL           = 0x4c56,   // Get detailed version information
    XBeeATCmdATHV           = 0x5648,   // Get hardware version number
    XBeeATCmdATAI           = 0x4941,   // Get information relating to last node join request

    // Command-mode option commands
    XBeeATCmdATCT           = 0x5443,   // Set/get command-mode timeout in units of 0.1s
    XBeeATCmdATCN           = 0x4e43,   // Exit command mode and apply pending changes
    XBeeATCmdATGT           = 0x5447,   // Set command-sequence pre-/post-silence in milliseconds
    XBeeATCmdATCC           = 0x4343,   // Set/get command-mode entry character

    // Sleep commands
    XBeeATCmdATSM           = 0x4d53,   // Set/get sleep mode
    XBeeATCmdATSN           = 0x4e53,   // Set/get # of sleep periods between ON/nSLEEP assertions
    XBeeATCmdATSP           = 0x5053,   // Set sleep duration (10ms units, 0.25s resolution)
    XBeeATCmdATST           = 0x5453,   // Set/get time-before-sleep in milliseconds
    XBeeATCmdATSO           = 0x4f53,   // Set/get sleep options
    XBeeATCmdATWH           = 0x4857,   // Set/get wake-host timer value in milliseconds
    XBeeATCmdATPO           = 0x4f50,   // Set/get end-device poll rate (10ms units)

    // Execution commands
    XBeeATCmdATAC           = 0x4341,   // Apply changes
    XBeeATCmdATAS           = 0x5341,   // Scan neighbourhood for beacon responses
    XBeeATCmdATWR           = 0x5257,   // Write parameter values to non-volatile memory
    XBeeATCmdATRE           = 0x4552,   // Reset all parameters (except ZS and KY) to default
    XBeeATCmdATFR           = 0x5246,   // Perform software reset
    XBeeATCmdATNR           = 0x524e,   // Perform network reset
    XBeeATCmdATSI           = 0x4953,   // Start cyclic sleep immediately
    XBeeATCmdATCB           = 0x4243,   // Simulate commissioning-button press
    XBeeATCmdATAmpersandX   = 0x5826,   // Clear binding and group tables
    XBeeATCmdATND           = 0x444e,   // Node discovery report
    XBeeATCmdATDN           = 0x4e44,   // Resolve node identifier (NI) string to physical address
    XBeeATCmdATDJ           = 0x4a44,   // Disable joining of local device to a network
    XBeeATCmdATIS           = 0x5349    // Force read of enabled digital and analogue inputs
} XBeeATCmd_t;

#endif
