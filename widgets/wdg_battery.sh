#!/bin/bash

status=$(cat /sys/class/power_supply/axp20x-battery/status) 

if [ $status == "Charging" ]; then
	echo "BAT: ${status}"
else
	bat=$(cat /sys/class/power_supply/axp20x-battery/capacity)
	echo "BAT: ${bat}%"
fi
