/* FAU200 Clear Command - Version 1.1 for Debian Linux
   Copyright (c) 2018-2019 Samuel Lourenço

   This program is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation, either version 3 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   You should have received a copy of the GNU General Public License along
   with this program.  If not, see <https://www.gnu.org/licenses/>.


   Please feel free to contact me via e-mail: samuel.fmlourenco@gmail.com */


// Includes
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libusb-1.0/libusb.h>
#include "fau200-core.h"
#include "libusb-extra.h"

int main(int argc, char **argv)
{
    err_level = EXIT_SUCCESS;  // Note that this variable is declared externally!
    libusb_context *context;
    if (libusb_init(&context) != 0)  // Initialize libusb. In case of failure
    {
        fprintf(stderr, "Error: Could not initialize libusb.\n");
        err_level = EXIT_FAILURE;
    }
    else  // If libusb is initialized
    {
        libusb_device_handle *devhandle;
        if (argc < 2)  // If the program was called without arguments
            devhandle = libusb_open_device_with_vid_pid(context, 0x10C4, 0x8C35);  // Open a device and get the device handle
        else  // Serial number was specified as argument
            devhandle = libusb_open_device_with_vid_pid_serial(context, 0x10C4, 0x8C35, (unsigned char *)argv[1]);  // Open the device having the specified serial number, and get the device handle
        if (devhandle == NULL)  // If the previous operation fails to get a device handle
        {
            fprintf(stderr, "Error: Could not find device.\n");
            err_level = EXIT_FAILURE;
        }
        else  // If the device is successfully opened and a handle obtained
        {
            bool kernel_attached = false;
            if (libusb_kernel_driver_active(devhandle, 0) != 0)  // If a kernel driver is active on the interface
            {
                libusb_detach_kernel_driver(devhandle, 0);  // Detach the kernel driver
                kernel_attached = true;  // Flag that the kernel driver was attached
            }
            if (libusb_claim_interface(devhandle, 0) != 0)  // Claim the interface. In case of failure
            {
                fprintf(stderr, "Error: Device is currently unavailable.\n");
                err_level = EXIT_FAILURE;
            }
            else  // If the interface is successfully claimed
            {
                configure_spi_mode(devhandle, 0, CPOL0, CPHA0);  // Clock polarity regarding channel 0 is active high (CPOL = 0) and data is valid on each rising edge (CPHA = 0)
                disable_spi_delays(devhandle, 0);  // Disable all SPI delays for channel 0
                select_cs(devhandle, 0);  // Enable the chip select corresponding to channel 0, and disable any others
                setup(devhandle);  // Configure the DAC, so that it uses its own voltage reference (by sending a specific sequence of three bytes to the LTC2640 DAC on channel 0)
                /* Note that the LTC2640 has a 24-bit shift register, and it will only
                   acknowledge the last three bytes in a given sequence. Thus, the
                   configuration and voltage settings must be done separately. */
                set_voltage(devhandle, 0);  // Set the output voltage to zero (again, by sending a specific sequence of three bytes to the LTC2640 DAC on channel 0)
                usleep(100);  // Wait 100us, in order to prevent possible errors while disabling the previouly enabled chip select (workaround)
                disable_cs(devhandle, 0);  // Disable the chip select corresponding to channel 0
                if (err_level == 0)  // If all goes well
                    printf("All settings cleared.\n");
                libusb_release_interface(devhandle, 0);  // Release the interface
            }
            if (kernel_attached)  // If a kernel driver was attached to the interface before
                libusb_attach_kernel_driver(devhandle, 0);  // Reattach the kernel driver
            libusb_close(devhandle);  // Close the device
        }
        libusb_exit(context);  // Deinitialize libusb
    }
    return err_level;
}
