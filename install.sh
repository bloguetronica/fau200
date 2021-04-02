#!/bin/sh

echo Obtaining required packages...
apt-get -qq update
apt-get -qq install build-essential
apt-get -qq install libusb-1.0-0-dev
echo Copying source code files...
mkdir -p /usr/local/src/fau200
cp -f src/common.c /usr/local/src/fau200/.
cp -f src/common.h /usr/local/src/fau200/.
cp -f src/fau200-clear.c /usr/local/src/fau200/.
cp -f src/fau200-core.c /usr/local/src/fau200/.
cp -f src/fau200-core.h /usr/local/src/fau200/.
cp -f src/fau200-list.c /usr/local/src/fau200/.
cp -f src/fau200-lockotp.c /usr/local/src/fau200/.
cp -f src/fau200-reset.c /usr/local/src/fau200/.
cp -f src/fau200-volt.c /usr/local/src/fau200/.
cp -f src/GPL.txt /usr/local/src/fau200/.
cp -f src/LGPL.txt /usr/local/src/fau200/.
cp -f src/libusb-extra.c /usr/local/src/fau200/.
cp -f src/libusb-extra.h /usr/local/src/fau200/.
cp -f src/Makefile /usr/local/src/fau200/.
cp -f src/README.txt /usr/local/src/fau200/.
echo Building and installing binaries...
make -C /usr/local/src/fau200 all install clean
echo Installing man pages...
mkdir -p /usr/local/share/man/man1
cp -f man/fau200-clear.1.gz /usr/local/share/man/man1/.
cp -f man/fau200-list.1.gz /usr/local/share/man/man1/.
cp -f man/fau200-lockotp.1.gz /usr/local/share/man/man1/.
cp -f man/fau200-reset.1.gz /usr/local/share/man/man1/.
cp -f man/fau200-volt.1.gz /usr/local/share/man/man1/.
echo Applying configurations...
cat > /etc/udev/rules.d/70-bgtn-fau200.rules << EOF
SUBSYSTEM=="usb", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="8c35", MODE="0666"
SUBSYSTEM=="usb_device", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="8c35", MODE="0666"
EOF
service udev restart
echo Done!
