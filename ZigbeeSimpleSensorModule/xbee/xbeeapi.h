#ifndef XBEEAPI_H_INC
#define XBEEAPI_H_INC
/*
    xbeeapi.h - declarations relating to the XBee API interface

    Stuart Wallace <stuartw@atom.net>, August 2018.
*/


#define XBEE_FRAME_DELIMITER    (0x7e)  // Start-of-frame delimiter


//
// Type definitions relating to XBee API frames
//

// Enumeration of type identifiers for transmitted and received frames
typedef enum XBEE_FRAME_TYPE
{
    XBEE_FRAME_NONE                                 = 0x00,     // Placeholder
    XBEE_FRAME_AT_COMMAND                           = 0x08,
    XBEE_FRAME_AT_COMMAND_QUEUE_PARAM_VAL           = 0x09,
    XBEE_FRAME_ZIGBEE_TX_REQUEST                    = 0x10,
    XBEE_FRAME_EXPLICIT_ADDR_ZIGBEE_CMD             = 0x11,
    XBEE_FRAME_REMOTE_COMMAND_REQUEST               = 0x17,
    XBEE_FRAME_CREATE_SOURCE_ROUTE                  = 0x21,
    XBEE_FRAME_AT_COMMAND_RESPONSE                  = 0x88,
    XBEE_FRAME_MODEM_STATUS                         = 0x8A,
    XBEE_FRAME_ZIGBEE_TRANSMIT_STATUS               = 0x8B,
    XBEE_FRAME_ZIGBEE_RECEIVE_PACKET                = 0x90,
    XBEE_FRAME_ZIGBEE_EXPLICIT_RX_INDICATOR         = 0x91,
    XBEE_FRAME_XBEE_SENSOR_READ_INDICATOR           = 0x94,
    XBEE_FRAME_NODE_ID_INDICATOR                    = 0x95,
    XBEE_FRAME_REMOTE_COMMAND_RESPONSE              = 0x97,
    XBEE_FRAME_EXTENDED_MODEM_STATUS                = 0x98,
    XBEE_FRAME_OTA_FIRMWARE_UPDATE_STATUS           = 0xA0,
    XBEE_FRAME_ROUTE_RECORD_INDICATOR               = 0xA1,
    XBEE_FRAME_MANY_TO_ONE_ROUTE_REQUEST_INDICATOR  = 0xA3
} XBEE_FRAME_TYPE_t;


// Transmit and receive buffers
#define XBEE_BUF_LEN                    (64)


// XBee device types
typedef enum XBEE_DEVICE_TYPE
{
    XBEE_DEV_TYPE_COORDINATOR   = 0,
    XBEE_DEV_TYPE_ROUTER        = 1,
    XBEE_DEV_TYPE_END_DEVICE    = 2
} XBEE_DEVICE_TYPE_t;


// Enumeration of the values of the <status> field in an AT command response frame
typedef enum AT_CMD_STATUS
{
    AT_CMD_OK                   = 0,
    AT_CMD_ERROR                = 1,
    AT_CMD_INVALID_CMD          = 2,
    AT_CMD_INVALID_PARAM        = 3,
    AR_CMD_TX_FAIL              = 4
} AT_CMD_STATUS_t;


// Enumeration of the values of the <status> field in a modem status frame
typedef enum MODEM_STATUS
{
    MODEM_STATUS_HARDWARE_RESET             = 0x00,
    MODEM_STATUS_WATCHDOG_RESET             = 0x01,
    MODEM_STATUS_JOINED_NETWORK             = 0x02,
    MODEM_STATUS_DISASSOCIATED              = 0x03,
    MODEM_STATUS_COORDINATOR_STARTED        = 0x06,
    MODEM_STATUS_NET_SECURITY_KEY_UPDATED   = 0x07,
    MODEM_STATUS_VSUPPLY_LIMIT_EXCEEDED     = 0x0d,
    MODEM_STATUS_CONFIG_CHANGE_DURING_JOIN  = 0x11
} MODEM_STATUS_t;


