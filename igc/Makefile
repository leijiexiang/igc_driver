# SPDX-License-Identifier: GPL-2.0
# Copyright (c)  2018 Intel Corporation

#
# Intel(R) I225-LM/I225-V 2.5G Ethernet Controller
#

KVER ?= $(shell uname -r)
KDIR ?= /lib/modules/$(KVER)/build

igc-objs := igc_main.o igc_mac.o igc_i225.o igc_base.o igc_nvm.o igc_phy.o

obj-m += igc.o

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
