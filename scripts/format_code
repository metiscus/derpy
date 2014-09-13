#!/bin/bash

for file in `find src -name "*.cpp"`; 
do
	cp $file $file+".bak"
	clang-format -style=clang-format-config $file+".bak" > $file;
done