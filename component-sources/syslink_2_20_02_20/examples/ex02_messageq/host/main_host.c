/*
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== main_host.c ========
 *
 */

/* cstdlib header files */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>//liuxu, 12/19/2013.

//liuxu, 8/21/2013, for mpeg4 encoder.
#define IVI_GETTIME		// enable IviGetTime() in IviTypedef.h
#include <math.h>
#include <time.h>
#include <string.h>

#include "IviTypedef.h"//liuxu, 02/12/2014, for "IviGetTime()".


/* package header files */
#include <ti/syslink/Std.h>     /* must be first */
#include <ti/syslink/IpcHost.h>
#include <ti/syslink/SysLink.h>

#include <ti/ipc/MultiProc.h>

#include <ti/ipc/HeapBufMP.h>//liuxu, 8/20/2013.
#include <ti/ipc/MessageQ.h>//liuxu, 8/20/2013.
//#include <xdc/runtime/IHeap.h>
#include <ti/ipc/SharedRegion.h> 
#include <ti/syslink/utils/Cache.h>//liuxu, 8/20/2013.
#include <ti/syslink/utils/IHeap.h>//liuxu, 8/20/2013.

#define SYSLINK_TRACE_ENABLE//liuxu, 8/19/2013.

#include <ti/syslink/utils/Trace.h>//liuxu, 8/19/2013.

/* local header files */
#include "App.h"

#define GFX_CUBE//liuxu, 8/28/2013, enable GFX demo, which should be enabled exclusively with MP4 encoder. 
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <GLES2/gl2ext.h>

#include <sys/ioctl.h>
//#include <fcntl.h>//liuxu, 12/19/2013, move to outside.
#include <sys/mman.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <sys/time.h>
#include <bc_cat.h>
#include "common.h"

// for 一汽紅旗
#define REDFLAG


// + a0220402, add support carit board key
#define CONFIG_CARIT
// - a0220402,

#define NO_MEMCPY//liuxu, 10/10/2013.

//liuxu, 10/12/2013, option 2, use one of 4 frames to constitute to a texture buffer at size of 736x480x1.5 for YUV420.
#define FRAME_WIDTH     736
#define FRAME_HEIGHT    480
#define FRAME_SIZE      736*480*3/2//liuxu, 9/7/2013. (FRAME_WIDTH * FRAME_HEIGHT * 1.5)


#define MAX_FRAMES      24//liuxu, 10/18/2013.
#define YUV_PIXEL_FMT   BC_PIX_FMT_NV12
#define MAX_BUFFERS     24//liuxu, 10/18/2013.


#define InterChannelIndexOffset 6//liuxu, 10/18/2013, to support 4 channels.

#define M3_FRAMEBUFFER_PA_0 0x83c6c0c0//liuxu, 02/12/2014. //liuxu, 11/19/2013, this would change when frame buffer memory map changes at M3 side.
#define M3_FRAMEBUFFER_PA_1 0x83e718c0
#define M3_FRAMEBUFFER_PA_2 0x840770c0
#define M3_FRAMEBUFFER_PA_3 0x8427c8c0
#define M3_FRAMEBUFFER_PA_4 0x844820c0
#define M3_FRAMEBUFFER_PA_5 0x846878c0//liuxu, 02/12/2014. 


static char *frame[MAX_FRAMES];
static char *yuv_data = NULL;
static int   fr_idx = 0;
static int   bcdev_id = 0;
static tex_buffer_info_t buf_info;

/* 3D AVM */
int    texChYuv[4] = {0};  //0:前 1:左 2:右 3:後
void avm_drawtexture_VBO(int bufferfront,int bufferright,int bufferleft,int bufferback);
void car_drawtexture_VBO();


int Enforcement3DAVM = 1; //強迫開機進入3DAVM


int frame_init(bc_buf_params_t *p)
{
    int   ii;
    if (p) {
        p->count = MAX_FRAMES;
        p->width = FRAME_WIDTH;
        p->height = FRAME_HEIGHT;
        p->fourcc = YUV_PIXEL_FMT;
        p->type = BC_MEMORY_USERPTR;//liuxu, 10/10/2013.
    }
    return 0;
}



void frame_cleanup(void)
{
    if (yuv_data)
        free(yuv_data);
}



void usage(char *arg)
{
    printf("Usage:\n"
           "    %s [-b <id> | -p | -w <width> | -t <height> | -h]\n"
           "\t-b - bc device id [default: 0]\n"
           "\t-p - enable profiling\n"
           "\t-w - width of texture buffer in pixel, multiple of 8 (for ES3.x)\n"
           "\t     or 32 (for ES2.x)\n"
           "\t-t - height of texture buffer in pixel\n"
           "\t-h - print this message\n\n", arg);
}



/* private functions */
static Int Main_main(Void);
static Int Main_parseArgs(Int argc, Char *argv[]);


#define Main_USAGE "\
Usage:\n\
    app_host [options] proc\n\
\n\
Arguments:\n\
    proc      : the name of the remote processor\n\
\n\
Options:\n\
    h   : print this help message\n\
    l   : list the available remote names\n\
\n\
Examples:\n\
    app_host DSP\n\
    app_host -l\n\
    app_host -h\n\
\n"

/* private data */
static String   Main_remoteProcName = NULL;

extern Int curTrace;//liuxu, 8/9/2013.

extern int test111();


/*
 *  ======== main ========
 */
Int main(Int argc, Char* argv[])
{
    Int status;
    
    printf("--> liuxu, main:, curTrace=0x%x\n", curTrace);

	printf("\r\n********************AUTORAD*******************main\n");    
	printf("\r\n********************AUTORAD*******************main\n");    
	printf("\r\n********************AUTORAD*******************main\n");  
    //test111();


    GT_setTrace ((GT_TraceState_Enable | GT_TraceSetFailure_Enable | (4 << (32 - GT_TRACECLASS_SHIFT))), GT_TraceType_User);//liuxu, 8/19/2013, enable trace.

    /* SysLink initialization */
    SysLink_setup();

    /* application create, exec, delete */
    status = Main_main();

    /* SysLink finalization */
    SysLink_destroy();

leave:
    printf("<-- main:\n");
    status = (status >= 0 ? 0 : status);
    
    return (status);
}


/*
 *  ======== Main_main ========
 */
typedef struct cfg4Pointers_t
{
    MessageQ_MsgHeader msgHdr;
    UInt32 cmdType;
    void *pPointer0;
    void *pPointer1;
    void *pPointer2;
    void *pPointer3;
} cfg4Pointers_t;

