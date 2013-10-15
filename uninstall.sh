#!/bin/bash
###############################
# uninstall the program
###################################

targetdir="/usr/local"

if [ "$(whoami)" != root ]; then
	echo "[error] Before doing this , you must be root...Abort"
	exit
fi

/bin/rm -rf $targetdir/bubble
/bin/rm  /usr/local/sbin/bubble

echo -e "Remove Success!\n We will miss you..."
