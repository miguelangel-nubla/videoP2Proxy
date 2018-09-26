## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(JSON_C_CFLAGS) \
	$(LIVE555_CFLAGS) \
	-Iinclude

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = videop2proxy

videop2proxy_SOURCES = \
	main.c \
	av.c \
	avframe.c \
	client.c \
	iotc.c \
	avframe.h \
	iotc.h \
	av.h \
	client.h \
	common.h \
	rtsp.cxx \
	rtsp.h

videop2proxy_LDFLAGS = \
	-Wl,-rpath $(libdir) \
	-Wl,-rpath $(P2P_LIBS)

videop2proxy_LDADD =  \
	-L$(P2P_LIBS) \
	$(JSON_C_LIBS) \
	$(LIVE555_LIBS) \
	-lIOTCAPIs \
	-lAVAPIs \
	-lpthread

p2pdir = $(libdir)
p2p_DATA = $(P2P_LIBS)/libIOTCAPIs.so \
    $(P2P_LIBS)/libAVAPIs.so

