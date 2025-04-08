# Valeton GP-100 multi-effect expression pedal as a general midi controller

This multi-effect has an expression pedal. But it sends its value in a `MIDI sysex`.

This program parses the sysex and outputs a more convenient `MIDI control change` 

Output is a control change with `param 15` on `channel 1`, customizable using the command line arguments.

## usage

    ./gp100expressioncc [--channel|-c N] [--param|-p N]

## compilation

    make

## details

The controller output 31 different sysex, so the cc outputs the same number of values between 0 and 127 (0,4,8,12,16,21...)

Note that the sysex values have been collected using a device with firmware v1.9.

