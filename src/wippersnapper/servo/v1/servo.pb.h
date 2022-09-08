/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.5-dev at Thu Sep  8 15:41:00 2022. */

#ifndef PB_WIPPERSNAPPER_SERVO_V1_WIPPERSNAPPER_SERVO_V1_SERVO_PB_H_INCLUDED
#define PB_WIPPERSNAPPER_SERVO_V1_WIPPERSNAPPER_SERVO_V1_SERVO_PB_H_INCLUDED
#include <pb.h>
#include "nanopb/nanopb.pb.h"

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Struct definitions */
typedef struct _wippersnapper_servo_v1_ServoAttachRequest {
    char servo_pin[6];
    int32_t servo_freq;
    int32_t min_pulse_width;
    int32_t max_pulse_width;
} wippersnapper_servo_v1_ServoAttachRequest;

typedef struct _wippersnapper_servo_v1_ServoAttachResponse {
    bool attach_success;
    char servo_pin[6];
} wippersnapper_servo_v1_ServoAttachResponse;

typedef struct _wippersnapper_servo_v1_ServoDetachRequest {
    char servo_pin[5];
} wippersnapper_servo_v1_ServoDetachRequest;

typedef struct _wippersnapper_servo_v1_ServoWriteRequest {
    char servo_pin[5];
    int32_t pulse_width;
} wippersnapper_servo_v1_ServoWriteRequest;


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define wippersnapper_servo_v1_ServoAttachRequest_init_default {"", 0, 0, 0}
#define wippersnapper_servo_v1_ServoAttachResponse_init_default {0, ""}
#define wippersnapper_servo_v1_ServoDetachRequest_init_default {""}
#define wippersnapper_servo_v1_ServoWriteRequest_init_default {"", 0}
#define wippersnapper_servo_v1_ServoAttachRequest_init_zero {"", 0, 0, 0}
#define wippersnapper_servo_v1_ServoAttachResponse_init_zero {0, ""}
#define wippersnapper_servo_v1_ServoDetachRequest_init_zero {""}
#define wippersnapper_servo_v1_ServoWriteRequest_init_zero {"", 0}

/* Field tags (for use in manual encoding/decoding) */
#define wippersnapper_servo_v1_ServoAttachRequest_servo_pin_tag 1
#define wippersnapper_servo_v1_ServoAttachRequest_servo_freq_tag 2
#define wippersnapper_servo_v1_ServoAttachRequest_min_pulse_width_tag 3
#define wippersnapper_servo_v1_ServoAttachRequest_max_pulse_width_tag 4
#define wippersnapper_servo_v1_ServoAttachResponse_attach_success_tag 1
#define wippersnapper_servo_v1_ServoAttachResponse_servo_pin_tag 2
#define wippersnapper_servo_v1_ServoDetachRequest_servo_pin_tag 1
#define wippersnapper_servo_v1_ServoWriteRequest_servo_pin_tag 1
#define wippersnapper_servo_v1_ServoWriteRequest_pulse_width_tag 2

/* Struct field encoding specification for nanopb */
#define wippersnapper_servo_v1_ServoAttachRequest_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, STRING,   servo_pin,         1) \
X(a, STATIC,   SINGULAR, INT32,    servo_freq,        2) \
X(a, STATIC,   SINGULAR, INT32,    min_pulse_width,   3) \
X(a, STATIC,   SINGULAR, INT32,    max_pulse_width,   4)
#define wippersnapper_servo_v1_ServoAttachRequest_CALLBACK NULL
#define wippersnapper_servo_v1_ServoAttachRequest_DEFAULT NULL

#define wippersnapper_servo_v1_ServoAttachResponse_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, BOOL,     attach_success,    1) \
X(a, STATIC,   SINGULAR, STRING,   servo_pin,         2)
#define wippersnapper_servo_v1_ServoAttachResponse_CALLBACK NULL
#define wippersnapper_servo_v1_ServoAttachResponse_DEFAULT NULL

#define wippersnapper_servo_v1_ServoDetachRequest_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, STRING,   servo_pin,         1)
#define wippersnapper_servo_v1_ServoDetachRequest_CALLBACK NULL
#define wippersnapper_servo_v1_ServoDetachRequest_DEFAULT NULL

#define wippersnapper_servo_v1_ServoWriteRequest_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, STRING,   servo_pin,         1) \
X(a, STATIC,   SINGULAR, INT32,    pulse_width,       2)
#define wippersnapper_servo_v1_ServoWriteRequest_CALLBACK NULL
#define wippersnapper_servo_v1_ServoWriteRequest_DEFAULT NULL

extern const pb_msgdesc_t wippersnapper_servo_v1_ServoAttachRequest_msg;
extern const pb_msgdesc_t wippersnapper_servo_v1_ServoAttachResponse_msg;
extern const pb_msgdesc_t wippersnapper_servo_v1_ServoDetachRequest_msg;
extern const pb_msgdesc_t wippersnapper_servo_v1_ServoWriteRequest_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define wippersnapper_servo_v1_ServoAttachRequest_fields &wippersnapper_servo_v1_ServoAttachRequest_msg
#define wippersnapper_servo_v1_ServoAttachResponse_fields &wippersnapper_servo_v1_ServoAttachResponse_msg
#define wippersnapper_servo_v1_ServoDetachRequest_fields &wippersnapper_servo_v1_ServoDetachRequest_msg
#define wippersnapper_servo_v1_ServoWriteRequest_fields &wippersnapper_servo_v1_ServoWriteRequest_msg

/* Maximum encoded size of messages (where known) */
#define wippersnapper_servo_v1_ServoAttachRequest_size 40
#define wippersnapper_servo_v1_ServoAttachResponse_size 9
#define wippersnapper_servo_v1_ServoDetachRequest_size 6
#define wippersnapper_servo_v1_ServoWriteRequest_size 17

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
