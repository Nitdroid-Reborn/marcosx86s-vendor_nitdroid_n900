# config.mk
#
# Product-specific compile-time definitions.
#

# The generic product target doesn't have any hardware-specific pieces.
TARGET_NO_BOOTLOADER := true
TARGET_NO_KERNEL := true
TARGET_CPU_ABI := armeabi
TARGET_BOARD_PLATFORM := omap3
TARGET_ARCH_VARIANT	:= armv7-a
TARGET_NO_RECOVERY	:= true
TARGET_PROVIDES_INIT_RC := true
TARGET_NO_RADIOIMAGE := true

# Bluetooth
BOARD_HAVE_BLUETOOTH	:= true

TARGET_HARDWARE_3D	:= false
#OMAP3_GL := false
#HARDWARE_OMX := true

USE_CAMERA_STUB		:= true
#BOARD_USES_TI_CAMERA_HAL := true

BOARD_USES_GENERIC_AUDIO := false
HAVE_HTC_AUDIO_DRIVER := false
BOARD_USES_ALSA_AUDIO := true
BUILD_WITH_ALSA_UTILS := true

#BUILD_FM_RADIO := true
#BOARD_HAVE_FM_ROUTING := true
#FM_CHR_DEV_ST := true
#USE_QEMU_GPS_HARDWARE	:= true

ifdef HARDWARE_OMX
OMX_VENDOR := ti
OMX_VENDOR_INCLUDES := \
   hardware/ti/omap3/omx/system/src/openmax_il/omx_core/inc \
   hardware/ti/omap3/omx/image/src/openmax_il/jpeg_enc/inc
OMX_VENDOR_WRAPPER := TI_OMX_Wrapper
BOARD_OPENCORE_LIBRARIES := libOMX_Core
BOARD_OPENCORE_FLAGS := -DHARDWARE_OMX=1
OMX_JPEG := true
endif


# Wifi related defines
#WPA_SUPPLICANT_VERSION		 := VER_0_6_X
BOARD_WPA_SUPPLICANT_DRIVER      := WEXT
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := 
BOARD_WLAN_DEVICE                := wl12xx

WIFI_DRIVER_MODULE_PATH     := "/system/lib/modules/wl12xx.ko"
WIFI_DRIVER_MODULE_ARG      := ""
WIFI_DRIVER_MODULE_NAME     := "wl12xx"
WIFI_FIRMWARE_LOADER        := ""

# This define enables the compilation of OpenCore's command line TestApps
#BUILD_PV_TEST_APPS :=1

