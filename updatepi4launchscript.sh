#!/bin/bash

filename="/etc/init.d/powertune"

if grep -q "Recovery" "$filename"; then
  echo "Recovery found in $filename, no replacement needed."
else
  echo "Recovery not found in $filename, replacing content with new text."
  echo "#!/bin/bash
export LC_ALL=en_US.utf8
export QT_QPA_EGLFS_PHYSICAL_WIDTH=155
export QT_QPA_EGLFS_PHYSICAL_HEIGHT=86
export QT_QPA_EGLFS_HIDECURSOR=1
export QT_QPA_EGLFS_ALWAYS_SET_MODE=1
export QT_QPA_EGLFS_KMS_ATOMIC=1
export QT_QPA_PLATFORM=eglfs

/home/pi/powertune-update.sh ||:

/home/pi/startdaemon.sh &
cd /opt/PowerTune
nohup ./PowertuneQMLGui -platform eglfs &
exit_status=0
while true; do
    pgrep -x "PowertuneQMLGui" > /dev/null
    if [ $? -eq 0 ]; then
    else
        cd /home/pi/Recovery/
		./Recovery -platform eglfs
        exit_status=$?
        break
    fi
    sleep 10
done

if [ $exit_status -ne 0 ]; then
    echo "PowertuneQMLGui did not exit properly, launching recovery script"
         cd /home/pi/Recovery/
		./Recovery -platform eglfs
else
    echo "PowertuneQMLGui exited properly"
fi" > "$filename"
fi
