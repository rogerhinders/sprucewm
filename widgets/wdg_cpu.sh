#!/bin/bash
bat=$(cat /sys/class/thermal/thermal_zone0/temp)
bat=$((bat/1000))
echo "CPU: ${bat}c"
