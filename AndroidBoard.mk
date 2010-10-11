LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := omap_twl4030keypad.kcm
include $(BUILD_KEY_CHAR_MAP)

file := $(TARGET_OUT_KEYLAYOUT)/omap_twl4030keypad.kl
ALL_PREBUILT += $(file)
$(file) : $(LOCAL_PATH)/omap_twl4030keypad.kl | $(ACP)
	$(transform-prebuilt-to-target)

file := $(TARGET_OUT_KEYLAYOUT)/triton2-pwrbutton.kl
ALL_PREBUILT += $(file)
$(file) : $(LOCAL_PATH)/triton2-pwrbutton.kl | $(ACP)
	$(transform-prebuilt-to-target)

file := $(TARGET_OUT_KEYLAYOUT)/gpio-keys.kl
ALL_PREBUILT += $(file)
$(file) : $(LOCAL_PATH)/gpio-keys.kl | $(ACP)
        $(transform-prebuilt-to-target)

file := $(TARGET_ROOT_OUT)/init.rc
ALL_PREBUILT += $(file)
$(file) : $(LOCAL_PATH)/init.rc | $(ACP)
	$(transform-prebuilt-to-target)

file := $(TARGET_ROOT_OUT)/init.nokia.rc
ALL_PREBUILT += $(file)
$(file) : $(LOCAL_PATH)/init.nokia.rc | $(ACP)
	$(transform-prebuilt-to-target)

file := $(TARGET_ROOT_OUT)/system/etc/vold.fstab
ALL_PREBUILT += $(file)
$(file) : $(LOCAL_PATH)/etc/vold.fstab | $(ACP)
	$(transform-prebuilt-to-target)

file := $(TARGET_ROOT_OUT)/system/etc/modem.conf
ALL_PREBUILT += $(file)
$(file) : $(LOCAL_PATH)/etc/modem.conf | $(ACP)
        $(transform-prebuilt-to-target)

file := $(TARGET_ROOT_OUT)/system/etc/gps.conf
ALL_PREBUILT += $(file)
$(file) : $(LOCAL_PATH)/etc/gps.conf | $(ACP)
        $(transform-prebuilt-to-target)

file := $(TARGET_ROOT_OUT)/system/etc/asound.conf
ALL_PREBUILT += $(file)
$(file) : $(LOCAL_PATH)/etc/asound.conf | $(ACP)
	$(transform-prebuilt-to-target)

file := $(TARGET_ROOT_OUT)/system/etc/init.nokia.sh
ALL_PREBUILT += $(file)
$(file) : $(LOCAL_PATH)/etc/init.nokia.sh | $(ACP)
	$(transform-prebuilt-to-target)

file := $(TARGET_ROOT_OUT)/system/etc/dhcpcd/dhcpcd.conf
ALL_PREBUILT += $(file)
$(file) : $(LOCAL_PATH)/etc/dhcpcd/dhcpcd.conf | $(ACP)
	$(transform-prebuilt-to-target)

file := $(TARGET_ROOT_OUT)/system/etc/wifi/wpa_supplicant.conf
ALL_PREBUILT += $(file)
$(file) : $(LOCAL_PATH)/etc/wifi/wpa_supplicant.conf | $(ACP)
	$(transform-prebuilt-to-target)

file := $(TARGET_ROOT_OUT)/system/maemo/bin/bme
ALL_PREBUILT += $(file)
$(file) : $(LOCAL_PATH)/system/maemo/bin/bme | $(ACP)
        $(transform-prebuilt-to-target)

file := $(TARGET_ROOT_OUT)/system/maemo/bin/bq27
ALL_PREBUILT += $(file)
$(file) : $(LOCAL_PATH)/system/maemo/bin/bq27 | $(ACP)
        $(transform-prebuilt-to-target)

file := $(TARGET_ROOT_OUT)/system/maemo/bin/dsme
ALL_PREBUILT += $(file)
$(file) : $(LOCAL_PATH)/system/maemo/bin/dsme | $(ACP)
        $(transform-prebuilt-to-target)

file := $(TARGET_ROOT_OUT)/system/maemo/bin/init_subsys
ALL_PREBUILT += $(file)
$(file) : $(LOCAL_PATH)/system/maemo/bin/init_subsys | $(ACP)
        $(transform-prebuilt-to-target)

file := $(TARGET_ROOT_OUT)/system/maemo/bin/klogd
ALL_PREBUILT += $(file)
$(file) : $(LOCAL_PATH)/system/maemo/bin/klogd | $(ACP)
        $(transform-prebuilt-to-target)

file := $(TARGET_ROOT_OUT)/system/maemo/bin/phonet
ALL_PREBUILT += $(file)
$(file) : $(LOCAL_PATH)/system/maemo/bin/phonet | $(ACP)
        $(transform-prebuilt-to-target)

file := $(TARGET_ROOT_OUT)/system/maemo/etc/android.sh
ALL_PREBUILT += $(file)
$(file) : $(LOCAL_PATH)/system/maemo/etc/android.sh | $(ACP)
        $(transform-prebuilt-to-target)

file := $(TARGET_ROOT_OUT)/system/maemo/etc/config.sh
ALL_PREBUILT += $(file)
$(file) : $(LOCAL_PATH)/system/maemo/etc/config.sh | $(ACP)
        $(transform-prebuilt-to-target)

file := $(TARGET_ROOT_OUT)/system/maemo/bin/wifi
ALL_PREBUILT += $(file)
$(file) : $(LOCAL_PATH)/system/maemo/bin/wifi | $(ACP)
        $(transform-prebuilt-to-target)
