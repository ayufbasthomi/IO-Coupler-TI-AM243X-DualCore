#ifndef IPC_SHAREMEM_H
#define IPC_SHAREMEM_H

#include <stdint.h>

#define IPC_MAGIC      0x47455350U

#define MAX_NODES      20
#define MAX_ANALOG_CH  8

#define MAX_DI         MAX_NODES
#define MAX_DO         MAX_NODES
#define MAX_AI         (MAX_NODES * MAX_ANALOG_CH)
#define MAX_AO         (MAX_NODES * MAX_ANALOG_CH)

#define UNKNOWN 0
#define DO      1
#define DI      2
#define AI_C    3
#define AI_V    4
#define AO_C    5
#define AO_V    6
#define RTDY    7
#define RTDB    8

#define IPC_NOTIFY_CLIENT_DO 0
#define IPC_NOTIFY_CLIENT_AO 1

enum io_device_type
{
    IO_DEVICE_TYPE_DO16 = 0x01,
    IO_DEVICE_TYPE_DI16 = 0x02,
    IO_DEVICE_TYPE_AIC8 = 0x03,
    IO_DEVICE_TYPE_AIV8 = 0x04,
    IO_DEVICE_TYPE_AOC8 = 0x05,
    IO_DEVICE_TYPE_AOV8 = 0x06,
    IO_DEVICE_TYPE_RTDY = 0x07,
    IO_DEVICE_TYPE_RTDB = 0x08,
};

typedef struct
{
    uint16_t di[MAX_DI];
    uint16_t do_[MAX_DO];

    int16_t ai[MAX_AI];
    int16_t ao[MAX_AO];

    // uint32_t doDirtyMask;

} IO_DataModel;

typedef struct
{
    uint8_t nodeId;
    uint8_t ioType;

    uint16_t diIndex;
    uint16_t doIndex;
    uint16_t aiIndex;
    uint16_t aoIndex;

} CANopenModule;

typedef struct
{
    uint16_t digital;
    int16_t analog[8];

    uint8_t ioType;

    char hwVer[16];
    char fwVer[16];
    char SN[32];

    char nodeState[20];

    uint8_t  lastErrorType;
    uint32_t lastErrorCode;

    uint8_t initialized;

} CANopenNodeData;

typedef struct
{
    uint32_t magic;

    uint32_t heartbeat_core0;
    uint32_t heartbeat_core1;

    IO_DataModel io;

    CANopenModule modules[MAX_NODES];
    CANopenModule txModules[MAX_NODES];
    CANopenModule rxModules[MAX_NODES];

    uint16_t moduleCount;
    uint16_t txCount;
    uint16_t rxCount;

    CANopenNodeData nodeData[MAX_NODES];

} ipc_shared_t;

extern volatile ipc_shared_t gSharedMem;

void IPC_SharedInit(void);
void IPC_Lock(void);
void IPC_Unlock(void);
void IPC_LockIO(void);
void IPC_UnlockIO(void);
#endif