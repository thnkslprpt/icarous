#pragma once
// MESSAGE DEVICE_OP_WRITE PACKING

#define MAVLINK_MSG_ID_DEVICE_OP_WRITE 11002

MAVPACKED(
typedef struct __mavlink_device_op_write_t {
 uint32_t request_id; /*< request ID - copied to reply*/
 uint8_t target_system; /*< System ID*/
 uint8_t target_component; /*< Component ID*/
 uint8_t bustype; /*< The bus type*/
 uint8_t bus; /*< Bus number*/
 uint8_t address; /*< Bus address*/
 char busname[40]; /*< Name of device on bus (for SPI)*/
 uint8_t regstart; /*< First register to write*/
 uint8_t count; /*< count of registers to write*/
 uint8_t data[128]; /*< write data*/
}) mavlink_device_op_write_t;

#define MAVLINK_MSG_ID_DEVICE_OP_WRITE_LEN 179
#define MAVLINK_MSG_ID_DEVICE_OP_WRITE_MIN_LEN 179
#define MAVLINK_MSG_ID_11002_LEN 179
#define MAVLINK_MSG_ID_11002_MIN_LEN 179

#define MAVLINK_MSG_ID_DEVICE_OP_WRITE_CRC 234
#define MAVLINK_MSG_ID_11002_CRC 234

#define MAVLINK_MSG_DEVICE_OP_WRITE_FIELD_BUSNAME_LEN 40
#define MAVLINK_MSG_DEVICE_OP_WRITE_FIELD_DATA_LEN 128

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_DEVICE_OP_WRITE { \
    11002, \
    "DEVICE_OP_WRITE", \
    10, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_device_op_write_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_device_op_write_t, target_component) }, \
         { "request_id", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_device_op_write_t, request_id) }, \
         { "bustype", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_device_op_write_t, bustype) }, \
         { "bus", NULL, MAVLINK_TYPE_UINT8_T, 0, 7, offsetof(mavlink_device_op_write_t, bus) }, \
         { "address", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_device_op_write_t, address) }, \
         { "busname", NULL, MAVLINK_TYPE_CHAR, 40, 9, offsetof(mavlink_device_op_write_t, busname) }, \
         { "regstart", NULL, MAVLINK_TYPE_UINT8_T, 0, 49, offsetof(mavlink_device_op_write_t, regstart) }, \
         { "count", NULL, MAVLINK_TYPE_UINT8_T, 0, 50, offsetof(mavlink_device_op_write_t, count) }, \
         { "data", NULL, MAVLINK_TYPE_UINT8_T, 128, 51, offsetof(mavlink_device_op_write_t, data) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_DEVICE_OP_WRITE { \
    "DEVICE_OP_WRITE", \
    10, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_device_op_write_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_device_op_write_t, target_component) }, \
         { "request_id", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_device_op_write_t, request_id) }, \
         { "bustype", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_device_op_write_t, bustype) }, \
         { "bus", NULL, MAVLINK_TYPE_UINT8_T, 0, 7, offsetof(mavlink_device_op_write_t, bus) }, \
         { "address", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_device_op_write_t, address) }, \
         { "busname", NULL, MAVLINK_TYPE_CHAR, 40, 9, offsetof(mavlink_device_op_write_t, busname) }, \
         { "regstart", NULL, MAVLINK_TYPE_UINT8_T, 0, 49, offsetof(mavlink_device_op_write_t, regstart) }, \
         { "count", NULL, MAVLINK_TYPE_UINT8_T, 0, 50, offsetof(mavlink_device_op_write_t, count) }, \
         { "data", NULL, MAVLINK_TYPE_UINT8_T, 128, 51, offsetof(mavlink_device_op_write_t, data) }, \
         } \
}
#endif

