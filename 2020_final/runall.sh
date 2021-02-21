#!/bin/bash

for file in input/*; do
	./run.sh "$file" > "${file/input/stats}"
done
