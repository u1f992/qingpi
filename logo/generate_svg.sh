#!/bin/bash

#
# Generate icon.svg from icon.png
#
convert icon.png \
    -background white \
    -alpha remove \
    -colorspace Gray \
    icon-bg-white.png
convert icon-bg-white.png icon.pnm
potrace icon.pnm -s -o icon.svg
rm icon-bg-white.png icon.pnm

#
# Concat icon.png and string
#
convert \
    -background transparent \
    -fill black \
    -font Pretendo \
    -pointsize 500 \
    label:"NeoControllerMod" \
    string.png
convert -size 200x1 xc:transparent spacer.png
convert icon.png spacer.png string.png \
    -colorspace Gray \
    -gravity center \
    -background none \
    +append logo.png
rm string.png spacer.png

#
# Generate logo.svg from logo.png
#
convert logo.png \
    -background white \
    -alpha remove \
    logo-bg-white.png
convert logo-bg-white.png logo.pnm
potrace logo.pnm -s -o logo.svg
rm logo.png logo-bg-white.png logo.pnm

#
# Generate white version of logo.svg
#
sed 's/#000000/#ffffff/gI' logo.svg > logo-white.svg