typedef struct cfg8Pointers_t
{
    MessageQ_MsgHeader msgHdr;
    UInt32 cmdType;
    void *pY_Pointer0;
    void *pY_Pointer1;
    void *pY_Pointer2;
    void *pY_Pointer3;
    void *pUV_Pointer0;
    void *pUV_Pointer1;
    void *pUV_Pointer2;
    void *pUV_Pointer3;
} cfg8Pointers_t;//liuxu, 10/5/2013.
 
#define MSGQ_HEAP_NAME      "msgQHeap"//liuxu, 8/20/2013.
HeapBufMP_Handle hHeap = NULL;
#define MSGQ_HEAPID         0u

MessageQ_Handle hMessageQ = NULL;
cfg4Pointers_t* cmdMsg = NULL;
UInt32 RemoteDspQId;

#define ROUNDUP_SIZE(a, b) (UInt32)((((UInt32)(a)) + (((UInt32)(b)) - 1)) & ~((UInt32)(b) - 1))

Int32 commandQPut(UInt32 remoteQId, void *pCmdMsg)
{
    Int32 nRetVal = 0;

    /* Flush and invalidate  pCmdMsg except MessageQ_MsgHeader. */
    //Cache_wbInv((Ptr) ((Int8 *) pCmdMsg), sizeof(cfg4Pointers_t), (UInt16)Cache_Type_ALL, (Bool)TRUE);

    nRetVal = MessageQ_put((MessageQ_QueueId) remoteQId, pCmdMsg);

    /* Flush and invalidate only MessageQ_MsgHeader in pCmdMsg. */
    //Cache_wbInv((Ptr) pCmdMsg , sizeof(MessageQ_MsgHeader), (UInt16)Cache_Type_ALL, (Bool)TRUE);

    return nRetVal;
}

Int32 commandQGet(void *hCmdQ, void *ppMsg, UInt32 nTimeOut)
{
    /* Invalidate cache. */
    //Cache_inv((cfg4Pointers_t *)(*(cfg4Pointers_t **) ppMsg), sizeof(cfg4Pointers_t), (UInt16)Cache_Type_ALL, (Bool)TRUE);

    return (MessageQ_get((MessageQ_Handle) hCmdQ, (MessageQ_Msg *) ppMsg, nTimeOut));
}



#include <ti/syslink/ProcMgr.h>


#define SAVE_PERSMAT_TO_FS//liuxu, 04/24/2014.

