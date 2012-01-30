#! /bin/sh
BASEDIR=$(dirname $0)
cd $BASEDIR
cd ./images/
../bin/perl ../build-playlist images . playlist
cd ..
XBEEDEV=/dev/tty.usbserial-FTF61JDO
export XBEEDEV
XBEESPEED=57600
export XBEESPEED
./dokermit images/images.wad
