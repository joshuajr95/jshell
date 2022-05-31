#! /bin/bash

INCLUDE_DIR="include"
INFILE="builtin.h"
INPATH="$INCLUDE_DIR/$INFILE"

LINES=$(grep "^BUILTIN_TABLE" $INPATH)

for line in $LINES
do
    echo $line
done