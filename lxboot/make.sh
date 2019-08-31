#!/bin/bash
source $HOME/env
$MTKHOST-gcc \
	-fno-exceptions \
	-nostartfiles \
	-nostdlib \
	-c hello.c
$MTKHOST-ld -s -T hello.lds \
	hello.o \
	-o hello
$MTKHOST-objcopy -O binary hello hello.bin 