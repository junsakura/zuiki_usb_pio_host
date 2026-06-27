# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(zuiki_usb_pio_host C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Explicitly enable TinyUSB so both native USB device and PIO-USB host are built.
set(PICO_ENABLE_TINYUSB 1)

# Pico-PIO-USB provides pio_usb.h. It is fetched by TinyUSB's get_deps.py,
# not by a direct git submodule pathspec. The VS Code task runs:
#   python tools/get_deps.py rp2040
# inside ${PICO_SDK_PATH}/lib/tinyusb before configuring.
set(PICO_PIO_USB_PATH
    "${PICO_SDK_PATH}/lib/tinyusb/hw/mcu/raspberry_pi/Pico-PIO-USB"
    CACHE PATH "Path to Pico-PIO-USB")

if (NOT EXISTS "${PICO_PIO_USB_PATH}/src/pio_usb.h")
    message(FATAL_ERROR
        "pio_usb.h was not found. Pico-PIO-USB is missing.\n"
        "Expected: ${PICO_PIO_USB_PATH}/src/pio_usb.h\n"
        "Fix on Windows PowerShell:\n"
        "  git -C $env:USERPROFILE/.pico-sdk/sdk/2.2.0/lib/tinyusb submodule update --init --recursive hw/mcu/raspberry_pi/Pico-PIO-USB\n"
        "Or clone it and configure with -DPICO_PIO_USB_PATH=C:/path/to/Pico-PIO-USB")
endif()

# Add executable. Default name is the project name, version 0.1

add_executable(zuiki_usb_pio_host 
        src/main.c
        src/usb_descriptors.c
        src/logger.c
        src/usb_host.c
        src/hid_dispatch.c
        src/zuiki.c
        src/oled_probe.c
        )

pico_set_program_name(zuiki_usb_pio_host "zuiki_usb_pio_host")
pico_set_program_version(zuiki_usb_pio_host "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(zuiki_usb_pio_host 0)
pico_enable_stdio_usb(zuiki_usb_pio_host 0)

# Add the standard library and USB libraries to the build.
# tinyusb_device is needed for the CDC logger on the Pico USB port.
# tinyusb_host + pico_pio_usb are needed for the USB-A host port through PIO USB.
target_link_libraries(zuiki_usb_pio_host
        pico_stdlib
        tinyusb_device
        tinyusb_host
        tinyusb_board
        tinyusb_pico_pio_usb
        hardware_pio
        hardware_i2c
)

# Add the standard include files to the build
target_include_directories(zuiki_usb_pio_host PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        include
        ${PICO_PIO_USB_PATH}/src
        ${PICO_PIO_USB_PATH}/src/rp2040
)

pico_add_extra_outputs(zuiki_usb_pio_host)