// Enumeration of the values of the <status> field in an extended modem status frame
typedef enum MODEM_STATUS_EXT
{
    MODEM_STATUS_EXT_REJOIN                 = 0x00,
    MODEM_STATUS_EXT_STACK_STATUS           = 0x01,
    MODEM_STATUS_EXT_JOINING                = 0x02,
    MODEM_STATUS_EXT_JOINED                 = 0x03,
    MODEM_STATUS_EXT_BEACON_RESPONSE        = 0x04,
    MODEM_STATUS_EXT_REJECT_ZS              = 0x05,
    MODEM_STATUS_EXT_REJECT_ID              = 0x06,
    MODEM_STATUS_EXT_REJECT_NJ              = 0x07,
    MODEM_STATUS_EXT_PAN_ID_MATCH           = 0x08,
    MODEM_STATUS_EXT_REJECT_LQIRSSI         = 0x09,
    MODEM_STATUS_EXT_BEACON_SAVED           = 0x0a,
    MODEM_STATUS_EXT_AI                     = 0x0b,
    MODEM_STATUS_EXT_PERMIT_JOIN            = 0x0c,
    MODEM_STATUS_EXT_SCANNING               = 0x0d,
    MODEM_STATUS_EXT_SCAN_ERROR             = 0x0e,
    MODEM_STATUS_EXT_JOIN_REQUEST           = 0x0f,
    MODEM_STATUS_EXT_REJECT_LQI             = 0x10,
    MODEM_STATUS_EXT_REJECT_RSSI            = 0x11,
    MODEM_STATUS_EXT_REJECTED_CMDLAST       = 0x12,
    MODEM_STATUS_EXT_REJECTED_CMDSAVE       = 0x13,
    MODEM_STATUS_EXT_REJECT_STRENGTH        = 0x14,
    MODEM_STATUS_EXT_RESET_FOR_DC80         = 0x16,
    MODEM_STATUS_EXT_SCAN_CHANNEL           = 0x18,
    MODEM_STATUS_EXT_SCAN_MODE              = 0x19,
    MODEM_STATUS_EXT_SCAN_INIT              = 0x1a,
    MODEM_STATUS_EXT_ENERGY_SCAN_CHAN_MASK  = 0x1d,
    MODEM_STATUS_EXT_ENERGY_SCAN_ENERGIES   = 0x1e,
    MODEM_STATUS_EXT_PAN_ID_SCAN_CHANNEL    = 0x1f,
    MODEM_STATUS_EXT_FORM_NETWORK           = 0x20,
    MODEM_STATUS_EXT_DISCOVER_KE_ENDPOINT   = 0x21,
    MODEM_STATUS_EXT_KE_ENDPOINT            = 0x22
} MODEM_STATUS_EXT_t;


// Macro to determine whether the value returned in the <status> field of a modem status frame
// represents an error.  All values >=0x80 represent errors in the Ember Zigbee stack.
#define MODEM_STATUS_IS_ERROR(status)       ((uint8_t) status & 0x80)


// Enumeration of the values of the <status> field in a transmit status frame
typedef enum TX_DELIVERY_STATUS
{
    TX_DEL_STATUS_SUCCESS                       = 0x00,
    TX_DEL_STATUS_MAC_ACK_FAILURE               = 0x01,
    TX_DEL_STATUS_CCA_FAILURE                   = 0x02,
    TX_DEL_STATUS_INVALID_DEST_ENDPOINT         = 0x15,
    TX_DEL_STATUS_NETWORK_ACK_FAILURE           = 0x21,
    TX_DEL_STATUS_NOT_JOINED_TO_NETWORK         = 0x22,
    TX_DEL_STATUS_SELF_ADDRESSED                = 0x23,
    TX_DEL_STATUS_ADDRESS_NOT_FOUND             = 0x24,
    TX_DEL_STATUS_ROUTE_NOT_FOUND               = 0x25,
    TX_DEL_STATUS_BROADCAST_RELAY_NOT_HEARD     = 0x26,
    TX_DEL_STATUS_INVALID_BINDING_TABLE_INDEX   = 0x2b,
    TX_DEL_STATUS_RESOURCE_ERROR_1              = 0x2c,
    TX_DEL_STATUS_ATTEMPTED_BROADCAST_WITH_APS  = 0x2d,
    TX_DEL_STATUS_ATTEMPTED_UNICAST_WITH_APS    = 0x2e,
    TX_DEL_STATUS_RESOURCE_ERROR_2              = 0x32,
    TX_DEL_STATUS_PAYLOAD_TOO_LARGE             = 0x74,
    TX_DEL_STATUS_INDIRECT_MESSAGE_UNREQUESTED  = 0x75
} TX_DELIVERY_STATUS_t;


