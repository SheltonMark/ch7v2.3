############################################################################
#
# Makefile for libmed
#
# (C) Copyright 2009 to 2013
#
# Sep 17, 2013		jiang_junhua		File created
#
# arm-uclibc-linux-gcc version 3.4.3
#
############################################################################
ifndef TOPDIR
	TOPDIR = ./
endif

#ifeq ($(strip $(cpu)),)
#        $(error "make cpu=cpu_type");
#endif
#ifeq ($(strip $(smart)),)
#        $(error "make =enable/disable");
#endif

ifndef SRCDIR
	SRCDIR = ../..
endif

#ifeq ($(strip $(cpu)),mc6830)
#SRC_COMMON = $(TOPDIR)/Src/common
#endif

SRCS += $(wildcard $(SRCDIR)/Src/audio/*.c)
SRCS += $(wildcard $(SRCDIR)/Src/capture/*.c)
SRCS += $(wildcard $(SRCDIR)/Src/media_com/*.c)
SRCS += $(wildcard $(SRCDIR)/Src/sdk_common/*.c)
SRCS += $(wildcard $(SRCDIR)/Src/detect/*.c)
SRCS +=$(wildcard $(SRCDIR)/Src/isp/*.c)
SRCS +=$(wildcard $(SRCDIR)/Src/video/*.c)
SRCS +=$(wildcard $(SRCDIR)/Src/video/adapter/*.c)
SRCS +=$(wildcard $(SRCDIR)/Src/video/adapter/ax/*.c)
SRCS +=$(wildcard $(SRCDIR)/Src/video/core/*.c)
SRCS +=$(wildcard $(SRCDIR)/Src/video/modules/*.c)

OBJS	= $(SRCS:%.c=%.o)


#LIBDVR = $(TOPDIR)/libmed.a
LIBDVR = ./libmed.a

#GIT_REVISION = $(shell git show -s --pretty=format:%h)
#$(info $(GIT_REVISION))


include $(TOPDIR)Rules.mak