/**
 * @brief Pack a device_op_write message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system System ID
 * @param target_component Component ID
 * @param request_id request ID - copied to reply
 * @param bustype The bus type
 * @param bus Bus number
 * @param address Bus address
 * @param busname Name of device on bus (for SPI)
 * @param regstart First register to write
 * @param count count of registers to write
 * @param data write data
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_device_op_write_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t target_system, uint8_t target_component, uint32_t request_id, uint8_t bustype, uint8_t bus, uint8_t address, const char *busname, uint8_t regstart, uint8_t count, const uint8_t *data)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEVICE_OP_WRITE_LEN];
    _mav_put_uint32_t(buf, 0, request_id);
    _mav_put_uint8_t(buf, 4, target_system);
    _mav_put_uint8_t(buf, 5, target_component);
    _mav_put_uint8_t(buf, 6, bustype);
    _mav_put_uint8_t(buf, 7, bus);
    _mav_put_uint8_t(buf, 8, address);
    _mav_put_uint8_t(buf, 49, regstart);
    _mav_put_uint8_t(buf, 50, count);
    _mav_put_char_array(buf, 9, busname, 40);
    _mav_put_uint8_t_array(buf, 51, data, 128);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DEVICE_OP_WRITE_LEN);
#else
    mavlink_device_op_write_t packet;
    packet.request_id = request_id;
    packet.target_system = target_system;
    packet.target_component = target_component;
    packet.bustype = bustype;
    packet.bus = bus;
    packet.address = address;
    packet.regstart = regstart;
    packet.count = count;
    mav_array_memcpy(packet.busname, busname, sizeof(char)*40);
    mav_array_memcpy(packet.data, data, sizeof(uint8_t)*128);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DEVICE_OP_WRITE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_DEVICE_OP_WRITE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_DEVICE_OP_WRITE_MIN_LEN, MAVLINK_MSG_ID_DEVICE_OP_WRITE_LEN, MAVLINK_MSG_ID_DEVICE_OP_WRITE_CRC);
}

/**
 * @brief Pack a device_op_write message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_system System ID
 * @param target_component Component ID
 * @param request_id request ID - copied to reply
 * @param bustype The bus type
 * @param bus Bus number
 * @param address Bus address
 * @param busname Name of device on bus (for SPI)
 * @param regstart First register to write
 * @param count count of registers to write
 * @param data write data
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_device_op_write_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t target_system,uint8_t target_component,uint32_t request_id,uint8_t bustype,uint8_t bus,uint8_t address,const char *busname,uint8_t regstart,uint8_t count,const uint8_t *data)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEVICE_OP_WRITE_LEN];
    _mav_put_uint32_t(buf, 0, request_id);
    _mav_put_uint8_t(buf, 4, target_system);
    _mav_put_uint8_t(buf, 5, target_component);
    _mav_put_uint8_t(buf, 6, bustype);
    _mav_put_uint8_t(buf, 7, bus);
    _mav_put_uint8_t(buf, 8, address);
    _mav_put_uint8_t(buf, 49, regstart);
    _mav_put_uint8_t(buf, 50, count);
    _mav_put_char_array(buf, 9, busname, 40);
    _mav_put_uint8_t_array(buf, 51, data, 128);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DEVICE_OP_WRITE_LEN);
#else
    mavlink_device_op_write_t packet;
    packet.request_id = request_id;
    packet.target_system = target_system;
    packet.target_component = target_component;
    packet.bustype = bustype;
    packet.bus = bus;
    packet.address = address;
    packet.regstart = regstart;
    packet.count = count;
    mav_array_memcpy(packet.busname, busname, sizeof(char)*40);
    mav_array_memcpy(packet.data, data, sizeof(uint8_t)*128);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DEVICE_OP_WRITE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_DEVICE_OP_WRITE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_DEVICE_OP_WRITE_MIN_LEN, MAVLINK_MSG_ID_DEVICE_OP_WRITE_LEN, MAVLINK_MSG_ID_DEVICE_OP_WRITE_CRC);
}

/**
 * @brief Encode a device_op_write struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param device_op_write C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_device_op_write_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_device_op_write_t* device_op_write)
{
    return mavlink_msg_device_op_write_pack(system_id, component_id, msg, device_op_write->target_system, device_op_write->target_component, device_op_write->request_id, device_op_write->bustype, device_op_write->bus, device_op_write->address, device_op_write->busname, device_op_write->regstart, device_op_write->count, device_op_write->data);
}

/**
 * @brief Encode a device_op_write struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param device_op_write C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_device_op_write_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_device_op_write_t* device_op_write)
{
    return mavlink_msg_device_op_write_pack_chan(system_id, component_id, chan, msg, device_op_write->target_system, device_op_write->target_component, device_op_write->request_id, device_op_write->bustype, device_op_write->bus, device_op_write->address, device_op_write->busname, device_op_write->regstart, device_op_write->count, device_op_write->data);
}

/**
 * @brief Send a device_op_write message
 * @param chan MAVLink channel to send the message
 *
 * @param target_system System ID
 * @param target_component Component ID
 * @param request_id request ID - copied to reply
 * @param bustype The bus type
 * @param bus Bus number
 * @param address Bus address
 * @param busname Name of device on bus (for SPI)
 * @param regstart First register to write
 * @param count count of registers to write
 * @param data write data
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_device_op_write_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, uint32_t request_id, uint8_t bustype, uint8_t bus, uint8_t address, const char *busname, uint8_t regstart, uint8_t count, const uint8_t *data)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEVICE_OP_WRITE_LEN];
    _mav_put_uint32_t(buf, 0, request_id);
    _mav_put_uint8_t(buf, 4, target_system);
    _mav_put_uint8_t(buf, 5, target_component);
    _mav_put_uint8_t(buf, 6, bustype);
    _mav_put_uint8_t(buf, 7, bus);
    _mav_put_uint8_t(buf, 8, address);
    _mav_put_uint8_t(buf, 49, regstart);
    _mav_put_uint8_t(buf, 50, count);
    _mav_put_char_array(buf, 9, busname, 40);
    _mav_put_uint8_t_array(buf, 51, data, 128);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEVICE_OP_WRITE, buf, MAVLINK_MSG_ID_DEVICE_OP_WRITE_MIN_LEN, MAVLINK_MSG_ID_DEVICE_OP_WRITE_LEN, MAVLINK_MSG_ID_DEVICE_OP_WRITE_CRC);
#else
    mavlink_device_op_write_t packet;
    packet.request_id = request_id;
    packet.target_system = target_system;
    packet.target_component = target_component;
    packet.bustype = bustype;
    packet.bus = bus;
    packet.address = address;
    packet.regstart = regstart;
    packet.count = count;
    mav_array_memcpy(packet.busname, busname, sizeof(char)*40);
    mav_array_memcpy(packet.data, data, sizeof(uint8_t)*128);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEVICE_OP_WRITE, (const char *)&packet, MAVLINK_MSG_ID_DEVICE_OP_WRITE_MIN_LEN, MAVLINK_MSG_ID_DEVICE_OP_WRITE_LEN, MAVLINK_MSG_ID_DEVICE_OP_WRITE_CRC);
#endif
}

/**
 * @brief Send a device_op_write message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_device_op_write_send_struct(mavlink_channel_t chan, const mavlink_device_op_write_t* device_op_write)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_device_op_write_send(chan, device_op_write->target_system, device_op_write->target_component, device_op_write->request_id, device_op_write->bustype, device_op_write->bus, device_op_write->address, device_op_write->busname, device_op_write->regstart, device_op_write->count, device_op_write->data);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEVICE_OP_WRITE, (const char *)device_op_write, MAVLINK_MSG_ID_DEVICE_OP_WRITE_MIN_LEN, MAVLINK_MSG_ID_DEVICE_OP_WRITE_LEN, MAVLINK_MSG_ID_DEVICE_OP_WRITE_CRC);
#endif
}

#if MAVLINK_MSG_ID_DEVICE_OP_WRITE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_device_op_write_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t target_system, uint8_t target_component, uint32_t request_id, uint8_t bustype, uint8_t bus, uint8_t address, const char *busname, uint8_t regstart, uint8_t count, const uint8_t *data)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, request_id);
    _mav_put_uint8_t(buf, 4, target_system);
    _mav_put_uint8_t(buf, 5, target_component);
    _mav_put_uint8_t(buf, 6, bustype);
    _mav_put_uint8_t(buf, 7, bus);
    _mav_put_uint8_t(buf, 8, address);
    _mav_put_uint8_t(buf, 49, regstart);
    _mav_put_uint8_t(buf, 50, count);
    _mav_put_char_array(buf, 9, busname, 40);
    _mav_put_uint8_t_array(buf, 51, data, 128);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEVICE_OP_WRITE, buf, MAVLINK_MSG_ID_DEVICE_OP_WRITE_MIN_LEN, MAVLINK_MSG_ID_DEVICE_OP_WRITE_LEN, MAVLINK_MSG_ID_DEVICE_OP_WRITE_CRC);
#else
    mavlink_device_op_write_t *packet = (mavlink_device_op_write_t *)msgbuf;
    packet->request_id = request_id;
    packet->target_system = target_system;
    packet->target_component = target_component;
    packet->bustype = bustype;
    packet->bus = bus;
    packet->address = address;
    packet->regstart = regstart;
    packet->count = count;
    mav_array_memcpy(packet->busname, busname, sizeof(char)*40);
    mav_array_memcpy(packet->data, data, sizeof(uint8_t)*128);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEVICE_OP_WRITE, (const char *)packet, MAVLINK_MSG_ID_DEVICE_OP_WRITE_MIN_LEN, MAVLINK_MSG_ID_DEVICE_OP_WRITE_LEN, MAVLINK_MSG_ID_DEVICE_OP_WRITE_CRC);
#endif
}
#endif

#endif

// MESSAGE DEVICE_OP_WRITE UNPACKING


/**
 * @brief Get field target_system from device_op_write message
 *
 * @return System ID
 */
