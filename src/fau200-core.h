/* FAU200 core functions - Version 1.0
   Copyright (c) 2019 Samuel Lourenço

   This library is free software: you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or (at your
   option) any later version.

   This library is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
   License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this library.  If not, see <https://www.gnu.org/licenses/>.


   Please feel free to contact me via e-mail: samuel.fmlourenco@gmail.com */


#ifndef FAU200_CORE_H_
#define FAU200_CORE_H_

// Includes
#include <stdbool.h>
#include <libusb-1.0/libusb.h>

// Defines
#define CPOL0 false   // Boolean corresponding to CPOL = 0, applicable to configure_spi_mode()
#define CPHA0 false   // Boolean corresponding to CPHA = 0, applicable to configure_spi_mode()

// Global external variables
extern int err_level;

// Function prototypes
void configure_spi_mode(libusb_device_handle *devhandle, uint8_t channel, bool cpol, bool cpha);
void disable_cs(libusb_device_handle *devhandle, uint8_t channel);
void disable_spi_delays(libusb_device_handle *devhandle, uint8_t channel);
bool is_otp_locked(libusb_device_handle *devhandle);
void lock_otp(libusb_device_handle *devhandle);
void reset(libusb_device_handle *devhandle);
void select_cs(libusb_device_handle *devhandle, uint8_t channel);
void setup(libusb_device_handle *devhandle);
void set_voltage(libusb_device_handle *devhandle, uint16_t value);

#endif
