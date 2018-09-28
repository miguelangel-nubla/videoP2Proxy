/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * Copyright (C) 2018 Miguel Angel Nubla <miguelangel.nubla@gmail.com>
 */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "config.h"
#include "common.h"
#include "client.h"
#include "rtsp.h"

void print_usage() {
    printf(
    		"Usage: videop2proxy --ip CAMERA_IP --token CAMERA_HEX_TOKEN [...] \n"
    		"\n"
    		"Options:\n"
    		"  --ip IP             [Required] Camera ip address.\n"
    		"  --token HEX_TOKEN   [Required] Camera miio token.\n"
			"\n"
    		"Modes:\n"
#ifdef ENABLE_RTSP
			"  --rtsp PORT         Enable RTSP server.\n"
#endif
    		"  --stdout            Enable output to stdout.\n"
    );
}

int tryConnect(char* ip, char* token) {
	char cmd[256];
	snprintf(cmd, sizeof cmd,
		"python3 -c \""
		"import miio;"
		"result = miio.device.Device('%s', '%s').send('get_ipcprop', ['all']);"
		"print(result['p2p_id']);"
		"print(result['avID']);"
		"print(result['avPass']);"
        "\""
	, ip, token);

	FILE *fp;
	char line[1035];
	fp = popen(cmd, "r");

	if (fp == NULL) {
		DPRINTF("Failed to run command: %s\n", cmd);
		return 1;
	}

	char p2p_id[1035];
	char username[1035];
	char password[1035];
	for(int i = 1; i <= 3; i = i + 1 ){
		if (fgets(line, sizeof(line), fp) == NULL) {
			DPRINTF("Can't read output line %d from command: %s\n", i, cmd);
			DPRINTF("Error connecting to camera, make sure ip and token are correct.\n");
			return 1;
		}
		line[strcspn(line, "\n")] = 0;
		switch(i) {
			case 1:
				strncpy(p2p_id, line, sizeof(line));
				break;
			case 2:
				strncpy(username, line, sizeof(line));
				break;
			case 3:
				strncpy(password, line, sizeof(line));
				break;
		}
	}
	pclose(fp);

	return clientRun(p2p_id, username, password);
}

int main(int argc, char *argv[]) {
    char *ip = "", *token = "";
	MODE_RTSP = -1, MODE_STDOUT = -1;

    static struct option long_options[] = {
        {"ip",     required_argument, 0,  'i' },
        {"token",  required_argument, 0,  't' },
		#ifdef ENABLE_RTSP
		{"rtsp",   required_argument, 0,  'r' },
		#endif
		{"stdout", no_argument,       0,  's' },
        {0,        0,                 0,  0   }
    };

	int opt= 0;
    int long_index =0;
    while ((opt = getopt_long(argc, argv,"i:t:rs",
                   long_options, &long_index )) != -1) {
        switch (opt) {
             case 'i' : ip = optarg;
                 break;
             case 't' : token = optarg;
                 break;
             case 'r' : MODE_RTSP = atoi(optarg);
                 break;
             case 's' : MODE_STDOUT = 1;
                 break;
             default: print_usage();
                 exit(EXIT_FAILURE);
        }
    }
    if (MODE_RTSP < 0 && MODE_STDOUT < 0) {
    	printf("ERROR: You must specify at least one mode.\n");
        print_usage();
        exit(EXIT_FAILURE);
    }


	#ifdef ENABLE_RTSP
	if (MODE_RTSP >= 0)
	{
		char template[] = "/tmp/videop2proxy.XXXXXX";
		char* tmpDir = mkdtemp(template);
		char* tmpFile = "/fifo";

		MODE_RTSP_FIFO_FILE = malloc(strlen(tmpDir) + strlen(tmpDir) + 1);
		strcat(MODE_RTSP_FIFO_FILE, tmpDir);
		strcat(MODE_RTSP_FIFO_FILE, tmpFile);

		mkfifo(MODE_RTSP_FIFO_FILE, 0600);

		pthread_t ThreadRTSP = 0;
		int ret;
		if ((ret=pthread_create(&ThreadRTSP, NULL, &min, NULL)))
		{
			DPRINTF("Create RTSP thread failed\n");
			return 1;
		}
		MODE_RTSP_FIFO = open(MODE_RTSP_FIFO_FILE, O_WRONLY);
	}
	#endif

	DPRINTF("Starting proxy...\n");
	int delay = 10;
    while (1)
	{
		tryConnect(ip, token);
		DPRINTF("Error, waiting %d seconds and trying again.\n", delay);
		sleep(delay);
	}
}

