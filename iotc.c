#include <json-c/json_object.h>
#include <stdio.h>

#include "common.h"

#include "include/IOTCAPIs.h"
#include "include/AVAPIs.h"
#include "include/AVIOCTRLDEFs.h"

int enterIOTC(int *avIndex, char *p2p_id, char *userName, char *passWord) {
    DPRINTF("IOTC_Initialize2...\n");
    int mRet = IOTC_Initialize2(0);

    if (mRet == -3) {
        mRet = 0;
    }

    if (mRet != 0) {
        DPRINTF("IOTC_Initialize2 ERROR %d\n", mRet);
        return 0;
    }

    mRet = avInitialize(3);
    if (mRet < 0) {
        DPRINTF("avInitialize ERROR %d\n", mRet);
        return 0;
    }
    int SID = IOTC_Get_SessionID();
    if (SID < 0) {
        DPRINTF("IOTC_Get_SessionID ERROR %d\n", SID);
        return 0;
    }

    mRet = IOTC_Connect_ByUID_Parallel(p2p_id, SID);
    if (mRet < 0) {
        DPRINTF("IOTC_Connect_ByUID_Parallel ERROR %d\n", SID);
        return 0;
    }
    DPRINTF("IOTC_Connect_ByUID_Parallel... SID:%d\n", SID);

    unsigned int srvType = 1;
    int bResend = 1;

    DPRINTF("avClientStart2... ");
    DPRINTF("userName: %s passWord: %s\n", userName, passWord);

    *avIndex = avClientStart2(SID, userName, passWord, 5000, &srvType, 0, &bResend);
    if (avIndex < 0) {
    	DPRINTF("avClientStart2 ERROR %d\n", *avIndex);
        return 0;
    }

	DPRINTF("avClientStart2 mAvIndex=%d\n", *avIndex);
    return 1;
}

int startIOTC(int *avIndex) {
    struct json_object *obj = json_object_new_object();
    json_object_object_add(obj, "channel", 0);
    const char *arr = json_object_to_json_string(obj);

    int ret = avSendIOCtrl(*avIndex, IOTYPE_USER_IPCAM_START, arr, sizeof(*arr));
    DPRINTF("startIOTC video ret=%d\n", ret);
    if (ret < 0) {
        return 0;
    }

    ret = avSendIOCtrl(*avIndex, IOTYPE_USER_IPCAM_AUDIOSTART, arr, sizeof(*arr));
    DPRINTF("startIOTC audio ret=%d\n", ret);
    if (ret < 0) {
        return 0;
    }

    return 1;
}


int stopIOTC(int *avIndex) {
    struct json_object *obj = json_object_new_object();
    json_object_object_add(obj, "channel", 0);
    const char *arr = json_object_to_json_string(obj);

    int ret = avSendIOCtrl(*avIndex, IOTYPE_USER_IPCAM_STOP, arr, sizeof(*arr));
    DPRINTF("stopIOTC video ret=%d\n", ret);
    if (ret < 0) {
        return 0;
    }
	
    ret = avSendIOCtrl(*avIndex, IOTYPE_USER_IPCAM_AUDIOSTOP, arr, sizeof(*arr));
    DPRINTF("stopIOTC audio ret=%d\n", ret);
    if (ret < 0) {
        return 0;
    }
	
    return 1;
}

int quitIOTC(int *avIndex, int *SID) {
    avClientStop(*avIndex);
    DPRINTF("avClientStop avIndex=%d\n", *avIndex);

    IOTC_Session_Close(*SID);
	DPRINTF("IOTC_Session_Close SID %d\n", *SID);

	return 1;
}