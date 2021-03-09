#!/bin/bash
bat=$(cat /sys/class/power_supply/axp20x-battery/capacity)
echo "BAT: ${bat}%"
