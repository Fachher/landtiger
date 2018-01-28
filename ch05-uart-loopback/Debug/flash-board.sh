#!/bin/bash

/Applications/SEGGER/JLink/JLinkExe << EOF
exec EnableFlashDL
exec Device=LPC1768
speed 400
h
loadbin ./led-lpc17xx.bin 0x0000
r
g
exit
EOF

