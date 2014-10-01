#!/bin/bash

ROOT=`pwd`
BINDINGSFILE=$ROOT/bindings_tmp.lua

cat $ROOT/scripts/prefix.lua > $BINDINGSFILE

for f in *.h 
do
    echo "Processing $f.."
    echo "#include \"$f\"" >> bindings_tmp.c
done

gcc -E bindings_tmp.c | grep -v '#' >> $BINDINGSFILE
rm -f bindings_tmp.c
echo "]]" >> $BINDINGSFILE

luajit -bg $BINDINGSFILE bindings_tmp.o
rm -f $BINDINGSFILE
