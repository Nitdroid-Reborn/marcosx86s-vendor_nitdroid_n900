LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# This device is xhdpi.  However the platform doesn't
# currently contain all of the bitmaps at xhdpi density so
# we do this little trick to fall back to the hdpi version
# if the xhdpi doesn't exist.
#PRODUCT_AAPT_CONFIG := normal hdpi
#PRODUCT_AAPT_PREF_CONFIG := hdpi

# Misc
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/busybox/busybox:root/bin/busybox \
	$(LOCAL_PATH)/init.rc:root/init.rc \
	$(LOCAL_PATH)/init.nokia.rc:root/init.nokiarx-51board.rc \
	$(LOCAL_PATH)/ueventd.nokia.rc:root/ueventd.nokia-rx51board.rc \
	$(LOCAL_PATH)/etc/media_profiles.xml:system/etc/media_profiles.xml \
	$(LOCAL_PATH)/etc/vold.fstab:system/etc/vold.fstab \
	$(LOCAL_PATH)/etc/switchswap.sh:system/etc/switchswap.sh \
	$(LOCAL_PATH)/etc/modem.conf:system/etc/modem.conf \
	$(LOCAL_PATH)/etc/gps.conf:system/etc/gps.conf \
	$(LOCAL_PATH)/etc/init.nokia.sh:system/etc/init.nokia.sh \
	$(LOCAL_PATH)/etc/dhcpcd/dhcpcd.conf:system/etc/dhcpcd/dhcpcd.conf \
	$(LOCAL_PATH)/etc/wifi/wpa_supplicant.conf:system/etc/wifi/wpa_supplicant.conf \
	$(LOCAL_PATH)/system/xbin/rr:system/xbin/rr \
	$(LOCAL_PATH)/system/maemo/bin/bme:system/maemo/bin/bme \
	$(LOCAL_PATH)/system/maemo/bin/bq27:system/maemo/bin/bq27 \
	$(LOCAL_PATH)/system/maemo/bin/dsme:system/maemo/bin/dbus \
	$(LOCAL_PATH)/system/maemo/bin/dsme:system/maemo/bin/dsme \
	$(LOCAL_PATH)/system/maemo/bin/init_subsys:system/maemo/bin/init_subsys \
	$(LOCAL_PATH)/system/maemo/bin/klogd:system/maemo/bin/klogd \
	$(LOCAL_PATH)/system/maemo/etc/android.sh:system/maemo/etc/android.sh \
	$(LOCAL_PATH)/system/maemo/etc/config.sh:system/maemo/etc/config.sh \
	$(LOCAL_PATH)/system/maemo/bin/wifi:system/maemo/bin/wifi
##

# Input device calibration files
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/TSC2005_touchscreen.idc:system/usr/idc/TSC2005_touchscreen.idc \
	$(LOCAL_PATH)/omap_twl4030keypad.idc:system/usr/idc/omap_twl4030keypad.idc
##

# Keylayouts
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/omap_twl4030keypad.kl:system/usr/keylayout/omap_twl4030keypad.kl \
	$(LOCAL_PATH)/triton2-pwrbutton.kl:system/usr/keylayout/triton2-pwrbutton.kl \
	$(LOCAL_PATH)/gpio-keys.kl:system/usr/keylayout/gpio-keys.kl \
##

# Keycharmaps
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/omap_twl4030keypad.kcm:system/usr/keychars/omap_twl4030keypad.kcm
##

# Permissions
PRODUCT_COPY_FILES += \
    frameworks/base/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml \
    frameworks/base/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    frameworks/base/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml \
    frameworks/base/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/base/data/etc/android.software.sip.voip.xml:system/etc/permissions/android.software.sip.voip.xml \
##
