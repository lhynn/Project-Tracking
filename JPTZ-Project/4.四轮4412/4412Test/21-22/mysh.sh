##!/bin/sh
arm-linux-gcc lcdfarme.c  mymfc.c SsbSipMfcDecAPI.c SsbSipMfcEncAPI.c v4l2.c -I./include -c
ar rc libpth.a *.o
arm-linux-g++ *.cpp libpth.a -o debug -I./include/
rm *.o
mv debug /mnt/hgfs/2.Share4or3/
