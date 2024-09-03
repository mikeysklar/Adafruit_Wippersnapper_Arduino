/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.8 */

#ifndef PB_WIPPERSNAPPER_UART_UART_PB_H_INCLUDED
#define PB_WIPPERSNAPPER_UART_UART_PB_H_INCLUDED
#include <pb.h>
#include "sensor.pb.h"

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Struct definitions */
/* *
 UARTBusData represents a message to configure a UART bus for communication with a device.
 NOTE: This message is never sent directly, it is packed inside UARTAdd. */
typedef struct _wippersnapper_uart_UARTBusData {
    int32_t baudrate; /* * The baudrate to use for UART communication (may be a common baud rate such as:
1200bps, 2400bps, 4800bps, 19200bps, 38400bps, 57600bps, or 115200bps). */
    char pin_rx[6]; /* * The pin on which to receive UART stream data. */
    char pin_tx[6]; /* * The pin on which to transmit UART stream data. */
    bool is_invert; /* * Inverts the UART signal on RX and TX pins. Defaults to False. */
} wippersnapper_uart_UARTBusData;

/* *
 UARTAdd represents a message sent from IO to a device
 to configure the UART bus (if not already configured) and attach a device. */
typedef struct _wippersnapper_uart_UARTAdd {
    bool has_bus_info;
    wippersnapper_uart_UARTBusData bus_info; /* * The UART bus configuration. */
    pb_callback_t device_id; /* * The unique identifier of the device to attach to the UART bus, from Adafruit_WipperSnapper_Components. */
    int32_t polling_interval; /* * The polling interval, in milliseconds, to use for the device. */
} wippersnapper_uart_UARTAdd;

/* *
 UARTAdded represents a message sent from a device to IO to
 confirm that a device has been attached to the UART bus. */
typedef struct _wippersnapper_uart_UARTAdded {
    pb_callback_t device_id; /* * The unique identifier of the device to attach to the UART bus, from Adafruit_WipperSnapper_Components. */
    bool is_success; /* * True if the UARTInit was successful, False otherwise. */
} wippersnapper_uart_UARTAdded;

/* UARTRemove represents a message sent from IO to a device
 to detach a device from the UART bus. */
typedef struct _wippersnapper_uart_UARTRemove {
    pb_callback_t device_id; /* * The unique identifier of the device to detach from the UART bus. */
} wippersnapper_uart_UARTRemove;

/* *
 UARTEvent represents incoming data from a UART sensor. */
typedef struct _wippersnapper_uart_UARTEvent {
    pb_callback_t device_id; /* * Unique identifier of the device to attach to the UART bus, from Adafruit_WipperSnapper_Components. */
    pb_callback_t sensor_events; /* * An optionally repeated event from a sensor. */
} wippersnapper_uart_UARTEvent;


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define wippersnapper_uart_UARTBusData_init_default {0, "", "", 0}
#define wippersnapper_uart_UARTAdd_init_default  {false, wippersnapper_uart_UARTBusData_init_default, {{NULL}, NULL}, 0}
#define wippersnapper_uart_UARTAdded_init_default {{{NULL}, NULL}, 0}
#define wippersnapper_uart_UARTRemove_init_default {{{NULL}, NULL}}
#define wippersnapper_uart_UARTEvent_init_default {{{NULL}, NULL}, {{NULL}, NULL}}
#define wippersnapper_uart_UARTBusData_init_zero {0, "", "", 0}
#define wippersnapper_uart_UARTAdd_init_zero     {false, wippersnapper_uart_UARTBusData_init_zero, {{NULL}, NULL}, 0}
#define wippersnapper_uart_UARTAdded_init_zero   {{{NULL}, NULL}, 0}
#define wippersnapper_uart_UARTRemove_init_zero  {{{NULL}, NULL}}
#define wippersnapper_uart_UARTEvent_init_zero   {{{NULL}, NULL}, {{NULL}, NULL}}

