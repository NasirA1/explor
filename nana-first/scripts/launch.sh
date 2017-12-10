#! /bin/bash

#$1 is the output command, i.e. anything that outputs to stdout/stderr
if [ -z "$1" ] ; then
    echo -e "error: output command not supplied"
    exit 1
fi

stdbuf -oL -eL $1 2>&1 | ./nana-first

