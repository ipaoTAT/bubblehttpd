#!/bin/bash
#################################
# Do all the works
#################################


if [ "$(whoami)" != "root" ];then
	echo "Before doing this , you must be root...Abort"
	exit
fi

echo "Building..."
./build.sh
echo 
echo "Installing..."
./install.sh
echo 
echo "Cleaning Unused File..."

./clean.sh
echo "success!"