// Enumeration of the values of the <discovery_status> field in a transmit status frame
typedef enum TX_DISCOVERY_STATUS
{
    TX_DISC_STATUS_NO_DISCOVERY_OVERHEAD        = 0x00,
    TX_DISC_STATUS_ADDRESS_DISCOVERY            = 0x01,
    TX_DISC_STATUS_ROUTE_DISCOVERY              = 0x02,
    TX_DISC_STATUS_ADDRESS_AND_ROUTE            = 0x03,
    TX_DISC_STATUS_EXTENDED_TIMEOUT_DISCOVERY   = 0x40
} TX_DISCOVERY_STATUS_t;


// Enumeration of values of the <source_event> field in a node identification indicator frame
typedef enum NODE_ID_SRC_EVENT
{
    NODE_ID_EVENT_PUSHBUTTON    = 1,
    NODE_ID_EVENT_JOINING       = 2,
    NODE_ID_EVENT_POWER_CYCLE   = 3
} NODE_ID_SRC_EVENT_t;


// Enumeration of values of the <message_type> field in an OTA firmware update status frame
typedef enum BOOTLOADER_MSG_TYPE
{
    BOOTLOADER_MSG_ACK              = 0x06,
    BOOTLOADER_MSG_NACK             = 0x15,
    BOOTLOADER_MSG_NO_MAC_ACK       = 0x40,
    BOOTLOADER_MSG_QUERY            = 0x51,
    BOOTLOADER_MSG_QUERY_RESPONSE   = 0x52
} BOOTLOADER_MSG_TYPE_t;


// Enumeration of the values of the argument to the "sleep mode" (SM) command
typedef enum ARG_SLEEP_MODE
{
    SLEEP_MODE_DISABLED                 = 0,
    SLEEP_MODE_PIN_SLEEP                = 1,
    SLEEP_MODE_CYCLIC_SLEEP             = 2,
    SLEEP_MODE_CYCLIC_SLEEP_PIN_WAKE    = 3
} ARG_SLEEP_MODE_t;

// Enumeration of the values of the argument to the various pin-configuration (ATDn/ATPn) commands
typedef enum ARG_PIN_CFG
{
    PIN_CFG_UNMONITORED_INPUT           = 0,
    PIN_CFG_ALTERNATE_FUNCTION          = 1,
    PIN_CFG_ANALOGUE_INPUT              = 2,
    PIN_CFG_DIGITAL_INPUT               = 3,
    PIN_CFG_DIGITAL_OUTPUT_DEFAULT_LOW  = 4,
    PIN_CFG_DIGITAL_OUTPUT_DEFAULT_HIGH = 5
} ARG_PIN_CFG_t;


typedef uint8_t         xbee_frame_id_t;


