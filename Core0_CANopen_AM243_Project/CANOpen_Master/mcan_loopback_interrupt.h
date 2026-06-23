#ifndef MCAN_LOOPBACK_INTERRUPT_H
#define MCAN_LOOPBACK_INTERRUPT_H

#include <stdint.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#include "ipc_shareMem.h"

#define ENABLE_DEBUG_LOG 0

#if ENABLE_DEBUG_LOG
#define DEBUG_LOG(...) DebugP_log(__VA_ARGS__)
#else
#define DEBUG_LOG(...)
#endif

#define CAN_RX_MODE_POLLING      0
#define CAN_RX_MODE_INTERRUPT    1

#define CAN_RX_MODE CAN_RX_MODE_INTERRUPT

#define COMM_TYPE_MODBUSTCP      0
#define COMM_TYPE_ENIP           1
#define COMM_TYPE_ETHERCAT       2

#define COMM_TYPE COMM_TYPE_ETHERCAT

#define APP_MCAN_INTR_NUM        (CONFIG_MCAN0_INTR)

#define CANOPEN_OK              0
#define APP_MCAN_BASE_ADDR      (CONFIG_MCAN0_BASE_ADDR)

#define CAN_TX_QUEUE_SIZE       32

typedef struct
{
    uint8_t nodeId;
    uint8_t type;

    uint16_t value;

    int16_t analog[8];

} CAN_TxMsg;

/* CANopen runtime objects */
extern QueueHandle_t     gCanTxQueue;
extern SemaphoreHandle_t gIODataMutex;

/* CANopen APIs */
int32_t CANopen_writeRPDO(
    uint8_t nodeId,
    uint16_t value);

int32_t CANopen_writeRPDO_Analog(
    uint8_t nodeId,
    int16_t values[8]);

void ECAT_BuildModuleMapping(void);

volatile CANopenModule* CANopen_findDO(uint16_t doIndex);
volatile CANopenModule* CANopen_findDI(uint16_t diIndex);
volatile CANopenModule* CANopen_findAI(uint16_t aiIndex);
volatile CANopenModule* CANopen_findAO(uint16_t aoIndex);
volatile CANopenModule* findModule(uint8_t nid);

void create_mcan_task(void);

#endif