Int Main_main(Void)
{
    UInt16      remoteProcId;
    Int         status = 0;
    Int         printremoteProcId = 0xff;

    MessageQ_Msg pTempCmdMsg = NULL;//liuxu, 8/20/2013.
    UInt16 nSRId = 0u;
    SharedRegion_SRPtr srPtr = {0u};
    cfg8Pointers_t *pFrom_DSP_TempCmdMsg = NULL;

    unsigned char *vitrualY;
    unsigned char *vitrualUV;

    unsigned char *vitrualY1;//liuxu, 10/5/2013.
    unsigned char *vitrualY2;
    unsigned char *vitrualY3;

    unsigned char *vitrualUV1;
    unsigned char *vitrualUV2;
    unsigned char *vitrualUV3;

    int time1_profile, time2_profile, time3_profile, time4_profile, time5_profile, time6_profile, time7_profile, time8_profile, time9_profile;//liuxu, 10/12/2013.


    printf("--> Main_main:\n");


    remoteProcId = MultiProc_getId("DSP");

    /* attach to the remote processor */
    printremoteProcId = MultiProc_getId("HOST");

    Osal_printf("\nliuxu, remoteProcId=%d, host-printremoteProcId=0x%x\n", remoteProcId, printremoteProcId);


    void *ipc_vector = (0x85c00400);//liuxu, 07/01/2014, this is the address of ipc header.

    status = Ipc_control(remoteProcId, Ipc_CONTROLCMD_LOADCALLBACK, &ipc_vector);//liuxu, 07/01/2014, support late attach. 

    
    if (status < 0) {
        printf("Main_main: load callback failed, remoteProcId=%d\n", remoteProcId);
        goto leave;
    }

    /* invoke the SysLink start callback */
    status = Ipc_control(remoteProcId, Ipc_CONTROLCMD_STARTCALLBACK, NULL);

    if (status < 0) {
        printf("Main_main: start callback failed\n");
        goto leave;
    }

    /* BEGIN application phase */
    printf("--> liuxu App_create:\n");//liuxu, 8/9/2013.

    do
    {
            /* Open the heap created by the other processor. Loop until opened. */
            do
            {
                status = HeapBufMP_open(MSGQ_HEAP_NAME, &hHeap);
                /*
                 *  Sleep for 1 clock tick to avoid inundating remote processor
                 *  with interrupts if open failed
                 */
                if (status < 0)
                {
                    sleep(1);//liuxu, 8/20/2013, sleep of linux.
                }
            } while (status < 0);

            /* Register this heap with MessageQ */
            MessageQ_registerHeap((IHeap_Handle) hHeap, MSGQ_HEAPID);

    } while(0);

    hMessageQ = MessageQ_create((String) "A8_CMD_Q_TO_DSP_LITE", NULL);
    cmdMsg = (cfg4Pointers_t *)MessageQ_alloc(MSGQ_HEAPID, ROUNDUP_SIZE(sizeof(cfg4Pointers_t), 128));//liuxuliuxu, don't forget to free at the end.

    do
    {
        status = MessageQ_open("DSP_CMD_Q_TO_A8", &RemoteDspQId);
    }while(status != 0);

    int fd_gpio;
    char ch;
    int pressing, released;
    int channelNo = 0;
    int long_press_counter = 0;
    int long_pressed_trigger = 0;



#ifdef REDFLAG
    int ChInfoToDSP = 6;
#else
	int ChInfoToDSP = 0;
#endif

// + a0220402, add support carit board key
    fd_gpio = open("/sys/class/gpio/gpio3/value", O_RDONLY | O_NONBLOCK );

// - a0220402,
	if (fd_gpio < 0) {
		perror("\nliuxu, gpio/fd_open\n");
	}

	printf("\n\n\nliuxu, 10/18/2013, fd_gpio=%d\n\n\n", fd_gpio);

	

    
    //liuxu, 02/20/2014, ti photo_snapshot.
        ProcMgr_Handle               handle = NULL;
        unsigned char *hardFrameOutput0; 
        unsigned char *hardFrameOutput1;
        unsigned char *hardFrameOutput2; 
        unsigned char *hardFrameOutput3;//liuxu, 02/20/2014, 0x8ff00000.

        unsigned char *phyAddrToBeChecked;//liuxu, 02/20/2014, 0x8ff00000.


    	status = ProcMgr_open (&handle, 0);

    	printf("\nliuxu, ProcMgr_open successful, handle=0x%x\n", handle);


    	if (status < 0)
        {
            printf("\nliuxu, ProcMgr_open error, status=0x%x\n", status);
            while(1);
        }  

        #define NV12_FRAMESIZE 529920//(736*480*1.5)
    	ProcMgr_translateAddr (handle,
                       &hardFrameOutput0,
                       ProcMgr_AddrType_MasterUsrVirt,
                       (void *)M3_FRAMEBUFFER_PA_0,
                       ProcMgr_AddrType_SlaveVirt);
                       
        ProcMgr_translateAddr (handle,
                       &hardFrameOutput1,
                       ProcMgr_AddrType_MasterUsrVirt,
                       (void *)(M3_FRAMEBUFFER_PA_0 + NV12_FRAMESIZE),
                       ProcMgr_AddrType_SlaveVirt);
                       
        ProcMgr_translateAddr (handle,
                       &hardFrameOutput2,
                       ProcMgr_AddrType_MasterUsrVirt,
                       (void *)(M3_FRAMEBUFFER_PA_0 + NV12_FRAMESIZE*2),
                       ProcMgr_AddrType_SlaveVirt);
                       
        ProcMgr_translateAddr (handle,
                       &hardFrameOutput3,
                       ProcMgr_AddrType_MasterUsrVirt,
                       (void *)(M3_FRAMEBUFFER_PA_0 + NV12_FRAMESIZE*3),
                       ProcMgr_AddrType_SlaveVirt);

    	printf("\nliuxu, 02/20/2014, virtual addr of hardFrameOutput0 = 0x%x\n", hardFrameOutput0);
    	printf("\nliuxu, 02/20/2014, virtual addr of hardFrameOutput1 = 0x%x\n", hardFrameOutput1);
    	printf("\nliuxu, 02/20/2014, virtual addr of hardFrameOutput2 = 0x%x\n", hardFrameOutput2);
    	printf("\nliuxu, 02/20/2014, virtual addr of hardFrameOutput3 = 0x%x\n", hardFrameOutput3);


    	ProcMgr_translateAddr (handle,
                       &phyAddrToBeChecked,
                       ProcMgr_AddrType_MasterPhys,
                       (void *)(0xd5604800),
                       ProcMgr_AddrType_MasterKnlVirt);

        printf("\nliuxu, 05/08/2014, 2, phy addr of phyAddrToBeChecked = 0x%x\n", phyAddrToBeChecked);//liuxu, 05/08/2014, leverage procMgr to sniff phy addr of entry->flag, which in SR0.



        FILE * dumpfile0;
        FILE * dumpfile1;
        FILE * dumpfile2;
        FILE * dumpfile3;

        dumpfile0 = fopen("/media/mmcblk0p1/TI_top.yuv", "w");
//+ a0220402, R1.1 add protection for the case of the absence of SD card
	if( dumpfile0 == NULL ) {
		printf ("SD card is needed for snopshot for developing!\n");
	}
	else
	{ 
	
		dumpfile1 = fopen("/media/mmcblk0p1/TI_right.yuv", "w");
		dumpfile2 = fopen("/media/mmcblk0p1/TI_left.yuv", "w");
        dumpfile3 = fopen("/media/mmcblk0p1/TI_back.yuv", "w");

        time1_profile = IviGetTime();

        fwrite(hardFrameOutput0, 1, 736*480*1.5, dumpfile0);

        fwrite(hardFrameOutput1, 1, 736*480*1.5, dumpfile1);

        fwrite(hardFrameOutput2, 1, 736*480*1.5, dumpfile2);

        fwrite(hardFrameOutput3, 1, 736*480*1.5, dumpfile3);

        time2_profile = IviGetTime() - time1_profile;
        printf("\nliuxu, time per dump 4 files = %d ms!!\n\n\n", time2_profile);

        printf("\n\nliuxu, 222,02/20/2014, dump 4 files well.\n\n");

        fclose(dumpfile0);
        fclose(dumpfile1);
        fclose(dumpfile2);
        fclose(dumpfile3);

	system("sync");
}
//- a0220402
        
        //fileSize += fread (buffer_addr, 1, 57600, yuvfp) ;//liuxu, 11/30/2013, rgb888.
        
/*
        //Y of YUV420sp.
        for(ii2=0; ii2<160; ii2++)
            for(jj2=0; jj2<240; jj2++)
            {
                *(hardFrameOutput+736*160+ii2*736+240+jj2) = buffer_addr[ii2*240+jj2];
            }

        //UV of YUV420sp.
        for(ii2=0; ii2<80; ii2++)
            for(jj2=0; jj2<240; jj2++)
            {
                *(hardFrameOutput+736*480+736*80+ii2*736+240+jj2) = buffer_addr[240*160+ii2*240+jj2];
            }    
*/
    


    //printf("\nliuxu, start, timestamp=%d!!!!\n", IviGetTime());//liuxuliuxu.

        int bcfd = -1; 
        char bcdev_name[] = "/dev/bccatX";
        BCIO_package ioctl_var;
        bc_buf_params_t buf_param;
        bc_buf_ptr_t buf_pa;

        unsigned long buf_paddr[MAX_BUFFERS];
        char *buf_vaddr[MAX_BUFFERS] = { MAP_FAILED };
        char *frame = NULL;

        char *frameUV = NULL;
        
        int buf_size = 0;
        int c, idx, ret = -1;
        char opts[] = "pw:t:b:h";

        int   ii;
        int   frame_w, frame_h;
        int   min_w = 0, min_h = 0;;
        int   cp_offset = 0;

        struct timeval tvp, tv, tv0 = {0,0};
        unsigned long tdiff = 0;
        unsigned long fcount = 0;
	
        if (frame_init(&buf_param))
            return -1;

        bcdev_name[strlen(bcdev_name)-1] = '0' + bcdev_id;

        if ((bcfd = open(bcdev_name, O_RDWR|O_NDELAY)) == -1) {
            printf("ERROR: open %s failed\n", bcdev_name);
            goto err_ret;
        }

        frame_w = buf_param.width;
        frame_h = buf_param.height;

        if (min_w > 0 && !(min_w % 8))
            buf_param.width = min_w;

        if (min_h > 0)
            buf_param.height = min_h;

//printf("\n-->liuxu, 06/19/2014, frame_w=%d, frame_h=%d, min_w=%d, min_h=%d", frame_w, frame_h, min_w, min_h);
//736 480 0 0

        int check = 0;


        if ((check = ioctl(bcfd, BCIOREQ_BUFFERS, &buf_param)) != 0) {
            printf("ERROR: BCIOREQ_BUFFERS failed, check=%d\n", check);
            goto err_ret;
        }

        bc_buf_ptr_t buf_pa_init;

/*liuxu, 10/18/2013, channel 0*/
        buf_pa_init.pa = M3_FRAMEBUFFER_PA_0;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 0;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }

        buf_pa_init.pa = M3_FRAMEBUFFER_PA_1;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 1;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }

        buf_pa_init.pa = M3_FRAMEBUFFER_PA_2;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 2;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }

        buf_pa_init.pa = M3_FRAMEBUFFER_PA_3;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 3;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }

        buf_pa_init.pa = M3_FRAMEBUFFER_PA_4;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 4;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }

        buf_pa_init.pa = M3_FRAMEBUFFER_PA_5;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 5;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }

