#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "common.h"
#include "avframe.h"

#include "include/AVAPIs.h"
#include "include/IOTCAPIs.h"

int FRAME_INFO_SIZE = 64;
int VIDEO_BUF_SIZE = 400000;

extern void CArrayToGoArray();

AVFrame lastFrame = {
    .timestamp = 0,
    .millisecond = 0
};

void *thread_ReceiveVideo(void *arg)
{
	DPRINTF("[ReceiveVideo] Running\n");

	setvbuf(stdout, NULL, _IONBF, 0);

	int avIndex = *(int *)arg;

	for (;;)
	{
		//usleep((1000/30) * 1000);

		char videoBuffer[VIDEO_BUF_SIZE];
		char frameInfo[FRAME_INFO_SIZE];
		int outBufSize = 0;
		int outFrmSize = 0;
		int outFrmInfoSize = 0;
		unsigned int frmNo;

		int ret = avRecvFrameData2(avIndex, videoBuffer, VIDEO_BUF_SIZE, &outBufSize, &outFrmSize, frameInfo, FRAME_INFO_SIZE, &outFrmInfoSize, &frmNo);
		//DPRINTF("Video Receive thread loop avIndex=%d, ret=%d\n", avIndex, ret);

		if(ret == AV_ER_DATA_NOREADY)
		{
			//DPRINTF("AV_ER_DATA_NOREADY[%d]\n", avIndex);
			usleep(10 * 1000);
			continue;
		}
		else if(ret == AV_ER_LOSED_THIS_FRAME)
		{
			DPRINTF("Lost video frame NO[%d]\n", frmNo);
			continue;
		}
		else if(ret == AV_ER_INCOMPLETE_FRAME)
		{
			DPRINTF("Incomplete video frame NO[%d] ReadSize[%d] FrmSize[%d] FrmInfoSize[%u]\n", frmNo, outBufSize, outFrmSize, outFrmInfoSize);
		}
		else if(ret == AV_ER_SESSION_CLOSE_BY_REMOTE)
		{
			DPRINTF("AV_ER_SESSION_CLOSE_BY_REMOTE\n");
			break;
		}
		else if(ret == AV_ER_REMOTE_TIMEOUT_DISCONNECT)
		{
			DPRINTF("AV_ER_REMOTE_TIMEOUT_DISCONNECT\n");
			break;
		}
		else if(ret == IOTC_ER_INVALID_SID)
		{
			DPRINTF("Session cant be used anymore\n");
			break;
		}

		if (ret > 0)
		{
			AVFrame avFrame = readAvFrame(frameInfo, videoBuffer, &ret);

			if (avFrame.videoHeight > 0 && avFrame.videoWidth > 0) {
				// getLog(&avFrame);
				#ifdef GO_WRAPPER
				CArrayToGoArray(videoBuffer, ret);
				#endif



				#pragma GCC diagnostic push
				#pragma GCC diagnostic ignored "-Wunused-result"
				
				//unsigned char hexV[]={0x00,0x00,0x00,0x01,0x09,0x30};
				//write(1, hexV, 6);

				if (MODE_STDOUT == 1)
				{
					write(1, videoBuffer, ret);
				}

				#ifdef ENABLE_RTSP
				if (MODE_RTSP > 0)
				{
					write(MODE_RTSP_FIFO, videoBuffer, ret);
				}
				#endif

				#pragma GCC diagnostic pop
			}
		}
	}
	return 0;
}

void *thread_ReceiveAudio(void *arg)
{
	DPRINTF("[ReceiveAudio] Running\n");

	return 0;
	// @TODO audio
	for (;;)
	{
		// receive and broadcast
		//usleep(5 * 1000);
	}
	return 0;
}

int startReceive(int *avIndex) {
	pthread_t ThreadReceiveVideo_ID = 0, ThreadReceiveAudio_ID = 0;

	int ret;
	if ((ret=pthread_create(&ThreadReceiveVideo_ID, NULL, &thread_ReceiveVideo, avIndex)))
	{
		DPRINTF("Create Video Receive thread failed\n");
		return 1;
	}

	if ((ret=pthread_create(&ThreadReceiveAudio_ID, NULL, &thread_ReceiveAudio, avIndex)))
	{
		DPRINTF("Create Audio Receive thread failed\n");
		return 1;
	}

	if(ThreadReceiveVideo_ID!=0) pthread_join(ThreadReceiveVideo_ID, NULL);
	if(ThreadReceiveAudio_ID!=0) pthread_join(ThreadReceiveAudio_ID, NULL);

	return 0;
}