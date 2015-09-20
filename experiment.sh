#!/bin/bash

set -e

REPEAT=repeat.sh
CALC=."/calc"
CLEAN="output.txt fastest.txt"
touch "$CLEAN"

echo "Building..."
make clean all > /dev/null

printf "Current cpu speed ... "
SPEED=$(cat /proc/cpuinfo | grep "cpu MHz" | head -n 1 | cut -d ":" -f 2)
echo "$SPEED MHz"

echo "+----------------------------------+"
echo "| All results are in nano seconds. |"
echo "+----------------------------------+"
printf "Base Measurement .... "
# Run the base test
FAST=$($REPEAT ./rdtsc)
./calc -s $SPEED $FAST

# Rebuild rdtsc
rm ./rdtsc
# echo "Rebuilding rdtsc..."
make BUILD_FLAGS="-DGETPID" rdtsc > /dev/null
printf "PID test: ........... "
FAST=$($REPEAT ./rdtsc)
./calc -s $SPEED $FAST

rm ./rdtsc
# echo "Rebuilding rdtsc..."
make BUILD_FLAGS="-DGETUID" rdtsc > /dev/null
printf "UID test ............ "
FAST=$($REPEAT ./rdtsc)
./calc -s $SPEED $FAST

printf "PThread self ........ "
FAST=$($REPEAT ./self)
./calc -s $SPEED $FAST

printf "Pthread join ........ "
FAST=$($REPEAT ./time_pthread)
./calc -s $SPEED $FAST

printf "FORK/WAIT ........... "
FAST=$($REPEAT ./time_proc)
./calc -s $SPEED $FAST

printf "Context Switch SIG .. "
FAST=$($REPEAT ./signal)
./calc -s $SPEED $FAST

printf "Context Switch Pipe . "
FAST=$($REPEAT ./pipe)
./calc -s $SPEED $FAST

printf "Threads Mutex ....... "
FAST=$($REPEAT ./mutex)
./calc -s $SPEED $FAST

printf "Threads Condition ... "
#FAST=$($REPEAT ./condvar)
#echo $FAST
#./calc -s $SPEED $FAST
# $REPEAT ./condvar
echo ""

printf "Fault time .......... "
FAST=$($REPEAT ./fault)
./calc -s $SPEED $FAST

rm -f $CLEAN
