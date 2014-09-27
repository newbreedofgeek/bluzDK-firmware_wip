# This file is a makefile included from the top level makefile which
# defines the sources built for the target.

# Define the prefix to this directory. 
# Note: The name must be unique within this build and should be
#       based on the root of the project
TARGET_HAL_SRC_PATH = $(TARGET_HAL_PATH)/src

INCLUDE_DIRS += $(TARGET_HAL_SRC_PATH)

# C source files included in this build.
CSRC += $(TARGET_HAL_SRC_PATH)/adc_hal.c
CSRC += $(TARGET_HAL_SRC_PATH)/core_hal.c
CSRC += $(TARGET_HAL_SRC_PATH)/core_subsys_hal.c
CSRC += $(TARGET_HAL_SRC_PATH)/delay_hal.c
CSRC += $(TARGET_HAL_SRC_PATH)/eeprom_hal.c
CSRC += $(TARGET_HAL_SRC_PATH)/gpio_hal.c
CSRC += $(TARGET_HAL_SRC_PATH)/i2c_hal.c
CSRC += $(TARGET_HAL_SRC_PATH)/inet_hal.c
CSRC += $(TARGET_HAL_SRC_PATH)/interrupts_hal.c
CSRC += $(TARGET_HAL_SRC_PATH)/ota_flash_hal.c
CSRC += $(TARGET_HAL_SRC_PATH)/pinmap_hal.c
CSRC += $(TARGET_HAL_SRC_PATH)/pwm_hal.c
CSRC += $(TARGET_HAL_SRC_PATH)/rtc_hal.c
CSRC += $(TARGET_HAL_SRC_PATH)/servo_hal.c
CSRC += $(TARGET_HAL_SRC_PATH)/socket_hal.c
CSRC += $(TARGET_HAL_SRC_PATH)/spi_hal.c
CSRC += $(TARGET_HAL_SRC_PATH)/timer_hal.c
CSRC += $(TARGET_HAL_SRC_PATH)/tone_hal.c
CSRC += $(TARGET_HAL_SRC_PATH)/usart_hal.c
CSRC += $(TARGET_HAL_SRC_PATH)/usb_hal.c
CSRC += $(TARGET_HAL_SRC_PATH)/wlan_hal.c

# C++ source files included in this build.
CPPSRC += $(TARGET_HAL_SRC_PATH)/deviceid_hal.cpp
# WIP - CPPSRC += $(TARGET_HAL_SRC_PATH)/memory_hal.cpp

# ASM source files included in this build.
ASRC +=
