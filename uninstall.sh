#!/bin/sh

echo Rolling back configurations...
rm -f /etc/udev/rules.d/70-bgtn-fau200.rules
service udev restart
echo Removing binaries and man pages...
make -C /usr/local/src/fau200 uninstall
echo Removing source code files...
rm -rf /usr/local/src/fau200
echo Done!
