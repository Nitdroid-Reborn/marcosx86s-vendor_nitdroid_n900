# -*- SH -*-
#
# Functions for maemo jail subsystem
#

export MAEMO=/mnt/initfs
export DROID=/and

export INIT_FUNC
export JAIL_FUNC
export DONE_FUNC

export JAILED


#
# This function runs the Maemo part of this script
# by re-executing itself with JAILED=TRUE
#

run_maemo_part()
{
	export JAILED=TRUE

	${ENTER} ${MAEMO} ${DROID}/$0
}


#
# This function runs the Maemo part of this script
#
#   - with Android mounted on ${DROID}
#

maemo_part()
{
	source ${DROID}/system/maemo/etc/maemo.sh

	${JAIL_FUNC}
}


#
# This function runs the Android parts of this script
#

android_part()
{
	source /system/maemo/etc/android.sh

	${INIT_FUNC}

	run_maemo_part

	${DONE_FUNC}
}


#
# Main function
#

MAIN()
{
	cd /

	if [ -z "${DONE_FUNC}" ]
	then
		ENTER='exec chroot'
	else
		ENTER='chroot'
	fi

	case X${JAILED} in

	  XTRUE)  maemo_part	;;
	  *)      android_part	;;

	esac
}


MAIN

## End.
