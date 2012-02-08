#!/bin/sh
BASEDIR=$(dirname $0)
#cd /
cd $BASEDIR
cd "$BASEDIR"/images/
#rm 0.img
perl ../build-playlist images . playlist
cd ..
for i in /dev/tty.usb*; do
		# if the file is there
	#filename=${i}
	#echo exporting $i to makefile
	XBEEDEV=$i
done
#XBEEDEV=/dev/tty.usbserial-FTF61JDO
echo "$XBEEDEV"
export XBEEDEV
XBEESPEED=57600
export XBEESPEED
./dokermit images/images.wad