/* Field tags (for use in manual encoding/decoding) */
#define wippersnapper_uart_UARTBusData_baudrate_tag 1
#define wippersnapper_uart_UARTBusData_pin_rx_tag 2
#define wippersnapper_uart_UARTBusData_pin_tx_tag 3
#define wippersnapper_uart_UARTBusData_is_invert_tag 4
#define wippersnapper_uart_UARTAdd_bus_info_tag  1
#define wippersnapper_uart_UARTAdd_device_id_tag 2
#define wippersnapper_uart_UARTAdd_polling_interval_tag 3
#define wippersnapper_uart_UARTAdded_device_id_tag 1
#define wippersnapper_uart_UARTAdded_is_success_tag 2
#define wippersnapper_uart_UARTRemove_device_id_tag 1
#define wippersnapper_uart_UARTEvent_device_id_tag 1
#define wippersnapper_uart_UARTEvent_sensor_events_tag 2

/* Struct field encoding specification for nanopb */
#define wippersnapper_uart_UARTBusData_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    baudrate,          1) \
X(a, STATIC,   SINGULAR, STRING,   pin_rx,            2) \
X(a, STATIC,   SINGULAR, STRING,   pin_tx,            3) \
X(a, STATIC,   SINGULAR, BOOL,     is_invert,         4)
#define wippersnapper_uart_UARTBusData_CALLBACK NULL
#define wippersnapper_uart_UARTBusData_DEFAULT NULL

#define wippersnapper_uart_UARTAdd_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, MESSAGE,  bus_info,          1) \
X(a, CALLBACK, SINGULAR, STRING,   device_id,         2) \
X(a, STATIC,   SINGULAR, INT32,    polling_interval,   3)
#define wippersnapper_uart_UARTAdd_CALLBACK pb_default_field_callback
#define wippersnapper_uart_UARTAdd_DEFAULT NULL
#define wippersnapper_uart_UARTAdd_bus_info_MSGTYPE wippersnapper_uart_UARTBusData

#define wippersnapper_uart_UARTAdded_FIELDLIST(X, a) \
X(a, CALLBACK, SINGULAR, STRING,   device_id,         1) \
X(a, STATIC,   SINGULAR, BOOL,     is_success,        2)
#define wippersnapper_uart_UARTAdded_CALLBACK pb_default_field_callback
#define wippersnapper_uart_UARTAdded_DEFAULT NULL

#define wippersnapper_uart_UARTRemove_FIELDLIST(X, a) \
X(a, CALLBACK, SINGULAR, STRING,   device_id,         1)
#define wippersnapper_uart_UARTRemove_CALLBACK pb_default_field_callback
#define wippersnapper_uart_UARTRemove_DEFAULT NULL

#define wippersnapper_uart_UARTEvent_FIELDLIST(X, a) \
X(a, CALLBACK, SINGULAR, STRING,   device_id,         1) \
X(a, CALLBACK, REPEATED, MESSAGE,  sensor_events,     2)
#define wippersnapper_uart_UARTEvent_CALLBACK pb_default_field_callback
#define wippersnapper_uart_UARTEvent_DEFAULT NULL
#define wippersnapper_uart_UARTEvent_sensor_events_MSGTYPE wippersnapper_sensor_SensorEvent

extern const pb_msgdesc_t wippersnapper_uart_UARTBusData_msg;
extern const pb_msgdesc_t wippersnapper_uart_UARTAdd_msg;
extern const pb_msgdesc_t wippersnapper_uart_UARTAdded_msg;
extern const pb_msgdesc_t wippersnapper_uart_UARTRemove_msg;
extern const pb_msgdesc_t wippersnapper_uart_UARTEvent_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define wippersnapper_uart_UARTBusData_fields &wippersnapper_uart_UARTBusData_msg
#define wippersnapper_uart_UARTAdd_fields &wippersnapper_uart_UARTAdd_msg
#define wippersnapper_uart_UARTAdded_fields &wippersnapper_uart_UARTAdded_msg
#define wippersnapper_uart_UARTRemove_fields &wippersnapper_uart_UARTRemove_msg
#define wippersnapper_uart_UARTEvent_fields &wippersnapper_uart_UARTEvent_msg

/* Maximum encoded size of messages (where known) */
/* wippersnapper_uart_UARTAdd_size depends on runtime parameters */
/* wippersnapper_uart_UARTAdded_size depends on runtime parameters */
/* wippersnapper_uart_UARTRemove_size depends on runtime parameters */
/* wippersnapper_uart_UARTEvent_size depends on runtime parameters */
#define WIPPERSNAPPER_UART_UART_PB_H_MAX_SIZE    wippersnapper_uart_UARTBusData_size
#define wippersnapper_uart_UARTBusData_size      27

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
