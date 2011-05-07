#!/bin/sh

SWAPPINESS=$(getprop persist.swap.swappiness)
SWAPDEV=/dev/block/$(getprop persist.swap.dev)

[ -z ${SWAPPINESS} ] && exit 0

if [ ${SWAPPINESS} -gt 0 ]
then
	echo "Enabling swap on ${SWAPDEV} with swapiness ${SWAPPINESS}"
	swapon ${SWAPDEV}
	echo ${SWAPPINESS} > /proc/sys/vm/swappiness
else
	echo "Disabling swap on ${SWAPDEV}"
	swapoff ${SWAPDEV}
fi
