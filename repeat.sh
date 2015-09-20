#!/bin/bash
rm -f output.txt
for run in {1..50000}
do
	$1 > /dev/null
done

sort -h output.txt | head -n 1
