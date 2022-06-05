#!/bin/sh

echo Obtaining required packages...
apt-get -qq update
apt-get -qq install build-essential
apt-get -qq install libusb-1.0-0-dev
echo Copying source code files...
mkdir -p /usr/local/src/fau200/man
cp -f src/cp2130.cpp /usr/local/src/fau200/.
cp -f src/cp2130.h /usr/local/src/fau200/.
cp -f src/error.cpp /usr/local/src/fau200/.
cp -f src/error.h /usr/local/src/fau200/.
cp -f src/fau200-clear.cpp /usr/local/src/fau200/.
cp -f src/fau200device.cpp /usr/local/src/fau200/.
cp -f src/fau200device.h /usr/local/src/fau200/.
cp -f src/fau200-info.cpp /usr/local/src/fau200/.
cp -f src/fau200-list.cpp /usr/local/src/fau200/.
cp -f src/fau200-lockotp.cpp /usr/local/src/fau200/.
cp -f src/fau200-reset.cpp /usr/local/src/fau200/.
cp -f src/fau200-volt.cpp /usr/local/src/fau200/.
cp -f src/GPL.txt /usr/local/src/fau200/.
cp -f src/LGPL.txt /usr/local/src/fau200/.
cp -f src/libusb-extra.c /usr/local/src/fau200/.
cp -f src/libusb-extra.h /usr/local/src/fau200/.
cp -f src/Makefile /usr/local/src/fau200/.
cp -f src/man/fau200-clear.1 /usr/local/src/fau200/man/.
cp -f src/man/fau200-info.1 /usr/local/src/fau200/man/.
cp -f src/man/fau200-list.1 /usr/local/src/fau200/man/.
cp -f src/man/fau200-lockotp.1 /usr/local/src/fau200/man/.
cp -f src/man/fau200-reset.1 /usr/local/src/fau200/man/.
cp -f src/man/fau200-volt.1 /usr/local/src/fau200/man/.
cp -f src/README.txt /usr/local/src/fau200/.
cp -f src/utils.cpp /usr/local/src/fau200/.
cp -f src/utils.h /usr/local/src/fau200/.
echo Building and installing binaries and man pages...
make -C /usr/local/src/fau200 install clean
echo Applying configurations...
cat > /etc/udev/rules.d/70-bgtn-fau200.rules << EOF
SUBSYSTEM=="usb", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="8c35", MODE="0666"
SUBSYSTEM=="usb_device", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="8c35", MODE="0666"
EOF
service udev restart
echo Done!
