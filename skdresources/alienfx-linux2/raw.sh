#!/bin/sh
#
#  AlienFX raw command mode.  Insight into the raw USB inputs...
#

# Reboot lightchip
sudo ./alienfx -X 2 12 0 0 0 0 0 0 0

# Reset all lights OFF
sudo ./alienfx -X 2 7 3 0 0 0 0 0 0

# Reset all lights ON
sudo ./alienfx -X 2 7 4 0 0 0 0 0 0

# Set morph colour, 3rd-5th are zones, 6th-8th are colours, 9th is apparently brightness
sudo ./alienfx -X 2 1 255 255 255 255 128 0 0

# Set blink colour, 3rd-5th are zones, 6th-7th is colour
sudo ./alienfx -X 2 2 255 255 255 255 128 0 0

# Set fixed colour, 3rd-5th are zones, 6th-7th is colour
sudo ./alienfx -X 2 3 255 255 255 255 128 0 0

# Get Status (should return "16" for which means OK)
sudo ./alienfx -X 2 6 0 0 0 0 0 0 0

# Signal end of colour commands
sudo ./alienfx -X 2 4 0 0 0 0 0 0 0

# Execute previous colour commands
sudo ./alienfx -X 2 5 0 0 0 0 0 0 0
