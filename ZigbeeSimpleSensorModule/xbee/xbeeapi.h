#ifndef XBEEAPI_H_INC
#define XBEEAPI_H_INC
/*
    xbeeapi.h - declarations relating to the XBee API interface

    Stuart Wallace <stuartw@atom.net>, August 2018.
*/


#define XBEE_FRAME_DELIMITER    (0x7e)  // Start-of-frame delimiter
#define XBEE_BUF_LEN            (64)    // Length of transmit and receive buffers, in bytes

typedef uint8_t                 XBeeFrameID_t;


//
// Type definitions relating to XBee API frames
//

// XBeeFrameType_t - enumeration of type identifiers for transmitted and received frames
//
typedef enum XBeeFrameType
{
    XBeeFrameNone                           = 0x00,     // Placeholder
    XBeeFrameATCommand                      = 0x08,
    XBeeFrameATCommandQueueParamVal         = 0x09,
    XBeeFrameZigbeeTXRequest                = 0x10,
    XBeeFrameExplicitAddrZigbeeCmd          = 0x11,
    XBeeFrameRemoteCommandRequest           = 0x17,
    XBeeFrameCreateSourceRoute              = 0x21,
    XBeeFrameATCommandResponse              = 0x88,
    XBeeFrameModemStatus                    = 0x8a,
    XBeeFrameZigbeeTransmitStatus           = 0x8b,
    XBeeFrameZigbeeReceivePacket            = 0x90,
    XBeeFrameZigbeeExplicitRXIndicator      = 0x91,
    XBeeFrameXBeeSensorReadIndicator        = 0x94,
    XBeeFrameNodeIDIndicator                = 0x95,
    XBeeFrameRemoteCommandResponse          = 0x97,
    XBeeFrameExtendedModemStatus            = 0x98,
    XBeeFrameOTAFirmwareUpdateStatus        = 0xa0,
    XBeeFrameRouteRecordIndicator           = 0xa1,
    XBeeFrameManyToOneRouteRequestIndicator = 0xa3
} XBeeFrameType_t;


// XBeeDeviceType_t - enumeration of XBee device types
//
typedef enum XBeeDeviceType
{
    XBeeDevTypeCoordinator  = 0,        // Signifies an XBee co-ordinator device
    XBeeDevTypeRouter       = 1,        // Signifies an XBee router device
    XBeeDevTypeEndDevice    = 2         // Signifies an XBee end device
} XBeeDeviceType_t;


// XBeeATCmdStatus_t - enumeration of the values of the <status> field in an AT command response
// frame
//
typedef enum XBeeATCmdStatus
{
    XBeeATCmdOK             = 0,        // AT command outcome: success
    XBeeATCmdError          = 1,        // AT command outcome: error
    XBeeATCmdInvalidCmd     = 2,        // AT command outcome: invalid command
    XBeeATCmdInvalidParam   = 3,        // AT command outcome: invalid parameter/argument
    XBeeATCmdTXFail         = 4         // AT command outcome: command transmit failed
} XBeeATCmdStatus_t;


// XBeeModemStatus_t - enumeration of the values of the <status> field in a modem status frame
//
typedef enum XBeeModemStatus
{
    XBeeModemStatusHardwareReset            = 0x00,
    XBeeModemStatusWatchdogReset            = 0x01,
    XBeeModemStatusJoinedNetwork            = 0x02,
    XBeeModemStatusDisassociated            = 0x03,
    XBeeModemStatusCoordinatorStarted       = 0x06,
    XBeeModemStatusNetSecurityKeyUpdated    = 0x07,
    XBeeModemStatusVSupplyLimitExceeded     = 0x0d,
    XBeeModemStatusConfigChangeDuringJoin   = 0x11
} XBeeModemStatus_t;