/*liuxu, 10/18/2013, channel 1*/
        buf_pa_init.pa = M3_FRAMEBUFFER_PA_0+FRAME_SIZE;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 0+InterChannelIndexOffset;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }

        buf_pa_init.pa = M3_FRAMEBUFFER_PA_1+FRAME_SIZE;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 1+InterChannelIndexOffset;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }

        buf_pa_init.pa = M3_FRAMEBUFFER_PA_2+FRAME_SIZE;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 2+InterChannelIndexOffset;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }

        buf_pa_init.pa = M3_FRAMEBUFFER_PA_3+FRAME_SIZE;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 3+InterChannelIndexOffset;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }

        buf_pa_init.pa = M3_FRAMEBUFFER_PA_4+FRAME_SIZE;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 4+InterChannelIndexOffset;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }

        buf_pa_init.pa = M3_FRAMEBUFFER_PA_5+FRAME_SIZE;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 5+InterChannelIndexOffset;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }

/*liuxu, 10/18/2013, channel 2*/
        buf_pa_init.pa = M3_FRAMEBUFFER_PA_0+FRAME_SIZE*2;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 0+InterChannelIndexOffset*2;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }

        buf_pa_init.pa = M3_FRAMEBUFFER_PA_1+FRAME_SIZE*2;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 1+InterChannelIndexOffset*2;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }

        buf_pa_init.pa = M3_FRAMEBUFFER_PA_2+FRAME_SIZE*2;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 2+InterChannelIndexOffset*2;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }

        buf_pa_init.pa = M3_FRAMEBUFFER_PA_3+FRAME_SIZE*2;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 3+InterChannelIndexOffset*2;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }

        buf_pa_init.pa = M3_FRAMEBUFFER_PA_4+FRAME_SIZE*2;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 4+InterChannelIndexOffset*2;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }

        buf_pa_init.pa = M3_FRAMEBUFFER_PA_5+FRAME_SIZE*2;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 5+InterChannelIndexOffset*2;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }

/*liuxu, 10/18/2013, channel 3*/
        buf_pa_init.pa = M3_FRAMEBUFFER_PA_0+FRAME_SIZE*3;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 0+InterChannelIndexOffset*3;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }

        buf_pa_init.pa = M3_FRAMEBUFFER_PA_1+FRAME_SIZE*3;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 1+InterChannelIndexOffset*3;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }

        buf_pa_init.pa = M3_FRAMEBUFFER_PA_2+FRAME_SIZE*3;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 2+InterChannelIndexOffset*3;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }

        buf_pa_init.pa = M3_FRAMEBUFFER_PA_3+FRAME_SIZE*3;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 3+InterChannelIndexOffset*3;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }

        buf_pa_init.pa = M3_FRAMEBUFFER_PA_4+FRAME_SIZE*3;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 4+InterChannelIndexOffset*3;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }

        buf_pa_init.pa = M3_FRAMEBUFFER_PA_5+FRAME_SIZE*3;//liuxu, 10/12/2013, this is the false one just for init.
        buf_pa_init.index = 5+InterChannelIndexOffset*3;
        if (ioctl(bcfd, BCIOSET_BUFFERPHYADDR, &buf_pa_init) != 0)
        {
            printf("\nliuxu, ERROR: BCIOSET_BUFFERPHYADDR error, %d, 0x%lx\n", buf_pa_init.index, buf_pa_init.pa);
            return -1;
        }


        if (initEGL(ioctl_var.output)) {
            printf("ERROR: init EGL failed\n");
            goto err_ret;
        }
		//printf("\n *************3D AVM bcdev_id = %d\n", bcdev_id);  
		//bcdev_id=0		
		//printf("\n *************3D AVM buf_info.n = %d, buf_info.w = %d, buf_info.h = %d, buf_info.fmt = %d\n", buf_info.n ,buf_info.w, buf_info.h, buf_info.fmt);		
		//0 0 0 0

        if ((ret = initTexExt(bcdev_id, &buf_info)) < 0) {
            printf("ERROR: initTexExt() failed [%d]\n", ret);
            goto err_ret;
        }

        if (buf_info.n > MAX_BUFFERS) {
            printf("ERROR: number of texture buffer exceeds the limit\n");
            goto err_ret;
        }

//printf("\n--> liuxu buf number 10/12/2013, buf_info.n=%d\n", buf_info.n); 
//24
//		render3DSRVObj.screen_width = FRAME_WIDTH;
//		render3DSRVObj.screen_height = FRAME_HEIGHT;	
//		status = SgxRender3DSRV_setup(&render3DSRVObj);


        /*FIXME calc stride instead of 2*/
        buf_size = buf_info.w * buf_info.h * 1.5;//liuxu, 9/7/2013. ?????
        min_w    = buf_info.w < frame_w ? buf_info.w : frame_w;
        min_h    = buf_info.h < frame_h ? buf_info.h : frame_h;

        if (buf_info.h > frame_h)
            cp_offset = (buf_info.h - frame_h) * buf_info.w;

        if (buf_info.w > frame_w)
            cp_offset += buf_info.w - frame_w;

