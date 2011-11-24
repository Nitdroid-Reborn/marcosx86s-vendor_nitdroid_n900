LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# Misc
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/busybox/busybox:$(TARGET_ROOT_OUT)/bin/busybox \
	$(LOCAL_PATH)/init.rc:$(TARGET_ROOT_OUT)/init.rc \
	$(LOCAL_PATH)/init.nokia.rc:$(TARGET_ROOT_OUT)/init.nokia.rc \
	$(LOCAL_PATH)/etc/vold.fstab:$(LOCAL_PATH)/etc/vold.fstab \
	$(LOCAL_PATH)/etc/switchswap.sh:$(TARGET_ROOT_OUT)/system/etc/switchswap.sh \
	$(LOCAL_PATH)/system/xbin/rr:$(TARGET_ROOT_OUT)/system/xbin/rr \
	$(LOCAL_PATH)/etc/modem.conf:$(TARGET_ROOT_OUT)/system/etc/modem.conf \
	$(LOCAL_PATH)/etc/gps.conf:$(TARGET_ROOT_OUT)/system/etc/gps.conf \
	$(LOCAL_PATH)/etc/asound.conf:$(TARGET_ROOT_OUT)/system/etc/asound.conf \
	$(LOCAL_PATH)/etc/init.nokia.sh:$(TARGET_ROOT_OUT)/system/etc/init.nokia.sh \
	$(LOCAL_PATH)/ueventd.nokia.rc:$(TARGET_ROOT_OUT)/ueventd.nokia.rc \
	$(LOCAL_PATH)/etc/dhcpcd/dhcpcd.conf:$(TARGET_ROOT_OUT)/system/etc/dhcpcd/dhcpcd.conf \
	$(LOCAL_PATH)/etc/wifi/wpa_supplicant.conf:$(TARGET_ROOT_OUT)/system/etc/wifi/wpa_supplicant.conf \
	$(LOCAL_PATH)/system/maemo/bin/bme:$(TARGET_ROOT_OUT)/system/maemo/bin/bme \
	$(LOCAL_PATH)/system/maemo/bin/bq27:$(TARGET_ROOT_OUT)/system/maemo/bin/bq27 \
	$(LOCAL_PATH)/system/maemo/bin/dsme:$(TARGET_ROOT_OUT)/system/maemo/bin/dsme \
	$(LOCAL_PATH)/system/maemo/bin/init_subsys:$(TARGET_ROOT_OUT)/system/maemo/bin/init_subsys \
	$(LOCAL_PATH)/system/maemo/bin/klogd:$(TARGET_ROOT_OUT)/system/maemo/bin/klogd \
	$(LOCAL_PATH)/system/maemo/etc/android.sh:$(TARGET_ROOT_OUT)/system/maemo/etc/android.sh \
	$(LOCAL_PATH)/system/maemo/etc/config.sh:$(TARGET_ROOT_OUT)/system/maemo/etc/config.sh \
	$(LOCAL_PATH)/system/maemo/bin/wifi:$(TARGET_ROOT_OUT)/system/maemo/bin/wifi
##

# Input device calibration files
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/TSC2005_touchscreen.idc:system/usr/idc/TSC2005_touchscreen.idc \
	$(LOCAL_PATH)/omap_twl4030keypad.idc:system/usr/idc/omap_twl4030keypad.idc
##

# Keylayouts
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/omap_twl4030keypad.kl:$(TARGET_OUT_KEYLAYOUT)/omap_twl4030keypad.kl \
	$(LOCAL_PATH)/triton2-pwrbutton.kl:$(TARGET_OUT_KEYLAYOUT)/triton2-pwrbutton.kl \
	$(LOCAL_PATH)/gpio-keys.kl:$(TARGET_OUT_KEYLAYOUT)/gpio-keys.kl \
##

# Keycharmaps
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/omap_twl4030keypad.kcm:$(TARGET_OUT_KEYCHARS)/omap_twl4030keypad.kcm
##
