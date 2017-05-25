#!/bin/bash


# Sur PC : reset des arduinos
# (sur RPi ça sera redémarré donc bon)
if which ard-reset-arduino
then
    for ard in com/ard*
    do
        ard-reset-arduino $ard
    done
    sleep 1
fi

bin/premier