printf("\n\n\n--> liuxu checkpoint, cp_offset=%d 10/12/2013\n\n\n", cp_offset);


        if (buf_param.type == BC_MEMORY_USERPTR) {
            ;//liuxu, 10/10/2013, space holder. 
        }
        else if (buf_param.type == BC_MEMORY_MMAP) {
            for (idx = 0; idx < buf_info.n; idx++) {
                ioctl_var.input = idx;

                if (ioctl(bcfd, BCIOGET_BUFFERPHYADDR, &ioctl_var) != 0) {
                    printf("ERROR: BCIOGET_BUFFERADDR failed\n");
                    goto err_ret;
                }


                buf_paddr[idx] = ioctl_var.output;
                buf_vaddr[idx] = (char *)mmap(NULL, buf_size,
                                  PROT_READ | PROT_WRITE, MAP_SHARED,
                                  bcfd, buf_paddr[idx]);

                if (buf_vaddr[idx] == MAP_FAILED) {
                    printf("ERROR: mmap failed\n");
                    goto err_ret;
                }

            }
        }

        ret = 0;
        idx = 0;


    int i_put = 0;
    int i_get = 0;

	//OVAL view point initial
	//camera position
    float eye_x = -50.0;  
    float eye_y = 0.0;
    float eye_z = 60.0;
    float angle_eye = PI*3/2;  
    float radious_eye = 50.0;
	//look direction
    float center_x = 15.0;
    float center_y = 0.0;
    float center_z = 3.0;
    float radious_cen = 15.0;
	float angle_cen = PI/2;    //angle_cen-PI
	//delay time
    int view_counter = 0;
	//camera normal line
    float normal_x = 0.0;
    float normal_y = 0.0;
    float normal_z = 1.0;

 	int ViewMode = 0;  //0: topview  1:circle view
 	int circleCount = 0; //circle要轉幾圈才換回topview

	//topview initial
	eye_x = 0.0;
	eye_y = 0.0;
	eye_z = 180.0;
	
	center_x = 0.0;
	center_y = 0.0;
	center_z = 0.0;

	
    do   //****************程式進入點*************
    {     

        time1_profile = IviGetTime();

        //printf("\nliuxu, before commandQGet");

		lseek(fd_gpio, 0, SEEK_SET);
        read(fd_gpio, &ch, 1);


        status = commandQGet(hMessageQ, &(pTempCmdMsg), (UInt32)MessageQ_FOREVER);


        if (status < 0)
        {
            printf("\nliuxu, commandQGet error, status=0x%x\n", status);//liuxuliuxu.
            while(1);
        }

        time5_profile = IviGetTime() - time1_profile;
        //printf("\nliuxu, input IPC get per frame = %d ms!!", time5_profile);


        time8_profile = IviGetTime();
        
        /* Get the id of the address if id is not already known. */
        nSRId = SharedRegion_getId(pTempCmdMsg);
        /* Get the shared region pointer for the address */
        srPtr = SharedRegion_getSRPtr(pTempCmdMsg, nSRId);
        /* Get the address back from the shared region pointer */
        pFrom_DSP_TempCmdMsg = SharedRegion_getPtr(srPtr);
        
        while(pFrom_DSP_TempCmdMsg->cmdType != 0x777)
        {
            printf("\nliuxu, pFrom_DSP_TempCmdMsg->cmdType != 0x777\n");
        }

        i_get++;

        
        time9_profile = IviGetTime() - time8_profile;
        //printf("\nliuxu, share region parser for ipc per frame = %d ms, iget=%d!!", time9_profile, i_get);

        //k++;
        //printf("\nliuxu, k=%d, pointerY=0x%x, pointerUV=0x%x\n", k, pFrom_DSP_TempCmdMsg->pPointer0, pFrom_DSP_TempCmdMsg->pPointer1);


        //frame = frame_get(&buf_pa);

        if(pFrom_DSP_TempCmdMsg->pY_Pointer0 == M3_FRAMEBUFFER_PA_0)
            idx = 0;
        else if(pFrom_DSP_TempCmdMsg->pY_Pointer0 == M3_FRAMEBUFFER_PA_1)
            idx = 1;
        else if(pFrom_DSP_TempCmdMsg->pY_Pointer0 == M3_FRAMEBUFFER_PA_2)
            idx = 2;
        else if(pFrom_DSP_TempCmdMsg->pY_Pointer0 == M3_FRAMEBUFFER_PA_3)
            idx = 3;
        else if(pFrom_DSP_TempCmdMsg->pY_Pointer0 == M3_FRAMEBUFFER_PA_4)
            idx = 4;
        else if(pFrom_DSP_TempCmdMsg->pY_Pointer0 == M3_FRAMEBUFFER_PA_5)
            idx = 5;
        else
            idx = 0;;//liuxu, 06/19/2014, here is a RISK...//liuxu, 06/05/2014, test for PAL cam..//printf("\n\nliuxu, error!!!!\n\n");

		//printf("\r\n **************idx:%d************** \n", idx);
		//會一直輪循 0->1->2->3->4->5->0

        if (buf_param.type == BC_MEMORY_USERPTR) 
        {
            //printf("\nliuxu, 10/10/2013, good, idx=%d!!!\n", idx);
        }

        if (ch == '0') 
    	{
    		pressing = 1;
    		long_press_counter++;
    		long_pressed_trigger = 0;
    	} 
    	else if (pressing == 1)
    	{
    		 released = 1;
    		 if(long_press_counter > 30*5)//liuxu, 06/03/2014, 5s for long press.
    		 {
    		    long_press_counter = 0;
    		    long_pressed_trigger = 1;
    		 }
    	}
    	else
    	{
    	    pressing = 0;
    	    released = 0;
    	    long_press_counter = 0;
    	    long_pressed_trigger = 0;
    	}

/* 強制3D AVM
		if (Enforcement3DAVM==1)
		{
			pressing = 1;
			released = 1;
			Enforcement3DAVM = 0;
		}
*/
    	//printf("\nliuxu, 6/3/2014, ch=%c, pressing=%d, released=%d!!!\n", ch, pressing, released);
        if(long_pressed_trigger == 1)
        {
            pressing = 0;
    	    released = 0;
    	    long_press_counter = 0;
    	    long_pressed_trigger = 0;

    	    printf("\nliuxu, 06/03/2014, long pressed triggered\n");
        
        }
    	else if((pressing == 1) && (released == 1))//liuxu, 10/18/2013, bug to improve, when rapidly press and release, the behavior may depends by chance.
    	{
    	    printf("\r\n\n\n\n\n\n\nliuxu, 10/18/2013, a normal key detected!!!\n\n");
    	    pressing = 0;
    	    released = 0;

    	    channelNo++;
    	    channelNo = channelNo%4;

			printf("\r\n **************channelNo:%d************** \n", channelNo);
			// 哪一路影像
			
			
			ChInfoToDSP++;
			ChInfoToDSP = ChInfoToDSP%7;//liuxu, 06/20/2014, add two cases for shuffling grx and video pipeline.//liuxu, 06/19/2014, for 5 layouts pattern of DSP and M3.

		#ifdef REDFLAG
			if (ChInfoToDSP == 1)  //skip mode 1-4
			{ChInfoToDSP = 5;}
		#endif
			  
			printf("\r\n **************ChInfoToDSP:%d************** \n", ChInfoToDSP);
			//0:  2D AVM  母 + 子
			//1:  2D AVM  子 + 母
			//2:  2D AVM  母 + 子
			//3:  單一全  
			//4:  四分割
			//5:  3D Cube (顯示channelNo)
			//6:  2D AVM  母 + 子


		//	ChInfoToDSP = 5; //永遠顯示3D AVM  Enforcement3DAVM=1
    	    if(ChInfoToDSP == 5)
    	    {
    	        system("echo 1,0/0/0/0 > /sys/devices/platform/vpss/display2/order");//liuxu, 06/20/2014, grx0 up.
    	    }
    	    else if (ChInfoToDSP == 6)
    	    {
    	        system("echo 1,3/0/0/0 > /sys/devices/platform/vpss/display2/order");//liuxu, 06/20/2014, video up.
    	    }

    	    //liuxu, 02/19/2014, add key pad function.
    	    if(channelNo == 1)
    	    {
                //system("echo 1,3/0/0/0 > /sys/devices/platform/vpss/display2/order");//liuxu, 06/03/2014, video pipeline on the top.
    	        //system("echo 0 > /sys/devices/platform/vpss/graphics0/enabled");
    	        //liuxu, 06/03/2014, no grx2 now. system("echo 0 > /sys/devices/platform/vpss/graphics2/enabled");
    	    }
    	    else if (channelNo == 2)
    	    {
    	        //system("echo 1,0/0/0/0 > /sys/devices/platform/vpss/display2/order");//liuxu, 06/03/2014, grx0 pipeline on the top.
    	        //system("echo 1 > /sys/devices/platform/vpss/graphics0/enabled");
    	    }
    	    else if (channelNo == 3)
    	    {
    	        //system("echo 1 > /sys/devices/platform/vpss/graphics2/enabled");
    	    }
    	}
    	//printf("\nliuxu, after drawCube, ch=%d, pressing=%d, released=%d", ch, pressing, released);

        time3_profile = IviGetTime();



		// 3D mode
		if(ChInfoToDSP == 5)
		{

			//texChYuv 編號 CH0(前):0-5  CH1(左):6-11  CH2(右):12-17  CH3(後):18-23  
			texChYuv[0] = idx + 0 * InterChannelIndexOffset;  //前
			texChYuv[1] = idx + 1 * InterChannelIndexOffset;  //左
			texChYuv[2] = idx + 2 * InterChannelIndexOffset;  //右
			texChYuv[3] = idx + 3 * InterChannelIndexOffset;  //後



		
			//畫3D AVM	
	        camera_LookAt(eye_x, eye_y, eye_z, center_x, center_y, center_z, normal_x, normal_y, normal_z, &aMVP[0]); 
			car_LookAt(eye_x, eye_y, eye_z, center_x, center_y, center_z, normal_x, normal_y, normal_z, &carMVP[0]);
	        avm_drawtexture_VBO(texChYuv[0],texChYuv[2],texChYuv[1] ,texChYuv[3]); //放置位置前 右 左 後
	        car_drawtexture_VBO();

			eglSwapBuffers(dpy, surface);
	 

			if (ViewMode == 0)
			{
				if (eye_z<100)
				{
					ViewMode = 1;
					//OVAL view point initial
					//camera position
					eye_x = -50.0;  
					eye_y = 0.0;
					eye_z = 60.0;
					angle_eye = PI*3/2;  
					//look direction
					center_x = 15.0;
					center_y = 0.0;
					center_z = 3.0;
					angle_cen = PI/2;    //angle_cen-PI
					//camera normal line
					normal_x = 0.0;
					normal_y = 0.0;
					normal_z = 1.0;
				}
				else
				{
					eye_z = eye_z - 0.3;
					angle_eye = angle_eye + PI/180;
					normal_x = cos(angle_eye);
					normal_y = sin(angle_eye);
				}
			}
			else
			{
				if (circleCount > 3)
				{
					ViewMode = 0;
					eye_x = 0.0;
					eye_y = 0.0;
					eye_z = 180.0;
					
					center_x = 0.0;
					center_y = 0.0;
					center_z = 0.0;
					circleCount = 0;
				}
				else
				{
					//角度旋轉
					view_Angle_Change(&angle_eye, &angle_cen, &view_counter, &circleCount);
					eye_x = radious_eye * cos(angle_eye);
			        eye_y = radious_eye * sin(angle_eye);
					center_x = radious_cen * cos(angle_cen);
			        center_y = radious_cen * sin(angle_cen);	
				}
			}	
		}



        time4_profile = IviGetTime() - time3_profile;
       // printf("\nliuxu, drawCube time per frame = %d ms!!", time4_profile);
        

//liuxu, 11/19/2013, ACK to DSP. 
        time6_profile = IviGetTime();

        cmdMsg->cmdType = (0x666 << 16 ) | ChInfoToDSP;//liuxu, 06/19/2014, pass info to DSP then M3 through DSP.//liuxu, 8/20/2013, ACK only.

        status = commandQPut(RemoteDspQId, (MessageQ_Msg )cmdMsg);

        if (status < 0)
        {
            printf("\nliuxu, commandQPut error, status=0x%x\n", status);//liuxuliuxu.
            while(1);
        }

        i_put++;

        time7_profile = IviGetTime() - time6_profile;
        //printf("\nliuxu, output IPC put per frame = %d ms, iput=%d!!", time7_profile, i_put);


        time2_profile = IviGetTime() - time1_profile;
        //liuxu, 06/16/2014, printf("\nliuxu, total time per frame = %d ms!!\n\n\n", time2_profile);

        //usleep(20000);//liuxu, 12/23/2013, usleep 20ms to yield to mpeg4 encoder thread. 

     
    }while(1);

