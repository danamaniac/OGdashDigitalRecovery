#!/bin/sh
if [ -d /home/pi/Recovery ]; then
echo "Updating to latest source "
cd /home/pi/Recovery
qmake /home/pi/Recoverysrc
make -j4
else
echo "Create source directory and clone PowerTune Repo"
mkdir /home/pi/Recovery
cd /home/pi/Recovery
qmake /home/pi/Recoverysrc
make -j4
fi
if [ -f /home/pi/Recovery/Recovery ]; then
./updatepi4launchscript.sh
fi
