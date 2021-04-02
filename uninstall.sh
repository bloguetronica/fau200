#!/bin/sh

echo Rolling back configurations...
rm -f /etc/udev/rules.d/70-bgtn-fau200.rules
service udev restart
echo Removing man pages...
rm -f /usr/local/share/man/man1/fau200-clear.1.gz
rm -f /usr/local/share/man/man1/fau200-list.1.gz
rm -f /usr/local/share/man/man1/fau200-lockotp.1.gz
rm -f /usr/local/share/man/man1/fau200-reset.1.gz
rm -f /usr/local/share/man/man1/fau200-volt.1.gz
rmdir --ignore-fail-on-non-empty /usr/local/share/man/man1
echo Removing binaries...
make -C /usr/local/src/fau200 uninstall
echo Removing source code files...
rm -rf /usr/local/src/fau200
echo Done!
