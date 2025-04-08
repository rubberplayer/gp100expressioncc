#pragma once
#include <alsa/asoundlib.h>
void send_note(snd_seq_t *seq_handle, int out_port, int channel, int note, int velocity, int note_on) ;
void send_control(snd_seq_t *seq_handle, int out_port, int channel, int param, int value) ;