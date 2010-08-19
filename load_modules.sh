#!/system/bin/sh
BDADDR="0cddef0a0e6d"
swapon /dev/block/mmcblk1p3
echo 25 > /proc/vm/swappiness
chmod 0777 /dev/pvrsrvkm
chown system.graphics /dev/pvrsrvkm
echo $BDADDR | sed 's/\(..\)/\1:/g;s/:$//' > /sys/devices/platform/hci_h4p/bdaddr
