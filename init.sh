#!/system/bin/sh
export PATH=/sbin:/system/sbin:/system/bin:/system/xbin:/bin

echo "loading modules..."
/system/bin/load_modules.sh

# uncomment in case of troubleshooting of init process
#exec /bin/sh

# uncomment to delete old boot.log
#/system/bin/rm /boot.log

echo "runnning init..."
exec /init
