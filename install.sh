#!/bin/bash
#############################################
# install program into filesystem
############################################

WHOAMI=$(whoami)

read -p "Who are you? (Your username) : " who

if [ "$WHOAMI" != "root" ]; then
	echo "[error] Before doing this , You Must Be ROOT...Abort"
	exit 1
fi

targetdir="/usr/local"

if [ -d "$targetdir/bubble" ]; then
	echo "[error] A problem happend when create diractory...Abort"
	exit 1
fi

if ! test -e "bin/bubblebin"; then
	echo "[error] Please Build Before Install...Abord"
	exit 1
fi

mkdir $targetdir/bubble

echo "copying file..."
cp -r bin $targetdir/bubble/
cp -r etc $targetdir/bubble/
cp -r sbin $targetdir/bubble/
cp -r webroot $targetdir/bubble/
cp -r log $targetdir/bubble/
cp sbin/bubble /usr/local/sbin/
chmod a+x /usr/local/sbin/bubble
chown -R $who /usr/local/bubble

echo "Install Success!\nThank you for using!"

exit 0
