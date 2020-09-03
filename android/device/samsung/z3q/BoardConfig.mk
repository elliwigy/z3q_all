# Inherit common board flags
include device/samsung/sm8250-common/BoardConfigCommon.mk

DEVICE_PATH := device/samsung/z3q

# Custom
ALLOW_MISSING_DEPENDENCIES := true

# Custom
BUILD_BROKEN_DUP_RULES := true

# Custom
# SELINUX_IGNORE_NEVERALLOWS := true

# Platform
TARGET_SPECIFIC_HEADER_PATH += $(DEVICE_PATH)/include

# Assert
TARGET_OTA_ASSERT_DEVICE := z3q

# Kernel
TARGET_KERNEL_CONFIG := vendor/x1q_chn_openx_defconfig
TARGET_KERNEL_VARIANT_CONFIG := vendor/variant_z3q_chn_openx_defconfig
KERNEL_LD := DEBUG_DEFCONFIG=vendor/aosp_defconfig

# Bluetooth
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR := $(DEVICE_PATH)/bluetooth/include

# Display
TARGET_SCREEN_DENSITY := 560

# Properties
TARGET_PRODUCT_PROP += $(DEVICE_PATH)/product.prop