static inline uint8_t mavlink_msg_device_op_write_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Get field target_component from device_op_write message
 *
 * @return Component ID
 */
static inline uint8_t mavlink_msg_device_op_write_get_target_component(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  5);
}

/**
 * @brief Get field request_id from device_op_write message
 *
 * @return request ID - copied to reply
 */
static inline uint32_t mavlink_msg_device_op_write_get_request_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field bustype from device_op_write message
 *
 * @return The bus type
 */
static inline uint8_t mavlink_msg_device_op_write_get_bustype(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  6);
}

/**
 * @brief Get field bus from device_op_write message
 *
 * @return Bus number
 */
static inline uint8_t mavlink_msg_device_op_write_get_bus(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  7);
}

/**
 * @brief Get field address from device_op_write message
 *
 * @return Bus address
 */
static inline uint8_t mavlink_msg_device_op_write_get_address(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  8);
}

/**
 * @brief Get field busname from device_op_write message
 *
 * @return Name of device on bus (for SPI)
 */
static inline uint16_t mavlink_msg_device_op_write_get_busname(const mavlink_message_t* msg, char *busname)
{
    return _MAV_RETURN_char_array(msg, busname, 40,  9);
}

/**
 * @brief Get field regstart from device_op_write message
 *
 * @return First register to write
 */
