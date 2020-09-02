# iio-uinput
Small tool to redirect iio accelerometers to evdev to allow using a iio acclereometer as a joystick

compile:
cmake .
make

run:
run iio-uinput as root with the iio device name you want to use, must be a acclereometer.

to get the names of avaibale iio devices list the /sys/bus/iio/devices/ directory
