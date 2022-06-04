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


#ifndef FAU200DEVICE_H
#define FAU200DEVICE_H

// Includes
#include <cstdint>
#include <list>
#include <string>
#include "cp2130.h"

class FAU200Device
{
private:
    CP2130 cp2130_;

public:
    // Class definitions
    static const uint16_t VID = 0x10c4;                          // USB vendor ID
    static const uint16_t PID = 0x8c35;                          // USB product ID
    static const int SUCCESS = CP2130::SUCCESS;                  // Returned by open() if successful
    static const int ERROR_INIT = CP2130::ERROR_INIT;            // Returned by open() in case of a libusb initialization failure
    static const int ERROR_NOT_FOUND = CP2130::ERROR_NOT_FOUND;  // Returned by open() if the device was not found
    static const int ERROR_BUSY = CP2130::ERROR_BUSY;            // Returned by open() if the device is already in use
    
    // Limits applicable to setVoltage()
    static constexpr float VOLTAGE_MIN = 0;       // Minimum voltage
    static constexpr float VOLTAGE_MAX = 4.095;   // Maximum voltage

    FAU200Device();

    bool disconnected() const;
    bool isOpen() const;

    void close();
    CP2130::SiliconVersion getCP2130SiliconVersion(int &errcnt, std::string &errstr);
    std::string getHardwareRevision(int &errcnt, std::string &errstr);
    std::u16string getManufacturerDesc(int &errcnt, std::string &errstr);
    std::u16string getProductDesc(int &errcnt, std::string &errstr);
    std::u16string getSerialDesc(int &errcnt, std::string &errstr);
    CP2130::USBConfig getUSBConfig(int &errcnt, std::string &errstr);
    int open(const std::string &serial = std::string());
    void reset(int &errcnt, std::string &errstr);
    void setup(int &errcnt, std::string &errstr);
    void setVoltage(float voltage, int &errcnt, std::string &errstr);

    static std::string hardwareRevision(const CP2130::USBConfig &config);
    static std::list<std::string> listDevices(int &errcnt, std::string &errstr);
};

#endif  // FAU200DEVICE_H
