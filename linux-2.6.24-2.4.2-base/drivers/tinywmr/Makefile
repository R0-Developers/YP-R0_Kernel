#
#   Makefile for TINYWMR driver
#
#   Copyright(c) 2004-2006, Samsung Electronics, Co., Ltd.
#
WHIMORY_INC     = $(TOPDIR)/../tinyWhimory

EXTRA_CFLAGS	+= -I$(WHIMORY_INC)/FIL/ATJ213x -I$(WHIMORY_INC)/Inc -I$(WHIMORY_INC)/OAM/Linux -I$(WHIMORY_INC)/Core/VFL -I$(WHIMORY_INC)/Exam/

# For WMR 1.5 RC5 and below version
EXTRA_CFLAGS	+= -DSYNC_BURST_READ -D_WHIMORY_PORT_RFS_

include $(srctree)/drivers/tinywmr/Makefile.26