// XBeeModemStatusExt_t - enumeration of the values of the <status> field in an extended modem
// status frame
//
typedef enum XBeeModemStatusExt
{
    XBeeModemStatusExtRejoin                = 0x00,
    XBeeModemStatusExtStackStatus           = 0x01,
    XBeeModemStatusExtJoining               = 0x02,
    XBeeModemStatusExtJoined                = 0x03,
    XBeeModemStatusExtBeaconResponse        = 0x04,
    XBeeModemStatusExtRejectZS              = 0x05,
    XBeeModemStatusExtRejectID              = 0x06,
    XBeeModemStatusExtRejectNJ              = 0x07,
    XBeeModemStatusExtPANIDMatch            = 0x08,
    XBeeModemStatusExtRejectLQIRSSI         = 0x09,
    XBeeModemStatusExtBeaconSaved           = 0x0a,
    XBeeModemStatusExtAI                    = 0x0b,
    XBeeModemStatusExtPermitJoin            = 0x0c,
    XBeeModemStatusExtScanning              = 0x0d,
    XBeeModemStatusExtScanError             = 0x0e,
    XBeeModemStatusExtJoinRequest           = 0x0f,
    XBeeModemStatusExtRejectLQI             = 0x10,
    XBeeModemStatusExtRejectRSSI            = 0x11,
    XBeeModemStatusExtRejectedCmdLast       = 0x12,
    XBeeModemStatusExtRejectedCmdSave       = 0x13,
    XBeeModemStatusExtRejectStrength        = 0x14,
    XBeeModemStatusExtResetForDC80          = 0x16,
    XBeeModemStatusExtScanChannel           = 0x18,
    XBeeModemStatusExtScanMode              = 0x19,
    XBeeModemStatusExtScanInit              = 0x1a,
    XBeeModemStatusExtEnergyScanChanMask    = 0x1d,
    XBeeModemStatusExtEnergyScanEnergies    = 0x1e,
    XBeeModemStatusExtPANIDScanChannel      = 0x1f,
    XBeeModemStatusExtFormNetwork           = 0x20,
    XBeeModemStatusExtDiscoverKEEndpoint    = 0x21,
    XBeeModemStatusExtKEEndpoint            = 0x22
} XBeeModemStatusExt_t;


// Macro to determine whether the value returned in the <status> field of a modem status frame
// represents an error.  All values >=0x80 represent errors in the Ember Zigbee stack.
#define XBEE_MODEM_STATUS_IS_ERROR(status)  ((uint8_t) status & 0x80)


// XBeeTXDeliveryStatus_t - enumeration of the values of the <status> field in a transmit status
// frame
//
typedef enum XBeeTXDeliveryStatus
{
    XBeeTXDelStatusSuccess                      = 0x00,
    XBeeTXDelStatusMACAckFailure                = 0x01,
    XBeeTXDelStatusCCAFailure                   = 0x02,
    XBeeTXDelStatusInvalidDestEndpoint          = 0x15,
    XBeeTXDelStatusNetworkAckFailure            = 0x21,
    XBeeTXDelStatusNotJoinedToNetwork           = 0x22,
    XBeeTXDelStatusSelfAddressed                = 0x23,
    XBeeTXDelStatusAddressNotFound              = 0x24,
    XBeeTXDelStatusRouteNotFound                = 0x25,
    XBeeTXDelStatusBroadcastRelayNotHeard       = 0x26,
    XBeeTXDelStatusInvalidBindingTableIndex     = 0x2b,
    XBeeTXDelStatusResourceError1               = 0x2c,
    XBeeTXDelStatusAttemptedBroadcastWithAPS    = 0x2d,
    XBeeTXDelStatusAttemptedUnicastWithAPS      = 0x2e,
    XBeeTXDelStatusResourceError2               = 0x32,
    XBeeTXDelStatusPayloadTooLarge              = 0x74,
    XBeeTXDelStatusIndirectMessageUnrequested   = 0x75
} XBeeTXDeliveryStatus_t;


// XBeeTXDiscoveryStatus_t - enumeration of the values of the <discovery_status> field in a
// transmit status frame
//
typedef enum XBeeTXDiscoveryStatus
{
    XBeeTXDiscStatusNoDiscoveryOverhead         = 0x00,
    XBeeTXDiscStatusAddressDiscovery            = 0x01,
    XBeeTXDiscStatusRouteDiscovery              = 0x02,
    XBeeTXDiscStatusAddressAndRoute             = 0x03,
    XBeeTXDiscStatusExtendedTimeoutDiscovery    = 0x40
} XBeeTXDiscoveryStatus_t;


// NodeIDSrcEvent_t - enumeration of values of the <source_event> field in a node identification
// indicator frame
//
typedef enum XBeeNodeIDSrcEvent
{
    XBeeNodeIDEventPushbutton   = 1,
    XBeeNodeIDEventJoining      = 2,
    XBeeNodeIDEventPowerCycle   = 3
} XBeeNodeIDSrcEvent_t;


// XBeeBootloaderMsgType_t - enumeration of values of the <message_type> field in an OTA firmware
// update status frame
//
typedef enum XBeeBootloaderMsgType
{
    XBeeBootloaderMsgAck            = 0x06,
    XBeeBootloaderMsgNAck           = 0x15,
    XBeeBootloaderMsgNoMACAck       = 0x40,
    XBeeBootloaderMsgQuery          = 0x51,
    XBeeBootloaderMsgQueryResponse  = 0x52
} BootloaderMsgType_t;