leave: 
    printf("<-- Main_main leave:\n");//liuxu, 8/19/2013.

#ifdef MPEG4_ENCODER
     pthread_join (thread_id, NULL);//liuxu, 12/19/2013.
#endif
    
    status = (status >= 0 ? 0 : status);
    return (status);
   
err_ret:
        if (buf_param.type == BC_MEMORY_MMAP) {
            for (idx = 0; idx < buf_info.n; idx++) {
                if (buf_vaddr[idx] != MAP_FAILED)
                    munmap(buf_vaddr[idx], buf_size);
            }
        }
        if (bcfd > -1)
            close(bcfd);

        deInitEGL(buf_info.n);
        frame_cleanup();

        close(fd_gpio);

        printf("liuxu, CUBE done\n");
        return ret;

}


/*
 *  ======== Main_parseArgs ========
 */
Int Main_parseArgs(Int argc, Char *argv[])
{
    Int             x, cp, opt, argNum;
    UInt16          i, numProcs;
    String          name;
    Int             status = 0;


    /* parse the command line options */
    for (opt = 1; (opt < argc) && (argv[opt][0] == '-'); opt++) {
        for (x = 0, cp = 1; argv[opt][cp] != '\0'; cp++) {
            x = (x << 8) | (int)argv[opt][cp];
        }

        switch (x) {
            case 'h': /* -h */
                printf("%s", Main_USAGE);
                exit(0);
                break;

            case 'l': /* -l */
                printf("Processor List\n");
                SysLink_setup();
                numProcs = MultiProc_getNumProcessors();
                for (i = 0; i < numProcs; i++) {
                    name = MultiProc_getName(i);
                    printf("    procId=%d, procName=%s\n", i, name);
                }
                SysLink_destroy();
                exit(0);
                break;

            default:
                printf(
                    "Error: %s, line %d: invalid option, %c\n",
                    __FILE__, __LINE__, (Char)x);
                printf("%s", Main_USAGE);
                status = -1;
                goto leave;
        }
    }

    /* parse the command line arguments */
    for (argNum = 1; opt < argc; argNum++, opt++) {

        switch (argNum) {
            case 1: /* name of proc #1 */
                Main_remoteProcName = argv[opt];
                break;

            default:
                printf(
                    "Error: %s, line %d: too many arguments\n",
                    __FILE__, __LINE__);
                printf("%s", Main_USAGE);
                status = -1;
                goto leave;
        }
    }

    /* validate command line arguments */
    if (Main_remoteProcName == NULL) {
        printf(
            "Error: %s, line %d: missing argument: proc\n",
            __FILE__, __LINE__);
        printf("%s", Main_USAGE);
        status = -1;
        goto leave;
    }

leave:
    return(status);
}



