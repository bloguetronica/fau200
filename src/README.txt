This directory contains all source code files required for compiling the
commands for FAU200 Power Supply. A list of relevant files follows:
– cp2130.cpp;
– cp2130.h;
– error.cpp;
– error.h;
– fau200-clear.cpp;
– fau200device.cpp;
– fau200device.h;
– fau200-info.cpp;
– fau200-list.cpp;
– fau200-lockotp.cpp;
– fau200-reset.cpp;
– fau200-volt.cpp;
– libusb-extra.c;
– libusb-extra.h;
– Makefile;
– man/fau200-clear.1;
– man/fau200-info.1;
– man/fau200-list.1;
– man/fau200-lockotp.1;
– man/fau200-reset.1;
– man/fau200-volt.1;
– utils.cpp;
– utils.h.

In order to compile successfully all commands, you must have the packages
"build-essential" and "libusb-1.0-0-dev" installed. Given that, if you wish to
simply compile, change your working directory to the current one on a terminal
window, and simply invoke "make" or "make all". If you wish to install besides
compiling, run "sudo make install". Alternatively, if you wish to force a
rebuild, you should invoke "make clean all", or "sudo make clean install" if
you prefer to install after rebuilding.

It may be necessary to undo any previous operations. Invoking "make clean"
will delete all object code generated (binaries included) during earlier
compilations. You can also invoke "sudo make uninstall" to unistall the
binaries.

P.S.:
Notice that any make operation containing the targets "install" or "uninstall"
(e.g. "make all install" or "make uninstall") requires root permissions, or in
other words, must be run with sudo.
