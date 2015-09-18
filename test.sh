#!/bin/bash

SPEED=$(cat /proc/cpuinfo | grep "cpu MHz" | head -n 1 | cut -d ":" -f 2)
FAST=$(./repeat.sh $1)
./calc -s $SPEED $FAST