// XBeeArgSleepMode_t - enumeration of the values of the argument to the "sleep mode" (SM) command
//
typedef enum XBeeArgSleepMode
{
    XBeeSleepModeDisabled           = 0,
    XBeeSleepModePinSleep           = 1,
    XBeeSleepModeCyclicSleep        = 2,
    XBeeSleepModeCyclicSleepPinWake = 3
} XBeeArgSleepMode_t;


// XBeeArgPinCfg_t - enumeration of the values of the argument to the various pin-configuration
// (ATDn/ATPn) commands
//
typedef enum XBeeArgPinCfg
{
    XBeePinCfgUnmonitoredInput          = 0,
    XBeePinCfgAlternateFunction         = 1,
    XBeePinCfgAnalogueInput             = 2,
    XBeePinCfgDigitalInput              = 3,
    XBeePinCfgDigitalOutputDefaultLow   = 4,
    XBeePinCfgDigitalOutputDefaultHigh  = 5
} XBeeArgPinCfg_t;


// The main packet-buffer struct.  This struct represents a buffer for a transmitted or received
// packet.  It contains a union of structs, one per packet type, to simplify access to packet data.
// The struct resembles a generic API frame, excluding the start delimiter and checksum bytes.
// Note that the number of bytes specified in the "len" field should exclude the length of the
// <frame_type> field - i.e. it must equal the length of the "frame-specific data" field.  E.g. for
// a frame of type XBeeFrameATcommand, sending the AT command "VR" with no arguments, len must
// equal 3: one byte for the frame ID, plus two bytes for the command name.
typedef struct __attribute__((packed)) XBeePacketBuf
{
    uint16_t            len;                // Always in device (little-endian) order
    XBeeFrameType_t     frame_type;

    union
    {
        // AT command frame (0x08)
        struct __attribute__((packed))
        {
            XBeeFrameID_t           frame_id;
            XBeeATCmd_t             cmd;
            char                    parameter_value[XBEE_BUF_LEN - 3];
        } at;

        // AT command queue parameter value frame (0x09)
        struct __attribute__((packed))
        {
            XBeeFrameID_t           frame_id;
            XBeeATCmd_t             cmd;
            char                    parameter_value[XBEE_BUF_LEN - 3];
        } atqpv;

        // Transmit-request frame (0x10)
        struct __attribute__((packed))
        {
            XBeeFrameID_t           frame_id;
            uint64_t                dest_addr;              // NOTE: MSB-first ("network order")
            uint16_t                dest_net_addr;          // NOTE: MSB-first ("network order")
            uint8_t                 broadcast_radius;
            uint8_t                 transmission_options;
            char                    data[XBEE_BUF_LEN - 13];
        } txrq;

        // Explicit-addressing command frame (0x11)
        struct __attribute__((packed))
        {
            XBeeFrameID_t           frame_id;
            uint64_t                dest_addr;          // NOTE: MSB-first ("network order")
            uint16_t                dest_net_addr;      // NOTE: MSB-first ("network order")
            uint8_t                 src_endpoint;
            uint8_t                 dest_endpoint;
            uint16_t                cluster_id;
            uint16_t                profile_id;
            uint8_t                 broadcast_radius;
            uint8_t                 options;
            char                    data[XBEE_BUF_LEN - 19];
        } eacf;

        // Remote AT command request frame (0x17)
        struct __attribute__((packed))
        {
            XBeeFrameID_t           frame_id;
            uint64_t                dest_addr;          // NOTE: MSB-first ("network order")
            uint16_t                dest_net_addr;      // NOTE: MSB-first ("network order")
            uint8_t                 options;
            XBeeATCmd_t             cmd;
            char                    parameter_value[XBEE_BUF_LEN - 14];
        } ratrq;

        // Create source route frame (0x21)
        struct __attribute__((packed))
        {
            XBeeFrameID_t           frame_id;           // Must be zero
            uint64_t                dest_addr;          // NOTE: MSB-first ("network order")
            uint16_t                dest_net_addr;      // NOTE: MSB-first ("network order")
            uint8_t                 route_cmd_options;  // Must be 0
            uint8_t                 num_addresses;
            uint16_t                address[(XBEE_BUF_LEN - 13) / sizeof(uint16_t)];
        } csr;

        // AT command response frame (0x88)
        struct __attribute__((packed))
        {
            XBeeFrameID_t           frame_id;
            XBeeATCmd_t             cmd;
            XBeeATCmdStatus_t       status;
            char                    data[XBEE_BUF_LEN - 4];
        } at_resp;

        // Modem status frame (0x8a)
        struct __attribute__((packed))
        {
            XBeeModemStatus_t       status;
        } ms;

        // Transmit status frame (0x8b)
        struct __attribute__((packed))
        {
            XBeeFrameID_t           frame_id;
            uint16_t                dest_net_addr;      // NOTE: MSB-first ("network order")
            uint8_t                 retry_count;
            XBeeTXDeliveryStatus_t  status;
            XBeeTXDiscoveryStatus_t discovery_status;
        } txs;

        // Receive packet frame (0x90)
        struct __attribute__((packed))
        {
            uint64_t                src_addr;           // NOTE: MSB-first ("network order")
            uint16_t                src_net_addr;       // NOTE: MSB-first ("network order")
            uint8_t                 options;
            char                    data[XBEE_BUF_LEN - 11];
        } rxp;

        // Explicit receive indicator frame (0x91)
        struct __attribute__((packed))
        {
            uint64_t                src_addr;           // NOTE: MSB-first ("network order")
            uint16_t                src_net_addr;       // NOTE: MSB-first ("network order")
            uint8_t                 src_endpoint;
            uint8_t                 dest_endpoint;
            uint16_t                cluster_id;
            uint16_t                profile_id;
            uint8_t                 options;
            char                    data[XBEE_BUF_LEN - 17];
        } erxi;

        // Data sample receive indicator frame (0x92)
        struct __attribute__((packed))
        {
            uint64_t                src_addr;           // NOTE: MSB-first ("network order")
            uint16_t                src_net_addr;       // NOTE: MSB-first ("network order")
            uint8_t                 options;
            uint8_t                 num_samples;
            uint16_t                digital_mask;
            uint16_t                analogue_mask;
            uint16_t                digital_samples;
            uint16_t                analogue_sample;
        } dsrxi;

        // XBee sensor read indicator (0x94)
        struct __attribute__((packed))
        {
            uint64_t                src_addr;           // NOTE: MSB-first ("network order")
            uint16_t                src_net_addr;       // NOTE: MSB-first ("network order")
            uint8_t                 options;
            uint8_t                 onewire_sensors;
            uint16_t                adc_values[4];
            uint16_t                temperature_read;
        } xbsri;

        // Node identification indicator frame (0x95)
        struct __attribute__((packed))
        {
            uint64_t                src_addr;           // NOTE: MSB-first ("network order")
            uint16_t                src_net_addr;       // NOTE: MSB-first ("network order")
            uint8_t                 options;
            uint16_t                remote_net_addr;    // NOTE: MSB-first ("network order")
            uint64_t                remote_addr;        // NOTE: MSB-first ("network order")
            uint16_t                ni_string;
            XBeeDeviceType_t        device_type;
            XBeeNodeIDSrcEvent_t    source_event;
            uint16_t                digi_profile_id;
            uint16_t                digi_manufacturer_id;
        } nii;

        // Remote command response frame (0x97)
        struct __attribute__((packed))
        {
            XBeeFrameID_t           frame_id;
            uint64_t                src_addr;           // NOTE: MSB-first ("network order")
            uint16_t                src_net_addr;       // NOTE: MSB-first ("network order")
            XBeeATCmd_t             cmd;
            XBeeATCmdStatus_t       status;
            char                    data[XBEE_BUF_LEN - 14];
        } rcr;

        // Extended modem status frame (0x98)
        struct __attribute__((packed))
        {
            XBeeModemStatusExt_t    status;
            char                    data[XBEE_BUF_LEN - 1];
        } ems;

        // Over-the-air (OTA) firmware update status frame
        struct __attribute__((packed))
        {
            uint64_t                src_addr;           // NOTE: MSB-first ("network order")
            uint16_t                dest_net_addr;      // NOTE: MSB-first ("network order")
            uint8_t                 options;
            BootloaderMsgType_t     message_type;
            uint8_t                 block_num;
            uint64_t                target_addr;
        } otaus;

        // Route record indicator frame
        struct __attribute__((packed))
        {
            uint64_t                src_addr;           // NOTE: MSB-first ("network order")
            uint16_t                src_net_addr;       // NOTE: MSB-first ("network order")
            uint8_t                 options;
            uint8_t                 num_addreses;
            uint16_t                address[(XBEE_BUF_LEN - 12) / sizeof(uint16_t)];
        } rri;

        // Many-to-one route request indicator
        struct __attribute__((packed))
        {
            uint64_t                src_addr;           // NOTE: MSB-first ("network order")
            uint16_t                src_net_addr;       // NOTE: MSB-first ("network order")
            uint8_t                 reserved;           // Always 0
        } mrri;

        // Raw buffer
        char raw[XBEE_BUF_LEN];
    };
} XBeePacketBuf_t;

#endif
