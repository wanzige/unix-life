#!/bin/bash
#
# Finds the fixed point of a function
# Usage:
#
#     ./fp ./life start
#
TMP=tmp
FUNC=$1
INITIAL=$2
echo $FUNC
echo "$INITIAL"
LAST_VAL=`"$FUNC" < $INITIAL`
echo "$LAST_VAL"
echo "$LAST_VAL" > $TMP

while :
do
    VAL=`$FUNC < $TMP`
    echo
    echo "$VAL"
    echo "$VAL" > $TMP
done
