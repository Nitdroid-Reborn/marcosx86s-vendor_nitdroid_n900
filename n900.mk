# This is a generic product that isn't specialized for a specific device.
# It includes the base Android platform. If you need Google-specific features,
# you should derive from generic_with_google.mk

$(call inherit-product, $(SRC_TARGET_DIR)/product/generic.mk)

# This is the list of apps included in the generic AOSP build
PRODUCT_PACKAGES += \
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

# Overrides
PRODUCT_BRAND := nokia
PRODUCT_DEVICE := n900
PRODUCT_NAME := n900
PRODUCT_MODEL := Nokia N900


# This is a high DPI device, so add the hdpi pseudo-locale
PRODUCT_LOCALES := hdpi