// The main packet-buffer struct.  This struct represents a buffer for a transmitted or received
// packet.  It contains a union of structs, one per packet type, to simplify access to packet data.
// The struct resembles a generic API frame, excluding the start delimiter and checksum bytes.
// Note that the number of bytes specified in the "len" field should exclude the length of the
// <frame_type> field - i.e. it must equal the length of the "frame-specific data" field.  E.g. for
// a frame of type XBEE_FRAME_AT_COMMAND, sending the AT command "VR" with no arguments, len must
// equal 3: one byte for the frame ID, plus two bytes for the command name.
typedef struct __attribute__((packed)) XBEE_PACKET_BUF
{
    uint16_t            len;                // Always in device (little-endian) order
    XBEE_FRAME_TYPE_t   frame_type;

    union
    {
        // AT command frame (0x08)
        struct __attribute__((packed))
        {
            xbee_frame_id_t         frame_id;
            AT_CMD_t                cmd;
            char                    parameter_value[XBEE_BUF_LEN - 3];
        } at;

        // AT command queue parameter value frame (0x09)
        struct __attribute__((packed))
        {
            xbee_frame_id_t         frame_id;
            AT_CMD_t                cmd;
            char                    parameter_value[XBEE_BUF_LEN - 3];
        } atqpv;

        // Transmit-request frame (0x10)
        struct __attribute__((packed))
        {
            xbee_frame_id_t         frame_id;
            uint64_t                dest_addr;              // NOTE: MSB-first ("network order")
            uint16_t                dest_net_addr;          // NOTE: MSB-first ("network order")
            uint8_t                 broadcast_radius;
            uint8_t                 transmission_options;
            char                    data[XBEE_BUF_LEN - 13];
        } txrq;

        // Explicit-addressing command frame (0x11)
        struct __attribute__((packed))
        {
            xbee_frame_id_t         frame_id;
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
            xbee_frame_id_t         frame_id;
            uint64_t                dest_addr;          // NOTE: MSB-first ("network order")
            uint16_t                dest_net_addr;      // NOTE: MSB-first ("network order")
            uint8_t                 options;
            AT_CMD_t                cmd;
            char                    parameter_value[XBEE_BUF_LEN - 14];
        } ratrq;

        // Create source route frame (0x21)
        struct __attribute__((packed))
        {
            xbee_frame_id_t         frame_id;           // Must be zero
            uint64_t                dest_addr;          // NOTE: MSB-first ("network order")
            uint16_t                dest_net_addr;      // NOTE: MSB-first ("network order")
            uint8_t                 route_cmd_options;  // Must be 0
            uint8_t                 num_addresses;
            uint16_t                address[(XBEE_BUF_LEN - 13) / sizeof(uint16_t)];
        } csr;

        // AT command response frame (0x88)
        struct __attribute__((packed))
        {
            xbee_frame_id_t         frame_id;
            AT_CMD_t                cmd;
            AT_CMD_STATUS_t         status;
            char                    data[XBEE_BUF_LEN - 4];
        } at_resp;

        // Modem status frame (0x8a)
        struct __attribute__((packed))
        {
            MODEM_STATUS_t          status;
        } ms;

        // Transmit status frame (0x8b)
        struct __attribute__((packed))
        {
            xbee_frame_id_t         frame_id;
            uint16_t                dest_net_addr;      // NOTE: MSB-first ("network order")
            uint8_t                 retry_count;
            TX_DELIVERY_STATUS_t    status;
            TX_DISCOVERY_STATUS_t   discovery_status;
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
            XBEE_DEVICE_TYPE_t      device_type;
            NODE_ID_SRC_EVENT_t     source_event;
            uint16_t                digi_profile_id;
            uint16_t                digi_manufacturer_id;
        } nii;

        // Remote command response frame (0x97)
        struct __attribute__((packed))
        {
            xbee_frame_id_t         frame_id;
            uint64_t                src_addr;           // NOTE: MSB-first ("network order")
            uint16_t                src_net_addr;       // NOTE: MSB-first ("network order")
            AT_CMD_t                cmd;
            AT_CMD_STATUS_t         status;
            char                    data[XBEE_BUF_LEN - 14];
        } rcr;

        // Extended modem status frame (0x98)
        struct __attribute__((packed))
        {
            MODEM_STATUS_EXT_t      status;
            char                    data[XBEE_BUF_LEN - 1];
        } ems;

        // Over-the-air (OTA) firmware update status frame
        struct __attribute__((packed))
        {
            uint64_t                src_addr;           // NOTE: MSB-first ("network order")
            uint16_t                dest_net_addr;      // NOTE: MSB-first ("network order")
            uint8_t                 options;
            BOOTLOADER_MSG_TYPE_t   message_type;
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
} XBEE_PACKET_BUF_t;

#endif
