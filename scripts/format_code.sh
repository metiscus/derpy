#!/bin/bash

echo ==== Formatting Source ====
cp clang-format-config .clang-format
for file in `find ../src -name "*.cpp"`; 
do
	cp $file $file.bak
	clang-format -style="file" $file.bak > $file;
	diff $file $file.bak || echo $file was modified.
done

echo ==== Cleaning Up ====
rm -f .clang-format
find ../src -name "*.cpp.bak" -exec rm -f {} \;