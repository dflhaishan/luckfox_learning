#!/bin/bash
set -e

make clean
make
adb shell "mkdir -p /test"
adb push ./oled_main /test/oled_main
adb shell "chmod +x /test/oled_main"
adb shell "cd /test && ./oled_main"
