#!/bin/bash

SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
cd $SCRIPTPATH/
./task $@
if [ "$(echo $?)" -ne 0 ]; then
    exit -1
else
    python3 make_graphics.py
fi
