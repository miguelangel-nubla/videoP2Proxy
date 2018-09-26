#include <fcntl.h>

#include "config.h"
#include "common.h"


#ifdef ENABLE_RTSP

#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"

UsageEnvironment* env;

Boolean reuseFirstSource = True;

Boolean iFramesOnly = False;

static void announceStream(RTSPServer* rtspServer, ServerMediaSession* sms,
                           char const* streamName, char const* inputFileName); // fwd

extern "C" void* min(void *) {
	TaskScheduler* scheduler = BasicTaskScheduler::createNew();
	env = BasicUsageEnvironment::createNew(*scheduler);

	UserAuthenticationDatabase* authDB = NULL;
#ifdef ACCESS_CONTROL
	authDB = new UserAuthenticationDatabase;
	authDB->addUserRecord("username1", "password1");
#endif

	RTSPServer* rtspServer = RTSPServer::createNew(*env, MODE_RTSP, authDB);
	if (rtspServer == NULL) {
		*env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
		exit(1);
	}

	char const* descriptionString
		= "Session streamed by " PACKAGE " (" PACKAGE_URL ")";

	{
		char const* streamName = "";

		int fd = open(MODE_RTSP_FIFO_FILE, O_RDONLY); // open inmediatelly to avoid hanging write process
		close(fd);
		
		char const* inputFileName = MODE_RTSP_FIFO_FILE;
		OutPacketBuffer::maxSize = 100000;
		ServerMediaSession* sms
			= ServerMediaSession::createNew(*env, streamName, streamName,
			                                descriptionString);
		sms->addSubsession(H264VideoFileServerMediaSubsession
		                   ::createNew(*env, inputFileName, reuseFirstSource));
		rtspServer->addServerMediaSession(sms);

		announceStream(rtspServer, sms, streamName, inputFileName);
	}

	if (rtspServer->setUpTunnelingOverHTTP(80) || rtspServer->setUpTunnelingOverHTTP(8000) || rtspServer->setUpTunnelingOverHTTP(8080)) {
		*env << "(Using port " << rtspServer->httpServerPortNum() << " for optional RTSP-over-HTTP tunneling.)\n";
	} else {
		*env << "(RTSP-over-HTTP tunneling is not available.)\n";
	}
	*env << "\n";

	env->taskScheduler().doEventLoop();
}

static void announceStream(RTSPServer* rtspServer, ServerMediaSession* sms,
                           char const* streamName, char const* inputFileName) {
	char* url = rtspServer->rtspURL(sms);
	UsageEnvironment& env = rtspServer->envir();
	env << "Starting RTSP server on \"" << url << "\"\n";
	delete[] url;
}
#endif
