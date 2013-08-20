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
if [[ $3 ]]
then
    VERBOSE=0
else
    VERBOSE=1
fi

cat $INITIAL_FILE > $LAST_OUT
while :
do
    if [[ $VERBOSE -eq 0 ]]; then
        sleep $DELAY
    fi
    $FUNC < $LAST_OUT > $CURR_OUT
    diff $LAST_OUT $CURR_OUT > /dev/null
    if [ $? -eq 0 ]
    then
        # print the fixed point
        cat $CURR_OUT
        exit 1
    else
        if [[ $VERBOSE -eq 0 ]]; then
            cat $CURR_OUT 1>&2
        fi
        cat $CURR_OUT > $LAST_OUT
    fi
done
