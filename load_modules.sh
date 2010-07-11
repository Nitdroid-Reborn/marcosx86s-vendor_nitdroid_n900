#!/system/bin/sh

# clock
    insmod /system/lib/modules/rtc-twl4030.ko

# ambient light sensor
    insmod /system/lib/modules/tsl2563.ko

# battery related
    insmod /system/lib/modules/power_supply.ko
# bq27x00_battery loaded by batt_mon because bme fails to start
# with module loaded
####insmod /system/lib/modules/bq27x00_battery.ko

# leds
    insmod /system/lib/modules/led-class.ko
    insmod /system/lib/modules/leds-lp5523.ko
    insmod /system/lib/modules/leds-twl4030-vibra.ko

# video out
    insmod /system/lib/modules/nokia-av.ko

# touchscreen
   insmod /system/lib/modules/tsc2005.ko

# watchdog
    insmod /system/lib/modules/omap_wdt.ko
    insmod /system/lib/modules/twl4030_wdt.ko

# omap ssi
    insmod /system/lib/modules/phonet.ko
    insmod /system/lib/modules/pn_pep.ko
    insmod /system/lib/modules/omap_ssi.ko

# video 4 linux
    insmod /system/lib/modules/v4l1-compat.ko
    insmod /system/lib/modules/v4l1-common.ko
    insmod /system/lib/modules/v4l2-int-device.ko
# net
#    insmod /system/lib/modules/x_tables.ko
#    insmod /system/lib/modules/xt_tcpudp.ko
#    insmod /system/lib/modules/ip_tables.ko
#    insmod /system/lib/modules/iptable_filter.ko
#    insmod /system/lib/modules/iphb.ko
#    insmod /system/lib/modules/sunrpc.ko
#    insmod /system/lib/modules/auth_rpcgss.ko
#    insmod /system/lib/modules/rpcsec_gss_krb5.ko


# bluetooth
#    insmod /system/lib/modules/bluetooth.ko
#    insmod /system/lib/modules/l2cap.ko
#    insmod /system/lib/modules/bnep.ko
#    insmod /system/lib/modules/hidp.ko
#    insmod /system/lib/modules/sco.ko
#    insmod /system/lib/modules/rfcomm.ko
#    insmod /system/lib/modules/btsdio.ko
#    insmod /system/lib/modules/rfkill.ko
#    insmod /system/lib/modules/hci_h4p.ko

# high speed mmc
    insmod /system/lib/modules/omap_hsmmc.ko

# iommu
    insmod /system/lib/modules/omap3-iommu.ko

# sensors
    insmod /system/lib/modules/smiaregs.ko
    insmod /system/lib/modules/smia-sensor.ko

# ssi mcsab
    insmod /system/lib/modules/ssi_mcsaab_imp.ko

# dsp
    insmod /system/lib/modules/bridgedriver.ko
    insmod /system/lib/modules/dspbridge.ko

# isp
    insmod /system/lib/modules/iommu.ko
    insmod /system/lib/modules/iommu2.ko
    insmod /system/lib/modules/iovmm.ko
    insmod /system/lib/modules/videobuf-core.ko
    insmod /system/lib/modules/videobuf-dma-sg.ko
    insmod /system/lib/modules/isp-mod.ko

# camera
#    insmod /system/lib/modules/et8ek8.ko
#    insmod /system/lib/modules/board-rx51-camera.ko

# file storage
#    insmod /system/lib/modules/g_file_storage.ko

# nokia usb
    insmod /system/lib/modules/g_nokia.ko
    insmod /system/lib/modules/pn_pep.ko

# power vr
    insmod /system/lib/modules/omaplfb.ko
    insmod /system/lib/modules/pvrsrvkm.ko

# video devices
    insmod /system/lib/modules/videodev.ko
    insmod /system/lib/modules/compat_ioctl32.ko
#    insmod /system/lib/modules/radio-bcm2048.ko

# fm transmitter
#    insmod /system/lib/modules/fmtx-si4713.ko

# autofocus
#    insmod /system/lib/modules/ad5820.ko

# torch
#    insmod /system/lib/modules/adp1653.ko

#    insmod /system/lib/modules/omap_previewer_hack.ko

# needed for wl12xx
    insmod /system/lib/modules/crc7.ko

#    insmod /system/lib/modules/sdio_uart.ko
#    insmod /system/lib/modules/omap34xxcam-mod.ko

# accelerometer
    insmod /system/lib/modules/lis302dl.ko

# adb
#    insmod /system/lib/modules/g_android.ko

/bin/busybox ifconfig usb0 192.168.2.15 up
