#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "ipc_shareMem.h"

#include <kernel/dpl/DebugP.h>
#include <drivers/ipc_notify.h>
#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/CacheP.h>
#include <kernel/dpl/SemaphoreP.h>
#include <kernel/dpl/AddrTranslateP.h>

#include <drivers/spinlock.h>

#define SPINLOCK_BASE_ADDR (CSL_SPINLOCK0_BASE)
#define SPINLOCK_LOCK_NUM  (0)

volatile ipc_shared_t gSharedMem
__attribute__((aligned(128),
section(".bss.user_shared_mem")));

static uint32_t spinlockBaseAddr;
static SemaphoreP_Object mutexObj;

void IPC_SharedInit(void)
{
    int32_t status;

    spinlockBaseAddr =
        (uint32_t)AddrTranslateP_getLocalAddr(
            SPINLOCK_BASE_ADDR);

    status =
        SemaphoreP_constructMutex(
            &mutexObj);

    DebugP_assert(
        status == SystemP_SUCCESS);
}

static void IPC_TakeSpinlock(void)
{
    int32_t status;

    SemaphoreP_pend(
        &mutexObj,
        SystemP_WAIT_FOREVER);

    while(1)
    {
        status =
            Spinlock_lock(
                spinlockBaseAddr,
                SPINLOCK_LOCK_NUM);

        if(status ==
           SPINLOCK_LOCK_STATUS_FREE)
        {
            break;
        }
    }
}

static void IPC_ReleaseSpinlock(void)
{
    Spinlock_unlock(
        spinlockBaseAddr,
        SPINLOCK_LOCK_NUM);

    SemaphoreP_post(
        &mutexObj);
}

void IPC_Lock(void)
{
    IPC_TakeSpinlock();

    CacheP_inv(
        (void*)&gSharedMem,
        sizeof(gSharedMem),
        CacheP_TYPE_ALL);
}

void IPC_Unlock(void)
{
    CacheP_wb(
        (void*)&gSharedMem,
        sizeof(gSharedMem),
        CacheP_TYPE_ALL);

    IPC_ReleaseSpinlock();
}

void IPC_LockIO(void)
{
    IPC_TakeSpinlock();

    CacheP_inv(
        (void*)&gSharedMem.io,
        sizeof(gSharedMem.io),
        CacheP_TYPE_ALL);
}

void IPC_UnlockIO(void)
{
    CacheP_wb(
        (void*)&gSharedMem.io,
        sizeof(gSharedMem.io),
        CacheP_TYPE_ALL);

    IPC_ReleaseSpinlock();
}