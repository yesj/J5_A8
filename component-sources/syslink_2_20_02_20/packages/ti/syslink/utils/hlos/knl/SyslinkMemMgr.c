/*
 *  @file   SyslinkMemMgr.c
 *
 *  @brief   Implements front end for the SyslinkMemMgr implementations.
 *
 *
 *  ============================================================================
 *
 *  Copyright (c) 2008-2012, Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  
 *  *  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  
 *  *  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  
 *  *  Neither the name of Texas Instruments Incorporated nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 *  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 *  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *  Contact information for paper mail:
 *  Texas Instruments
 *  Post Office Box 655303
 *  Dallas, Texas 75265
 *  Contact information: 
 *  http://www-k.ext.ti.com/sc/technical-support/product-information-centers.htm?
 *  DCMP=TIHomeTracking&HQS=Other+OT+home_d_contact
 *  ============================================================================
 *  
 */


#if defined(SYSLINK_BUILDOS_LINUX)
#include <linux/string.h>
#else
#include <string.h>
#endif

/* Standard headers */
#include <ti/syslink/Std.h>
#include <ti/syslink/utils/Trace.h>
#include <ti/syslink/utils/Memory.h>
#include <ti/syslink/utils/String.h>

#include <ti/syslink/inc/SyslinkMemMgr_errBase.h>
#include <ti/syslink/inc/_SyslinkMemMgr.h>
#include <ti/syslink/utils/SyslinkMemMgr.h>
#include <ti/syslink/inc/SharedMemoryMgr.h>


struct SyslinkMemMgr_Object {
   SyslinkMemMgr_Fxns *fxns;
   UInt8       name[MEMMGR_MAX_NAME_LENGTH];
   SyslinkMemMgr_Type interfaceType;
   Ptr         handle;
};

extern SyslinkMemMgr_Fxns SyslinkMemMgr_SharedMemory_fxns;
/* =============================================================================
 * APIs
 * =============================================================================
 */
/* Function to create a SyslinkMemMgr instance */
SyslinkMemMgr_Handle SyslinkMemMgr_create(Ptr params)
{
    Int32 status = MEMMGR_SUCCESS;
    Ptr                  impHandle = NULL;
    SyslinkMemMgr_Object       *obj = NULL;
    SyslinkMemMgr_Handle       handle = NULL;
    SyslinkMemMgr_CreateParams *commonCreParams = (SyslinkMemMgr_CreateParams *)params;

    if (params == NULL) {
       status = MEMMGR_E_INVALIDARG;
    }
    else {
        switch (commonCreParams->memMgrType) {
            case SyslinkMemMgr_TYPE_SHAREDMEM:
                impHandle = SyslinkMemMgr_SharedMemory_create(params);
                if (impHandle == NULL) {
                    status = MEMMGR_E_FAIL;
                }
                else {
                    obj = (SyslinkMemMgr_Object *)Memory_alloc(NULL,
                        sizeof(SyslinkMemMgr_Object), 0, NULL);
                    if (obj == NULL) {
                        status = MEMMGR_E_FAIL;
                    }
                    else {
                        obj->fxns = &SyslinkMemMgr_SharedMemory_fxns;
                        strcpy((String)obj->name,
                               (String)commonCreParams->name);
                        obj->interfaceType = SyslinkMemMgr_TYPE_SHAREDMEM;
                        obj->handle = impHandle;
                    }

                }
            break;

            case SyslinkMemMgr_TYPE_TILERMEM:
            {   /* TODO*/
                status = MEMMGR_E_NOTIMPLEMENTED;
            }
            break;

            default:
                status = MEMMGR_E_INVALIDARG;
         }
    }

    if (status >= 0) {
        handle = obj;
    }

    return (handle);
}

/* Function to delete the created Memory Manager  instance*/
Int32 SyslinkMemMgr_delete(SyslinkMemMgr_Handle *pHandle)
{
    Int32 status = MEMMGR_SUCCESS;
    SyslinkMemMgr_Object * obj = *pHandle;

    if (obj == NULL) {
        status = MEMMGR_E_INVALIDARG;
    }
    else if (obj->handle == NULL) {
        status = MEMMGR_E_INVALIDARG;
    }
    else {
         switch (obj->interfaceType) {
            case SyslinkMemMgr_TYPE_SHAREDMEM:
                status = SyslinkMemMgr_SharedMemory_delete(
                        (SyslinkMemMgr_SharedMemory_Handle*)&obj->handle);

                Memory_free(NULL, obj, sizeof(SyslinkMemMgr_Object));
                break;

            case SyslinkMemMgr_TYPE_TILERMEM:
                /* TODO*/
                status = MEMMGR_E_NOTIMPLEMENTED;
                break;

            default:
                status = MEMMGR_E_INVALIDARG;
         }
    }

    return (status);
}


/* Function to allocate memory from the SyslinkMemMgr */
Ptr SyslinkMemMgr_alloc(SyslinkMemMgr_Object *obj,
        SyslinkMemMgr_AllocParams *params)
{
    Ptr ptr = NULL;

    if ((obj != NULL) && (obj->handle != NULL)) {

        if (obj->fxns->alloc != NULL) {
            ptr = obj->fxns->alloc(obj->handle, params);
        }
    }

    return (ptr);
}


/* Function to allocate memory from the SyslinkMemMgr */
Int32 SyslinkMemMgr_free(SyslinkMemMgr_Object * obj, Ptr buf, UInt32 size)
{
    Int32 status = MEMMGR_SUCCESS;

    if (obj == NULL) {
        status = MEMMGR_E_INVALIDARG;
    }
    else if (obj->handle == NULL) {
        status = MEMMGR_E_INVALIDARG;
    }
    else {
        if (obj->fxns->free != NULL) {
            status = obj->fxns->free(obj->handle, buf, size);
        }
    }

    return (status);
}

/* Function to map. not used for shared memory*/
Ptr
SyslinkMemMgr_map (SyslinkMemMgr_Object * obj, Ptr arg)
{
    Ptr   ptr = NULL;

    if (   (obj != NULL)
        && (obj->handle != NULL)) {

        if (obj->fxns->map != NULL) {
            ptr = obj->fxns->map(obj->handle, arg);
        }
    }

    return (ptr);
}

/* Function to do unmap */
Int32
SyslinkMemMgr_unmap (SyslinkMemMgr_Object * obj, Ptr arg)
{
    Int32 status = MEMMGR_SUCCESS;

    if (  (obj != NULL) && (obj->handle != NULL)) {
        if (obj->fxns->unmap != NULL) {
            status = obj->fxns->unmap(obj->handle,
                                      arg);
        }
        else {
            status = MEMMGR_E_NOTIMPLEMENTED;
        }
    }
    else {
        status = MEMMGR_E_INVALIDARG;
    }

    return (status);
}

/* Function to translate source address to destination address type */
Ptr SyslinkMemMgr_translate(SyslinkMemMgr_Object *obj, Ptr srcAddr,
        SyslinkMemMgr_AddrType srcAddrType, SyslinkMemMgr_AddrType desAddrType)
{
    Ptr ptr = NULL;

    ptr = obj->fxns->translate(obj->handle, srcAddr, srcAddrType, desAddrType);

    return (ptr);
}
