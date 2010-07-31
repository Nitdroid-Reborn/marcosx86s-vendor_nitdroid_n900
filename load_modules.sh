#!/system/bin/sh
BDADDR="0cddef0a0e6d"
swapon /dev/block/mmcblk1p3
echo 25 > /proc/vm/swappiness
chmod 0777 /dev/pvrsrvkm
chown system.graphics /dev/pvrsrvkm
echo $BDADDR | sed 's/\(..\)/\1:/g;s/:$//' > /sys/devices/platform/hci_h4p/bdaddr
chown system.system /sys/class/leds/lp5523\:kb1/brightness
chown system.system /sys/class/leds/lp5523\:kb2/brightness
chown system.system /sys/class/leds/lp5523\:kb3/brightness
chown system.system /sys/class/leds/lp5523\:kb4/brightness
chown system.system /sys/class/leds/lp5523\:kb5/brightness
chown system.system /sys/class/leds/lp5523\:kb6/brightness
chmod 664 /sys/class/leds/lp5523\:kb1/brightness
chmod 664 /sys/class/leds/lp5523\:kb2/brightness
chmod 664 /sys/class/leds/lp5523\:kb3/brightness
chmod 664 /sys/class/leds/lp5523\:kb4/brightness
chmod 664 /sys/class/leds/lp5523\:kb5/brightness
chmod 664 /sys/class/leds/lp5523\:kb6/brightness
chown system.system /sys/class/backlight/acx565akm/brightness
chown system.system /sys/class/leds/cover/brightness
chown system.system /sys/class/leds/keyboard/brightness
chown system.system /sys/class/graphics/fb0/blank
chmod 0660 /sys/class/backlight/acx565akm/brightness
chmod 0660 /sys/class/leds/cover/brightness
chmod 0660 /sys/class/leds/keyboard/brightness
chmod 0660 /sys/class/graphics/fb0/blank
chown bluetooth.bluetooth /sys/class/rfkill/rfkill0/type
chown bluetooth.bluetooth /sys/class/rfkill/rfkill0/state
chmod 0660                /sys/class/rfkill/rfkill0/state
/bin/chmod chmod -R /sys/class/rfkill/rfkill0
