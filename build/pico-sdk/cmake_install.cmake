# Install script for directory: E:/programs/PICO/pico-sdk

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/pico_w_webserver")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "E:/programs/PICO/gcc-arm-none-eabi/bin/arm-none-eabi-objdump.exe")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("E:/Onedrive/Dokumente/Studium/Semester V/Mikrorechentechnik/Praktikum/HfBK Zeitmaschine/zeitmaschine_v2/zeitmaschine_v2/build/pico-sdk/tools/cmake_install.cmake")
  include("E:/Onedrive/Dokumente/Studium/Semester V/Mikrorechentechnik/Praktikum/HfBK Zeitmaschine/zeitmaschine_v2/zeitmaschine_v2/build/pico-sdk/src/cmake_install.cmake")
  include("E:/Onedrive/Dokumente/Studium/Semester V/Mikrorechentechnik/Praktikum/HfBK Zeitmaschine/zeitmaschine_v2/zeitmaschine_v2/build/pico-sdk/docs/cmake_install.cmake")

endif()

