# Valeton GP-100 multi-effect expression pedal as a general midi controller

This multi-effect has an expression pedal. But it sends its value in a `MIDI sysex`.

This program parses the sysex and outputs a more convenient `MIDI control change` 

Output is a control change with `param 15` on `channel 1`, customizable using the command line arguments.

## usage

    ./gp100expressioncc \
        [--channel|-c N] \
        [--param|-p N] \
        [--autoconnect|-a] \

`autoconnect` tries to connect the expression pedal to gp100expressioncc input using the device name.

## compilation

    make

## details

The controller output 31 different sysex, so the cc outputs the same number of values between 0 and 127 (0,4,8,12,16,21...)

Note that the sysex values have been collected using a device with firmware v1.9.

## License

gp100expressioncc
Copyright (C) 2025 rubberplayer

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.