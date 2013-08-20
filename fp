#!/bin/bash
#
# Finds the fixed point of a function
# Usage:
#
#     ./fp ./life start
#
DELAY=0.1
LAST_OUT=`mktemp`
CURR_OUT=`mktemp`
FUNC=$1
INITIAL_FILE=$2
cat $INITIAL_FILE > $LAST_OUT
while :
do
    sleep $DELAY
    $FUNC < $LAST_OUT > $CURR_OUT
    diff $LAST_OUT $CURR_OUT > /dev/null
    if [ $? -eq 0 ]
    then
        # print the fixed point
        cat $CURR_OUT
        exit 1
    else
        echo "Not a fixed point." 1>&2
        cat $CURR_OUT 1>&2
        cat $CURR_OUT > $LAST_OUT
    fi
done
