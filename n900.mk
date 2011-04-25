# This is a generic product that isn't specialized for a specific device.
# It includes the base Android platform. If you need Google-specific features,
# you should derive from generic_with_google.mk

$(call inherit-product, $(SRC_TARGET_DIR)/product/generic.mk)

# This is the list of apps included in the generic AOSP build
PRODUCT_PACKAGES += \
    Development \
    Term \
    SpareParts \
    Provision \
    DeskClock \
    DownloadProvider \
    Gallery3D \
    GlobalSearch \
    LiveWallpapers \
    LiveWallpapersPicker \
    MagicSmokeWallpapers \
    SoundRecorder \
    VisualizationWallpers \
    VoiceDialer \
	libRS \
	librs_jni

### Permissions ###

# Handheld core
PRODUCT_COPY_FILES += \
    frameworks/base/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml

# Wi-Fi
PRODUCT_COPY_FILES += \
    frameworks/base/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml

# Telephony (GSM)
PRODUCT_COPY_FILES += \
    frameworks/base/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml

# Location (GPS)
PRODUCT_COPY_FILES += \
    frameworks/base/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml

# Enable SIP+VoIP
PRODUCT_COPY_FILES += \
    frameworks/base/data/etc/android.software.sip.voip.xml:system/etc/permissions/android.software.sip.voip.xml

### End of Permissions ###


# Overrides
PRODUCT_BRAND := nokia
PRODUCT_DEVICE := n900
PRODUCT_NAME := n900
PRODUCT_MODEL := Nokia N900


# This is a high DPI device, so add the hdpi pseudo-locale
PRODUCT_LOCALES := hdpi
