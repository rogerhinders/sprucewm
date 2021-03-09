#!/bin/bash
bat=$(cat /sys/class/thermal/thermal_zone1/temp)
bat=$((bat/1000))
echo "GPU: ${bat}c"
