#ifndef IOTC_H_
#define IOTC_H_

int enterIOTC(int *avIndex, char *p2p_id, char *userName, char *passWord);
int quitIOTC(int *avIndex, int *SID);
int startIOTC(int *avIndex);
int stopIOTC(int *avIndex);

#endif