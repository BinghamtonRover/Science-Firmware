/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.8 */

#ifndef PB_SCIENCE_PB_H_INCLUDED
#define PB_SCIENCE_PB_H_INCLUDED
#include "utils/pb.h"

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
/* / The state of a servo. If undefined, don't open or close. */
typedef enum _ServoState {
    ServoState_SERVO_STATE_UNDEFINED = 0,
    ServoState_SERVO_OPEN = 1,
    ServoState_SERVO_CLOSE = 2
} ServoState;

/* / The state of a pump. If undefined: don't do anything. If fill: turn on, wait, then turn off. */
typedef enum _PumpState {
    PumpState_PUMP_STATE_UNDEFINED = 0,
    PumpState_PUMP_ON = 1,
    PumpState_PUMP_OFF = 2,
    PumpState_FILL = 3
} PumpState;

/* / The state of the science subsystem. If not COLLECT_DATA, don't stream data at all. */
typedef enum _ScienceState {
    ScienceState_SCIENCE_STATE_UNDEFINED = 0,
    ScienceState_COLLECT_DATA = 1,
    ScienceState_STOP_COLLECTING = 2
} ScienceState;

/* / A command for the carousel and funnel to follow. */
typedef enum _CarouselCommand {
    CarouselCommand_CAROUSEL_COMMAND_UNDEFINED = 0,
    CarouselCommand_NEXT_TUBE = 1,
    CarouselCommand_PREV_TUBE = 2,
    CarouselCommand_NEXT_SECTION = 3,
    CarouselCommand_PREV_SECTION = 4,
    CarouselCommand_FILL_TUBE = 5,
    CarouselCommand_FILL_SECTION = 6
} CarouselCommand;

/* Struct definitions */
/* / A command to the science subsystem. */
typedef struct _ScienceCommand {
    /* Individual control over each motor. Indicates steps to move */
    float carousel_motor;
    float scoop_motor;
    float subsurface_motor;
    /* Control over other hardware */
    PumpState pumps;
    ServoState funnel;
    ServoState scoop;
    CarouselCommand carousel;
    /* High level commands */
    bool calibrate;
    bool stop;
    int32_t sample;
    ScienceState state;
} ScienceCommand;

/* / Data coming from the science subsystem. */
typedef struct _ScienceData {
    /* High-level data */
    int32_t sample;
    ScienceState state;
    /* Sensor data */
    float co2;
    float humidity;
    float temperature;
} ScienceData;


#ifdef __cplusplus
extern "C" {
#endif

/* Helper constants for enums */
#define _ServoState_MIN ServoState_SERVO_STATE_UNDEFINED
#define _ServoState_MAX ServoState_SERVO_CLOSE
#define _ServoState_ARRAYSIZE ((ServoState)(ServoState_SERVO_CLOSE+1))

#define _PumpState_MIN PumpState_PUMP_STATE_UNDEFINED
#define _PumpState_MAX PumpState_FILL
#define _PumpState_ARRAYSIZE ((PumpState)(PumpState_FILL+1))

#define _ScienceState_MIN ScienceState_SCIENCE_STATE_UNDEFINED
#define _ScienceState_MAX ScienceState_STOP_COLLECTING
#define _ScienceState_ARRAYSIZE ((ScienceState)(ScienceState_STOP_COLLECTING+1))

#define _CarouselCommand_MIN CarouselCommand_CAROUSEL_COMMAND_UNDEFINED
#define _CarouselCommand_MAX CarouselCommand_FILL_SECTION
#define _CarouselCommand_ARRAYSIZE ((CarouselCommand)(CarouselCommand_FILL_SECTION+1))

#define ScienceCommand_pumps_ENUMTYPE PumpState
#define ScienceCommand_funnel_ENUMTYPE ServoState
#define ScienceCommand_scoop_ENUMTYPE ServoState
#define ScienceCommand_carousel_ENUMTYPE CarouselCommand
#define ScienceCommand_state_ENUMTYPE ScienceState

#define ScienceData_state_ENUMTYPE ScienceState


/* Initializer values for message structs */
#define ScienceCommand_init_default              {0, 0, 0, _PumpState_MIN, _ServoState_MIN, _ServoState_MIN, _CarouselCommand_MIN, 0, 0, 0, _ScienceState_MIN}
#define ScienceData_init_default                 {0, _ScienceState_MIN, 0, 0, 0}
#define ScienceCommand_init_zero                 {0, 0, 0, _PumpState_MIN, _ServoState_MIN, _ServoState_MIN, _CarouselCommand_MIN, 0, 0, 0, _ScienceState_MIN}
#define ScienceData_init_zero                    {0, _ScienceState_MIN, 0, 0, 0}

/* Field tags (for use in manual encoding/decoding) */
#define ScienceCommand_carousel_motor_tag        1
#define ScienceCommand_scoop_motor_tag           2
#define ScienceCommand_subsurface_motor_tag      3
#define ScienceCommand_pumps_tag                 4
#define ScienceCommand_funnel_tag                5
#define ScienceCommand_scoop_tag                 6
#define ScienceCommand_carousel_tag              7
#define ScienceCommand_calibrate_tag             8
#define ScienceCommand_stop_tag                  9
#define ScienceCommand_sample_tag                10
#define ScienceCommand_state_tag                 11
#define ScienceData_sample_tag                   1
#define ScienceData_state_tag                    2
#define ScienceData_co2_tag                      3
#define ScienceData_humidity_tag                 4
#define ScienceData_temperature_tag              5

/* Struct field encoding specification for nanopb */
#define ScienceCommand_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, FLOAT,    carousel_motor,    1) \
X(a, STATIC,   SINGULAR, FLOAT,    scoop_motor,       2) \
X(a, STATIC,   SINGULAR, FLOAT,    subsurface_motor,   3) \
X(a, STATIC,   SINGULAR, UENUM,    pumps,             4) \
X(a, STATIC,   SINGULAR, UENUM,    funnel,            5) \
X(a, STATIC,   SINGULAR, UENUM,    scoop,             6) \
X(a, STATIC,   SINGULAR, UENUM,    carousel,          7) \
X(a, STATIC,   SINGULAR, BOOL,     calibrate,         8) \
X(a, STATIC,   SINGULAR, BOOL,     stop,              9) \
X(a, STATIC,   SINGULAR, INT32,    sample,           10) \
X(a, STATIC,   SINGULAR, UENUM,    state,            11)
#define ScienceCommand_CALLBACK NULL
#define ScienceCommand_DEFAULT NULL

#define ScienceData_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    sample,            1) \
X(a, STATIC,   SINGULAR, UENUM,    state,             2) \
X(a, STATIC,   SINGULAR, FLOAT,    co2,               3) \
X(a, STATIC,   SINGULAR, FLOAT,    humidity,          4) \
X(a, STATIC,   SINGULAR, FLOAT,    temperature,       5)
#define ScienceData_CALLBACK NULL
#define ScienceData_DEFAULT NULL

extern const pb_msgdesc_t ScienceCommand_msg;
extern const pb_msgdesc_t ScienceData_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define ScienceCommand_fields &ScienceCommand_msg
#define ScienceData_fields &ScienceData_msg

/* Maximum encoded size of messages (where known) */
#define SCIENCE_PB_H_MAX_SIZE                    ScienceCommand_size
#define ScienceCommand_size                      40
#define ScienceData_size                         28

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
