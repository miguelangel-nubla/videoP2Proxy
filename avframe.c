#include <string.h>
#include <stdio.h>

#include "common.h"
#include "avframe.h"

short byteArrayToShort_Little(char* frameInfo, int pos) {
	return (frameInfo[pos] & 255) | ((frameInfo[pos+1] & 255) << 8);
}

unsigned int byteArrayToInt_Little(char* frameInfo, int pos) {
	return (frameInfo[pos] & 255) | ((frameInfo[pos+1] & 255) << 8) | ((frameInfo[pos+2] & 255) << 16) | ((frameInfo[pos+3] & 255) << 24);
}

unsigned long byteArrayToLong_Little(char* frameInfo, int pos) {
	return (frameInfo[pos] & 255) | ((frameInfo[pos+1] & 255) << 8) | ((frameInfo[pos+2] & 255) << 16) | ((frameInfo[pos+3] & 255) << 24);
}

AVFrame readAvFrame(char *frameInfo, char *videoBuffer, int *frmSize)
{
	AVFrame f;
	f.codec_id = byteArrayToShort_Little(frameInfo, 0);
	f.flags = frameInfo[2];
	f.cam_index = frameInfo[3];
	f.onlineNum = frameInfo[4];
	f.isLiveflages = frameInfo[5];
	f.backwardIndex = frameInfo[6] & 255;
	f.backwardIndex2 = frameInfo[7] & 255;
	f.millisecond = byteArrayToLong_Little(frameInfo, 8);
	f.timestamp = byteArrayToLong_Little(frameInfo, 12);
	f.videoWidth = byteArrayToInt_Little(frameInfo, 16);
	f.videoHeight = byteArrayToInt_Little(frameInfo, 20);
	f.frmSize = *frmSize;
	f.frmData = videoBuffer;
	f.time = (f.timestamp * 1000) + f.millisecond;
	return f;
}


void getLog(const AVFrame *avFrame) {
	DPRINTF("flags=%d backwardIndex=%d timestamp=%ld.%ld isLiveflages=%d \n", avFrame->flags, avFrame->backwardIndex, avFrame->timestamp, avFrame->millisecond, avFrame->isLiveflages);
}

int isIFrame(const AVFrame *avFrame) {
	return avFrame->flags == 1;
}

int isLive(const AVFrame *avFrame) {
	if ((avFrame->isLiveflages & 255) < 150) {
		return 1;
	}
	return 0;
}
