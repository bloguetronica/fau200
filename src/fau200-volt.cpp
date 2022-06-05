/* FAU200 Volt Command - Version 2.0 for Debian Linux
   Copyright (c) 2018-2022 Samuel Lourenço

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
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include "error.h"
#include "fau200device.h"
#include "utils.h"

// Global variables
int EXIT_USERERR = 2;  // Exit status value to indicate a command usage error

int main(int argc, char **argv)
{
    int err, errlvl = EXIT_SUCCESS;
    if (argc < 2) {  // If the program was called without arguments
        std::cerr << "Error: Missing argument.\nUsage: fau200-volt VOLTAGE(V)\n";
        errlvl = EXIT_USERERR;
    } else if (!Utils::isNumber(argv[1])) {  // If the argument string doesn't constitute a valid number
        std::cerr << "Error: Argument is not a valid number.\n";
        errlvl = EXIT_USERERR;
    } else {  // Serial number was specified as argument
        float voltage = atof(argv[1]);  // Convert the argument string into a floating point number
        if (voltage < FAU200Device::VOLTAGE_MIN || voltage > FAU200Device::VOLTAGE_MAX) {  // If the obtained voltage value (in V) after conversion is lesser than "VOLTAGE_MIN" [0] or greater than "VOLTAGE_MAX" [4.095]
            std::cerr << "Error: Voltage should be between 0 and 4.095V.\n";
            errlvl = EXIT_USERERR;
        } else {
            FAU200Device device;
            if (argc < 3) {  // If no serial number was specified
                err = device.open();  // Open a device and get the device handle
            } else {  // Serial number was specified as a second (optional) argument
                err = device.open(argv[2]);  // Open the device having the specified serial number, and get the device handle
            }    
            if (err == FAU200Device::SUCCESS) {  // Device was successfully opened
                int errcnt = 0;
                std::string errstr;
                device.setup(errcnt, errstr);  // Prepare the device (SPI setup)
                device.setVoltage(voltage, errcnt, errstr);  // Set the output voltage to the given value
                if (errcnt > 0) {  // In case of error
                    if (device.disconnected()) {  // If the device disconnected
                        std::cerr << "Error: Device disconnected.\n";
                    } else {
                        printErrors(errstr);
                    }
                    errlvl = EXIT_FAILURE;
                } else {  // Operation successful
                    std::cout << "Voltage set to " << std::fixed << std::setprecision(3) << voltage << "V." << std::endl;  // Print the set output voltage
                }
                device.close();
            } else {  // Failed to open device
                if (err == FAU200Device::ERROR_INIT) {  // Failed to initialize libusb
                    std::cerr << "Error: Could not initialize libusb\n";
                } else if (err == FAU200Device::ERROR_NOT_FOUND) {  // Failed to find device
                    std::cerr << "Error: Could not find device.\n";
                } else if (err == FAU200Device::ERROR_BUSY) {  // Failed to claim interface
                    std::cerr << "Error: Device is currently unavailable.\n";
                }
                errlvl = EXIT_FAILURE;
            }
        }
    }
    return errlvl;
}
