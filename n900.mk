# This is a generic product that isn't specialized for a specific device.
# It includes the base Android platform. If you need Google-specific features,
# you should derive from generic_with_google.mk

$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base.mk)

DEVICE_PACKAGE_OVERLAYS := vendor/nitdroid/n900/overlay

# This is the list of apps included in the generic AOSP build
PRODUCT_PACKAGES += \
    Camera \
    Development \
    SpareParts \
    SoundRecorder \
    VoiceDialer
##

# Overrides
PRODUCT_BRAND := nokia
PRODUCT_DEVICE := n900
PRODUCT_NAME := n900
PRODUCT_MODEL := Nokia N900

# This is a high DPI device, so add the hdpi pseudo-locale
PRODUCT_LOCALES := en_US hdpi
