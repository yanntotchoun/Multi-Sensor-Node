# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/yanno/esp/esp-idf/components/bootloader/subproject"
  "/home/yanno/esp/Multi-Sensor-Node/Multi-Sensor-Node/build/bootloader"
  "/home/yanno/esp/Multi-Sensor-Node/Multi-Sensor-Node/build/bootloader-prefix"
  "/home/yanno/esp/Multi-Sensor-Node/Multi-Sensor-Node/build/bootloader-prefix/tmp"
  "/home/yanno/esp/Multi-Sensor-Node/Multi-Sensor-Node/build/bootloader-prefix/src/bootloader-stamp"
  "/home/yanno/esp/Multi-Sensor-Node/Multi-Sensor-Node/build/bootloader-prefix/src"
  "/home/yanno/esp/Multi-Sensor-Node/Multi-Sensor-Node/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/yanno/esp/Multi-Sensor-Node/Multi-Sensor-Node/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/yanno/esp/Multi-Sensor-Node/Multi-Sensor-Node/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
