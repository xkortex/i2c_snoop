cmake_minimum_required(VERSION 3.6)
project (i2c_snoop)

#add_library(avr /usr/lib/avr/)
#find_package(avr REQUIRED)

include_directories(/usr/lib/avr
        /usr/lib/avr/include
        /usr/share/arduino/hardware/arduino/cores/arduino
        /usr/share/arduino/libraries/EEPROM
        /usr/share/arduino/libraries/Wire
        /usr/share/arduino15/packages/adafruit/hardware/nrf52/0.7.5/libraries/Bluefruit52Lib/src
        /usr/share/arduino15/packages/adafruit/hardware/nrf52/0.7.5/libraries/nffs/src
        /usr/share/arduino15/packages/adafruit/hardware/nrf52/0.7.5/cores/nRF5/SDK/components/device/
        #                    /usr/lib/avr/include/avr
        #                    /usr/lib/gcc/avr
)

FIND_LIBRARY(avr_LIBRARIES avr /usr/lib/avr)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES i2c_snoop.ino i2c_snoop.h constants.h uart_io.cpp uart_io.h)

add_executable(i2c_snoop ${SOURCE_FILES})
#target_link_libraries(LyrebirdFirmware ${avr_LIBRARIES})