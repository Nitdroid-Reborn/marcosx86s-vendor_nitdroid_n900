# config.mk
#
# Product-specific compile-time definitions.
#

# The generic product target doesn't have any hardware-specific pieces.
TARGET_NO_BOOTLOADER := true
TARGET_NO_KERNEL := true
TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_BOARD_PLATFORM := omap3
TARGET_ARCH_VARIANT	:= armv7-a-neon
TARGET_GLOBAL_CFLAGS += -mtune=cortex-a8
TARGET_GLOBAL_CPPFLAGS += -mtune=cortex-a8

TARGET_NO_RECOVERY	:= true
TARGET_PROVIDES_INIT_RC := true
TARGET_NO_RADIOIMAGE := true

# Bluetooth
BOARD_HAVE_BLUETOOTH	:= true

# Enable TI/OMX
HARDWARE_OMX := true

USE_CAMERA_STUB		:= false
#BOARD_USES_TI_CAMERA_HAL := true

BOARD_USES_GENERIC_AUDIO:= false
HAVE_HTC_AUDIO_DRIVER	:= false
BOARD_USES_ALSA_AUDIO	:= true
BUILD_WITH_ALSA_UTILS	:= true

BUILD_WITH_OFONO	:= true

ifdef HARDWARE_OMX
OMX_VENDOR := ti
OMX_VENDOR_INCLUDES := \
   hardware/ti/omap3/omx/system/src/openmax_il/omx_core/inc \
   hardware/ti/omap3/omx/image/src/openmax_il/jpeg_enc/inc
OMX_VENDOR_WRAPPER := TI_OMX_Wrapper
BUILD_WITHOUT_PV:=true
BUILD_WITH_TI_AUDIO:=1
#ENABLE_RMPM_STUB:=1
DVFS_ENABLED:=1
BUILD_JPEG_DECODER:= true
endif

# GPS related defines
BOARD_HAVE_FAKE_GPS := true

# Wifi related defines
BOARD_WPA_SUPPLICANT_DRIVER      := WEXT
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := 
BOARD_WLAN_DEVICE                := wl12xx

WIFI_DRIVER_MODULE_PATH     := "/system/lib/modules/current/wl12xx.ko"
WIFI_DRIVER_MODULE_ARG      := ""
WIFI_DRIVER_MODULE_NAME     := "wl12xx"
WIFI_FIRMWARE_LOADER        := "wlan_loader"
