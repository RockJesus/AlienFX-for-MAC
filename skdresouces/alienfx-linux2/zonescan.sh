#!/bin/bash

# This script will help to determine what zone numbers control which
# lights on your AlienWare hardware.
#
# Version 3.

echo "Lights out!"
sudo ./alienfx -r
echo "Press enter key for next zone"
read
echo "ZONE 01 (press enter)"
sudo ./alienfx -r -v -c z01ffff
read
echo "ZONE 02 (press enter)"
sudo ./alienfx -r -v -c z02ffff
read
echo "ZONE 03 (press enter)"
sudo ./alienfx -r -v -c z03ffff
read
echo "ZONE 04 (press enter)"
sudo ./alienfx -r -v -c z04ffff
read
echo "ZONE 05 (press enter)"
sudo ./alienfx -r -v -c z05ffff
read
echo "ZONE 06 (press enter)"
sudo ./alienfx -r -v -c z06ffff
read
echo "ZONE 07 (press enter)"
sudo ./alienfx -r -v -c z07ffff
read
echo "ZONE 08 (press enter)"
sudo ./alienfx -r -v -c z08ffff
read
echo "ZONE 09 (press enter)"
sudo ./alienfx -r -v -c z09ffff
read
echo "ZONE 10 (press enter)"
sudo ./alienfx -r -v -c z10ffff
read
echo "ZONE 11 (press enter)"
sudo ./alienfx -r -v -c z11ffff
read
echo "ZONE 12 (press enter)"
sudo ./alienfx -r -v -c z12ffff
read
echo "ZONE 13 (press enter)"
sudo ./alienfx -r -v -c z13ffff
read
echo "ZONE 14 (press enter)"
sudo ./alienfx -r -v -c z14ffff
read
echo "ZONE 15 (press enter)"
sudo ./alienfx -r -v -c z15ffff
read
echo "ZONE 16 (press enter)"
sudo ./alienfx -r -v -c z16ffff
read
echo "ZONE 17 (press enter)"
sudo ./alienfx -r -v -c z17ffff
read
echo "ZONE 18 (press enter)"
sudo ./alienfx -r -v -c z18ffff
read
echo "ZONE 19 (press enter)"
sudo ./alienfx -r -v -c z19ffff
read
echo "ZONE 20 (press enter)"
sudo ./alienfx -r -v -c z20ffff
read
echo "ZONE 21 (press enter)"
sudo ./alienfx -r -v -c z21ffff
read
echo "ZONE 22 (press enter)"
sudo ./alienfx -r -v -c z22ffff
read
echo "ZONE 23 (press enter)"
sudo ./alienfx -r -v -c z23ffff
read
echo "ZONE 24 (press enter)"
sudo ./alienfx -r -v -c z24ffff
read
echo "ZONE 25 (press enter)"
sudo ./alienfx -r -v -c z25ffff
read
echo "ZONE 26 (press enter)"
sudo ./alienfx -r -v -c z26ffff
read
echo "ZONE 27 (press enter)"
sudo ./alienfx -r -v -c z27ffff
read
echo "Lights out!"
sudo ./alienfx -r