static inline uint8_t mavlink_msg_device_op_write_get_regstart(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  49);
}

/**
 * @brief Get field count from device_op_write message
 *
 * @return count of registers to write
 */
static inline uint8_t mavlink_msg_device_op_write_get_count(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  50);
}

/**
 * @brief Get field data from device_op_write message
 *
 * @return write data
 */
static inline uint16_t mavlink_msg_device_op_write_get_data(const mavlink_message_t* msg, uint8_t *data)
{
    return _MAV_RETURN_uint8_t_array(msg, data, 128,  51);
}

/**
 * @brief Decode a device_op_write message into a struct
 *
 * @param msg The message to decode
 * @param device_op_write C-struct to decode the message contents into
 */
static inline void mavlink_msg_device_op_write_decode(const mavlink_message_t* msg, mavlink_device_op_write_t* device_op_write)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    device_op_write->request_id = mavlink_msg_device_op_write_get_request_id(msg);
    device_op_write->target_system = mavlink_msg_device_op_write_get_target_system(msg);
    device_op_write->target_component = mavlink_msg_device_op_write_get_target_component(msg);
    device_op_write->bustype = mavlink_msg_device_op_write_get_bustype(msg);
    device_op_write->bus = mavlink_msg_device_op_write_get_bus(msg);
    device_op_write->address = mavlink_msg_device_op_write_get_address(msg);
    mavlink_msg_device_op_write_get_busname(msg, device_op_write->busname);
    device_op_write->regstart = mavlink_msg_device_op_write_get_regstart(msg);
    device_op_write->count = mavlink_msg_device_op_write_get_count(msg);
    mavlink_msg_device_op_write_get_data(msg, device_op_write->data);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_DEVICE_OP_WRITE_LEN? msg->len : MAVLINK_MSG_ID_DEVICE_OP_WRITE_LEN;
        memset(device_op_write, 0, MAVLINK_MSG_ID_DEVICE_OP_WRITE_LEN);
    memcpy(device_op_write, _MAV_PAYLOAD(msg), len);
#endif
}
