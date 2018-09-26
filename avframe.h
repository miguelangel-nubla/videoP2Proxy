#ifndef AVFRAME_H_
#define AVFRAME_H_

typedef struct {
	int backwardIndex;
	int backwardIndex2;
	char cam_index;
	short codec_id;
	int definition;
	char flags;
	int frmSize;
	char isLiveflages;
	long millisecond;
	char onlineNum;
	long time;
	long timestamp;
	int videoHeight;
	int videoWidth;
	char* frmData;
} AVFrame;

int isLive(const AVFrame *avFrame);
int isIFrame(const AVFrame *avFrame);
void getLog(const AVFrame *avFrame);
AVFrame readAvFrame(char *frameInfo,char *videoBuffer,int *frmSize);
unsigned long byteArrayToLong_Little(char *frameInfo,int pos);
unsigned int byteArrayToInt_Little(char *frameInfo,int pos);
short byteArrayToShort_Little(char *frameInfo,int pos);

#endif