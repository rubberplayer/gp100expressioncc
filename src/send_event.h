/*
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
*/
#pragma once
#include <alsa/asoundlib.h>
void send_note(snd_seq_t *seq_handle, int out_port, int channel, int note, int velocity, int note_on) ;
void send_control(snd_seq_t *seq_handle, int out_port, int channel, int param, int value) ;