void avm_drawtexture_VBO(int bufferfront,int bufferright,int bufferleft,int bufferback)
{
    int iXangle = 0, iYangle = 0, iZangle = 0;
    float aRotate[16], aModelView[16], aPerspective[16];

/*
    rotate_matrix(iXangle, 1.0, 0.0, 0.0, aModelView);
    rotate_matrix(iYangle, 0.0, 1.0, 0.0, aRotate);

    multiply_matrix(aRotate, aModelView, aModelView);

    rotate_matrix(iZangle, 0.0, 1.0, 0.0, aRotate);

    multiply_matrix(aRotate, aModelView, aModelView);


    aModelView[14] -= 40;//2.5

    perspective_matrix(90.0, (double)uiWidth/(double)uiHeight, 0.01, 200.0, aPerspective);
    multiply_matrix(aPerspective, aModelView, aMVP);*/


    //camera_LookAt(50.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, &aMVP[0]); //不改視點



    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


    avm_texture_restore_vbo(&bottom_front, bufferfront);
    glUniformMatrix4fv(bottom_front.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, BFBT_index_size, GL_UNSIGNED_SHORT,  0);

    avm_texture_restore_vbo(&Bfront_left, bufferfront);
    glUniformMatrix4fv(Bfront_left.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, BCT_index_size, GL_UNSIGNED_SHORT,  0);

    avm_texture_restore_vbo(&Bfront_right, bufferfront);
    glUniformMatrix4fv(Bfront_left.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, BCT_index_size, GL_UNSIGNED_SHORT,  0);

    avm_texture_restore_vbo(&bottom_back, bufferback);
    glUniformMatrix4fv(bottom_back.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, BFBT_index_size, GL_UNSIGNED_SHORT,  0);

    avm_texture_restore_vbo(&Bback_left, bufferback);
    glUniformMatrix4fv(Bback_left.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, BCT_index_size, GL_UNSIGNED_SHORT,  0);

    avm_texture_restore_vbo(&Bback_right, bufferback);
    glUniformMatrix4fv(Bback_right.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, BCT_index_size, GL_UNSIGNED_SHORT,  0);

    avm_texture_restore_vbo(&bottom_left, bufferleft);
    glUniformMatrix4fv(bottom_left.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, BLRT_index_size, GL_UNSIGNED_SHORT,  0);

    avm_texture_restore_vbo(&bottom_right, bufferright);
    glUniformMatrix4fv(bottom_right.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, BLRT_index_size, GL_UNSIGNED_SHORT,  0);


    avm_texture_restore_vbo(&top_right, bufferright);
    glUniformMatrix4fv(top_right.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, TLRT_index_size, GL_UNSIGNED_SHORT,  0);

    avm_texture_restore_vbo(&top_left, bufferleft);
    glUniformMatrix4fv(top_left.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, TLRT_index_size, GL_UNSIGNED_SHORT,  0);

    avm_texture_restore_vbo(&top_front, bufferfront);
    glUniformMatrix4fv(top_front.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, TFBT_index_size, GL_UNSIGNED_SHORT,  0);


    avm_texture_restore_vbo(&Tfront_left, bufferfront);
    glUniformMatrix4fv(Tfront_left.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, TCT_index_size, GL_UNSIGNED_SHORT,  0);

    avm_texture_restore_vbo(&Tfront_right, bufferfront);
    glUniformMatrix4fv(Tfront_right.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, TCT_index_size, GL_UNSIGNED_SHORT,  0);


    avm_texture_restore_vbo(&top_back, bufferback);
    glUniformMatrix4fv(top_back.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, TFBT_index_size, GL_UNSIGNED_SHORT,  0);

    avm_texture_restore_vbo(&Tback_left, bufferback);
    glUniformMatrix4fv(Tback_left.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, TCT_index_size, GL_UNSIGNED_SHORT,  0);

    avm_texture_restore_vbo(&Tback_right, bufferback);
    glUniformMatrix4fv(Tback_right.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, TCT_index_size, GL_UNSIGNED_SHORT,  0);


    //****************************GL_BLEND start**************************************
	 glDisable(GL_DEPTH_TEST);
	 glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	avm_overlap_restore_vbo(&Bright_up, bufferright);
    glUniformMatrix4fv(Bright_up.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, BCT_index_size, GL_UNSIGNED_SHORT,  0);

	avm_overlap_restore_vbo(&Bright_down, bufferright);
    glUniformMatrix4fv(Bright_down.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, BCT_index_size, GL_UNSIGNED_SHORT,  0);

    avm_overlap_restore_vbo(&Bleft_up, bufferleft);
    glUniformMatrix4fv(Bleft_up.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, BCT_index_size, GL_UNSIGNED_SHORT,  0);

	avm_overlap_restore_vbo(&Bleft_down, bufferleft);
    glUniformMatrix4fv(Bleft_down.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, BCT_index_size, GL_UNSIGNED_SHORT,  0);

    avm_overlap_restore_vbo(&Tright_up, bufferright);
    glUniformMatrix4fv(Tright_up.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, TCT_index_size, GL_UNSIGNED_SHORT,  0);

    avm_overlap_restore_vbo(&Tright_down, bufferright);
    glUniformMatrix4fv(Tright_down.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, TCT_index_size, GL_UNSIGNED_SHORT,  0);

    avm_overlap_restore_vbo(&Tleft_up, bufferleft);
    glUniformMatrix4fv(Tleft_up.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, TCT_index_size, GL_UNSIGNED_SHORT,  0);

    avm_overlap_restore_vbo(&Tleft_down, bufferleft);
    glUniformMatrix4fv(Tleft_down.mvMatrixOffsetLoc, 1, GL_FALSE, aMVP);
    glDrawElements(GL_TRIANGLES, TCT_index_size, GL_UNSIGNED_SHORT,  0);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);


    //****************************GL_BLEND end**************************************

	
}




