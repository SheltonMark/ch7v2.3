###########################################################################
#
# Makefile for libsys
#
# (C) Copyright 2009 to 2009 iCareVision Tech CO., LTD.
#
# NOV 20, wamg_danxing		wang_danxing		File created
# 
# arm-uclibc-linux-gcc version 3.4.3
#
############################################################################
TOPDIR = ../..

#CROSS	= arm-mol-linux-uclibcgnueabihf-
#CROSS	= arm-fullhanv3-linux-uclibcgnueabi-
CROSS	= arm-nia7ux2111v2e-linux-uclibcgnueabihf-
CC	= @echo " GCC	$@"; $(CROSS)gcc
CPP	= @echo " G++	$@"; $(CROSS)g++
LD	= @echo " LD	$@"; $(CROSS)ld
AR	= @echo " AR	$@"; $(CROSS)ar
RM	= @echo " RM	$@"; rm -f
CP	= @echo " CP    $(TARGET)"; cp -rf

CFLAGS   += -I$(TOPDIR)/Build/XS_7302/
CFLAGS   += -I$(TOPDIR)/Include/Private/Configs
CFLAGS	+= -I$(TOPDIR)/Include/Private/Common/
CFLAGS   += -I$(TOPDIR)/Include/APIs/


ifeq ($(cpu), 16dv300-sdk011)
	CFLAGS	+= -DCHIP_TYPE_hi3516dv300
	CFLAGS	+= -DHICHIP=HI3516D_V300

	CFLAGS	+= -DZNAPPLY

	CFLAGS  += -I$(TOPDIR)/sdk/include/hi3516dv300_sdk011/
	CFLAGS  += -I$(TOPDIR)/sdk/include/hi3516dv300_sdk011/adp/

	CFLAGS  += -fno-aggressive-loop-optimizations -ldl -ffunction-sections -fdata-sections -O2 
	CFLAGS  += -fno-aggressive-loop-optimizations
endif

ifeq ($(cpu), mc6830)
	CFLAGS  += -I$(TOPDIR)/sdk/mc6830/include/
	CFLAGS  += -I$(TOPDIR)/sdk/mc6830/include/mkp/
	CFLAGS  += -I$(SRC_COMMON)/
endif

ifeq ($(cpu), 8856v200)
	CFLAGS  += -I$(TOPDIR)/Inc/FH8852/
	CFLAGS  += -I$(TOPDIR)/Inc/FH8852/dsp/
	CFLAGS  += -I$(TOPDIR)/Inc/FH8852/dsp_ext
	CFLAGS  += -I$(TOPDIR)/Inc/FH8852/human_detect
	CFLAGS  += -I$(TOPDIR)/Inc/FH8852/isp
	CFLAGS  += -I$(TOPDIR)/Inc/FH8852/isp_ext
	CFLAGS  += -I$(TOPDIR)/Src/common/components
	CFLAGS  += -I$(TOPDIR)/Src/common/sample_common
	CFLAGS  += -DCONFIG_ARCH_FH885xV200
	CFLAGS  += -D__LINUX_OS__
#CFLAGS  += -DDEBUG_AVBR
endif

ifeq ($(cpu), XS7302)
	CFLAGS  += -I$(TOPDIR)/Include/Private/XS7302/
	CFLAGS  += -I$(TOPDIR)/Include/Private/XS7302/aisp/
	CFLAGS  += -I$(TOPDIR)/Src/sdk_common/
#CFLAGS  += -DDEBUG_AVBR
endif

ifeq ($(smart), enable)
	CFLAGS  += -D_IVS_
endif

#SVN Verinfo
#CFLAGS += -D BUILD_INFO='"\
#build user: $(shell whoami) \n \
#build version: $(shell svnversion ../../) \n \
#build url: $(shell svn info | grep '\bURL:\s\{1,\}\(http\|https\)://' | sed 's/URL:\s\(http\|https\):\/\/\(\([0-9]\)\{1,3\}.\)\{3\}[0-9]\{1,3\}/ /g')"'


#??????????,????
CFLAGS	+=	-DLIBMED_PRINT

#?????????,?????????????
CFLAGS +=      -DZERO_CHANNEL_CODE

CFLAGS  += -DLIBRARY_NAME=\"libmed\"


#音频AAC使能,需要关闭AUDIO_MODE_RESMP
#CFLAGS +=      -DAUDIO_ACC_ENABLE

#输出设备单画面切换成本地通道，多画面切换成网络通道
#CFLAGS +=      -DChn_SwitchMode

CFLAGS	+= -Wall -O2 -Os
#CFLAGS  += -g

LDFLAGS	+= -L$(TOPDIR)Lib

AFLAGS	+= -r

TARGET	= $(LIBDVR) $(EXEC)

all: $(TARGET)

$(LIBDVR): $(OBJS)
	$(RM) $@;
	$(AR) $(AFLAGS) $@ $^
	rm -f  $(OBJS) $(SRCDIR)/ivs/*.o $(SRCDIR)/XS_7302/*.o version.c;
	find ../../ -type f \( -name '*.d' \) | xargs rm -f
	

$(EXEC): $(OBJS) $(LIBS)
	$(CC) -o $@ $^ $(LDFLAGS)


.c.o:
	$(CC)  $(CFLAGS) -c -o $@ $<

.cpp.o:
	$(CPP) $(CFLAGS) -c -o $@ $<


#install:

#	$(CP) $(TARGET) $(CHALLENGEDIR)


clean:
	rm -f $(TARGET) $(OBJS) $(SRCDIR)/ivs/*.o $(SRCDIR)/FH_8852/*.o;
	find ../../ -type f \( -name '*.elf*' -o -name '*.gdb' -o -name '*.d' \) | xargs rm -f
