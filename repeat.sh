#!/bin/bash
rm output.txt
for run in {1..10000}
do
	$1 > /dev/null
done

sort -h output.txt | head