void car_drawtexture_VBO()
{
    //float aRotate[16], aModelView[16], aPerspective[16];

#ifdef REDFLAG

	avm_car_texture_restore_vbo(&jeep_car, jeep_car.textureID);
	glUniformMatrix4fv(jeep_car.mvMatrixOffsetLoc, 1, GL_FALSE, carMVP);
	glDrawElements(GL_TRIANGLES, jeep_car.car_texture_size, GL_UNSIGNED_SHORT,  0);

	avm_car_texture_restore_vbo(&jeep_carWH, jeep_carWH.textureID);
	glUniformMatrix4fv(jeep_carWH.mvMatrixOffsetLoc, 1, GL_FALSE, carMVP);
	glDrawElements(GL_TRIANGLES, jeep_carWH.car_texture_size, GL_UNSIGNED_SHORT,	0);

	avm_car_texture_restore_vbo(&jeep_carGL, jeep_carGL.textureID);
	glUniformMatrix4fv(jeep_carGL.mvMatrixOffsetLoc, 1, GL_FALSE, carMVP);
	glDrawElements(GL_TRIANGLES, jeep_carGL.car_texture_size, GL_UNSIGNED_SHORT,	0);


#else
	avm_car_texture_restore_vbo(&jeep_car, jeep_car.textureID);
    glUniformMatrix4fv(jeep_car.mvMatrixOffsetLoc, 1, GL_FALSE, carMVP);
	glClear(GL_DEPTH_BUFFER_BIT);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES, jeep_car.car_texture_size, GL_UNSIGNED_SHORT,  0);
	
#endif



	
}




void view_Angle_Change(float *angle_e, float *angle_c, int *view_count, int *circleC)
{
	float rotateSpeed =  PI/180;
	int stoptime = 200;  //1s =20 fps
	
	//avoid error and value overflow
	if (*angle_e > 2*PI)
	{
		*angle_e = *angle_e - 2*PI;
		*circleC = circleC + 1;
	}
	if (*angle_c > 2*PI)
	{
		*angle_c = *angle_c - 2*PI;
	}

	//                                                                                                                   ^
	//set delay time & stop angle   順時針旋轉  0度:-->   ;90度:|    ;180度:<--   ; 270度: |
	//									       		   V
#ifdef REDFLAG	
	if(0)
	{

	}
#else
	if ((*angle_e > 0) && (*angle_e < 0+rotateSpeed))
	{
		*view_count = *view_count + 1;
		if (*view_count == stoptime)
		{
			*angle_e = *angle_e + rotateSpeed;
			*angle_c = *angle_c + rotateSpeed;
			*view_count = 0;
		}
	}
#endif
#ifdef REDFLAG	
	else if ((*angle_e > PI*1/4) && (*angle_e < PI*1/4+rotateSpeed))
	{
		*view_count = *view_count + 1;
		if (*view_count == stoptime)
		{
			*angle_e = *angle_e + rotateSpeed;
			*angle_c = *angle_c + rotateSpeed;
			*view_count = 0;
		}
	}	
#endif
	else if ((*angle_e > PI*2/4) && (*angle_e < PI*2/4+rotateSpeed))
	{
		*view_count = *view_count + 1;
		if (*view_count == stoptime)
		{
			*angle_e = *angle_e + rotateSpeed;
			*angle_c = *angle_c + rotateSpeed;
			*view_count = 0;
		}
	}
#ifdef REDFLAG	
	else if ((*angle_e > PI*3/4) && (*angle_e < PI*3/4+rotateSpeed))
	{
		*view_count = *view_count + 1;
		if (*view_count == stoptime)
		{
			*angle_e = *angle_e + rotateSpeed;
			*angle_c = *angle_c + rotateSpeed;
			*view_count = 0;
		}
	}	
#endif	
#ifdef REDFLAG
#else
	else if ((*angle_e > PI*4/4) && (*angle_e < PI*4/4+rotateSpeed))
	{
		*view_count = *view_count + 1;
		if (*view_count == stoptime)
		{
			*angle_e = *angle_e + rotateSpeed;
			*angle_c = *angle_c + rotateSpeed;
			*view_count = 0;
		}
	}
	else if ((*angle_e > PI*5/4) && (*angle_e < PI*5/4+rotateSpeed))
	{
		*view_count = *view_count + 1;
		if (*view_count == stoptime)
		{
			*angle_e = *angle_e + rotateSpeed;
			*angle_c = *angle_c + rotateSpeed;
			*view_count = 0;
		}
	}
#endif
	else if ((*angle_e > PI*6/4) && (*angle_e < PI*6/4+rotateSpeed))
	{
		*view_count = *view_count + 1;
		if (*view_count == stoptime)
		{
			*angle_e = *angle_e + rotateSpeed;
			*angle_c = *angle_c + rotateSpeed;
			*view_count = 0;
		}
	}
#ifdef REDFLAG
#else
	else if ((*angle_e > PI*7/4) && (*angle_e < PI*7/4+rotateSpeed))
	{
		*view_count = *view_count + 1;
		if (*view_count == stoptime)
		{
			*angle_e = *angle_e + rotateSpeed;
			*angle_c = *angle_c + rotateSpeed;
			*view_count = 0;
		}
	}
#endif
	else
	{
		*angle_e = *angle_e + rotateSpeed;
		*angle_c = *angle_c + rotateSpeed;
	}

}

