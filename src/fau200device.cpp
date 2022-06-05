/* FAU200 device class - Version 1.0.0
   Requires CP2130 class version 1.1.0 or later
   Copyright (c) 2022 Samuel Lourenço

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


// Includes
#include <sstream>
#include <unistd.h>
#include <vector>
#include "fau200device.h"

// Definitions
const uint8_t EPOUT = 0x01;  // Address of endpoint assuming the OUT direction

FAU200Device::FAU200Device() :
    cp2130_()
{
}

// Diagnostic function used to verify if the device has been disconnected
bool FAU200Device::disconnected() const
{
    return cp2130_.disconnected();
}

// Checks if the device is open
bool FAU200Device::isOpen() const
{
    return cp2130_.isOpen();
}

// Closes the device safely, if open
void FAU200Device::close()
{
    cp2130_.close();
}

// Returns the silicon version of the CP2130 bridge
CP2130::SiliconVersion FAU200Device::getCP2130SiliconVersion(int &errcnt, std::string &errstr)
{
    return cp2130_.getSiliconVersion(errcnt, errstr);
}

// Returns the hardware revision of the device
std::string FAU200Device::getHardwareRevision(int &errcnt, std::string &errstr)
{
    return hardwareRevision(getUSBConfig(errcnt, errstr));
}

// Gets the manufacturer descriptor from the device
std::u16string FAU200Device::getManufacturerDesc(int &errcnt, std::string &errstr)
{
    return cp2130_.getManufacturerDesc(errcnt, errstr);
}

// Gets the product descriptor from the device
std::u16string FAU200Device::getProductDesc(int &errcnt, std::string &errstr)
{
    return cp2130_.getProductDesc(errcnt, errstr);
}

// Gets the serial descriptor from the device
std::u16string FAU200Device::getSerialDesc(int &errcnt, std::string &errstr)
{
    return cp2130_.getSerialDesc(errcnt, errstr);
}

// Gets the USB configuration of the device
CP2130::USBConfig FAU200Device::getUSBConfig(int &errcnt, std::string &errstr)
{
    return cp2130_.getUSBConfig(errcnt, errstr);
}

// Opens a device and assigns its handle
int FAU200Device::open(const std::string &serial)
{
    return cp2130_.open(VID, PID, serial);
}

// Issues a reset to the CP2130, which in effect resets the entire device
void FAU200Device::reset(int &errcnt, std::string &errstr)
{
    cp2130_.reset(errcnt, errstr);
}

// Sets up and prepares the device
void FAU200Device::setup(int &errcnt, std::string &errstr)
{
    CP2130::SPIMode mode;
    mode.csmode = CP2130::CSMODEPP;  // Chip select pin mode regarding channel 0 is push-pull
    mode.cfrq = CP2130::CFRQ750K;  // SPI clock frequency set to 750KHz
    mode.cpol = CP2130::CPOL0;  // SPI clock polarity is active high (CPOL = 0)
    mode.cpha = CP2130::CPHA0;  // SPI data is valid on each rising edge (CPHA = 0)
    cp2130_.configureSPIMode(0, mode, errcnt, errstr);  // Configure SPI mode for channel 0, using the above settings
    cp2130_.disableSPIDelays(0, errcnt, errstr);  // Disable all SPI delays for channel 0
    cp2130_.selectCS(0, errcnt, errstr);  // Enable the chip select corresponding to channel 0, and disable any others
    std::vector<uint8_t> config = {0x60, 0x00, 0x00};  // Use the DAC's internal voltage reference (default configuration)
    cp2130_.spiWrite(config, EPOUT, errcnt, errstr);  // Send the the configuration above to the LTC2640 DAC
    usleep(100);  // Wait 100us, in order to prevent possible errors while disabling the chip select (workaround)
    cp2130_.disableCS(0, errcnt, errstr);  // Disable the previously enabled chip select
}

// Sets the output voltage to a given value
void FAU200Device::setVoltage(float voltage, int &errcnt, std::string &errstr)
{
    if (voltage < VOLTAGE_MIN || voltage > VOLTAGE_MAX) {
        ++errcnt;
        errstr += "In setVoltage(): Voltage must be between 0 and 4.095.\n";  // Program logic error
    } else {
        cp2130_.selectCS(0, errcnt, errstr);  // Enable the chip select corresponding to channel 0, and disable any others
        uint16_t voltageCode = static_cast<uint16_t>(voltage * 1000 + 0.5);
        std::vector<uint8_t> set = {
            0x30,                                    // Input and DAC registers updated to the given value
            static_cast<uint8_t>(voltageCode >> 4),  // Upper 8 bits of the 12-bit value
            static_cast<uint8_t>(voltageCode << 4)   // Lower 4 bits of the value, followed by four zero bits
        };
        cp2130_.spiWrite(set, EPOUT, errcnt, errstr);  // Set the output voltage by updating the above registers
        usleep(100);  // Wait 100us, in order to prevent possible errors while disabling the chip select (workaround)
        cp2130_.disableCS(0, errcnt, errstr);  // Disable the previously enabled chip select
    }
}

// Helper function that returns the hardware revision from a given USB configuration
std::string FAU200Device::hardwareRevision(const CP2130::USBConfig &config)
{
    std::string revision;
    if (config.majrel > 1 && config.majrel <= 27) {
        revision += static_cast<char>(config.majrel + 'A' - 2);  // Append major revision letter (a major release number value of 2 corresponds to the letter "A" and so on)
    }
    if (config.majrel == 1 || config.minrel != 0) {
        std::ostringstream stream;
        stream << static_cast<int>(config.minrel);
        revision += stream.str();  // Append minor revision number
    }
    return revision;
}

// Helper function to list devices
std::list<std::string> FAU200Device::listDevices(int &errcnt, std::string &errstr)
{
    return CP2130::listDevices(VID, PID, errcnt, errstr);
}
