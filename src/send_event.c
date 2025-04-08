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
#include "send_event.h"

void send_note(snd_seq_t *seq_handle, int out_port, int channel, int note, int velocity, int note_on) {
    snd_seq_event_t note_ev;
    snd_seq_ev_clear(&note_ev);  // Clear the event structure

    // Set the event type to NOTEON or NOTEOFF
    note_ev.type = note_on ? SND_SEQ_EVENT_NOTEON : SND_SEQ_EVENT_NOTEOFF;

    // Set the queue to direct
    note_ev.queue = SND_SEQ_QUEUE_DIRECT;

    // Set the source port (output port of our program)
    note_ev.source.port = out_port;

    // Set the destination (broadcast to all subscribers)
    note_ev.dest.client = SND_SEQ_ADDRESS_SUBSCRIBERS;
    note_ev.dest.port = SND_SEQ_ADDRESS_UNKNOWN;

    // Set the note event parameters
    note_ev.data.note.channel = channel;    // MIDI channel
    note_ev.data.note.note = note;          // MIDI note number
    note_ev.data.note.velocity = velocity;  // Velocity (0 for NOTEOFF)

    // Debug: Print the entire event structure
    // printf("Event details:\n");
    // printf("  Type: %d\n", note_ev.type);
    // printf("  Source: client %d, port %d\n", note_ev.source.client, note_ev.source.port);
    // printf("  Dest: client %d, port %d\n", note_ev.dest.client, note_ev.dest.port);
    // printf("  Channel: %d\n", note_ev.data.note.channel);
    // printf("  Note: %d\n", note_ev.data.note.note);
    // printf("  Velocity: %d\n", note_ev.data.note.velocity);

    // Send the event
    int err = snd_seq_event_output_direct(seq_handle, &note_ev);
    if (err < 0) {
        // fprintf(stderr, "Error sending note event: %s\n", snd_strerror(err));
    } else {
        // printf("Note event sent successfully.\n");
    }
}

void send_control(snd_seq_t *seq_handle, int out_port, int channel, int param, int value) {
    snd_seq_event_t control_ev;
    snd_seq_ev_clear(&control_ev);  // Clear the event structure

    // Set the event type to control change
    control_ev.type = SND_SEQ_EVENT_CONTROLLER;

    // 1
    control_ev.queue = SND_SEQ_QUEUE_DIRECT;

    // Set the source port (output port of our program)
    control_ev.source.port = out_port;

    // Set the destination (broadcast to all subscribers)
    control_ev.dest.client = SND_SEQ_ADDRESS_SUBSCRIBERS;
    control_ev.dest.port = SND_SEQ_ADDRESS_UNKNOWN;

    // Set the control change parameters
    // control_ev.data.control.channel = ev->data.note.channel;  // Use the same channel as the input event
    // control_ev.data.control.param = 11;                       // Control number
    // control_ev.data.control.value = 52;                       // Control value

    control_ev.data.control.channel = channel;  // Use the same channel as the input event
    control_ev.data.control.param = param;      // Control number
    control_ev.data.control.value = value;      // Control value

    // Debug: Print the entire event structure
    // printf("Event details:\n");
    // printf("  Type: %d\n", control_ev.type);
    // printf("  Source: client %d, port %d\n", control_ev.source.client, control_ev.source.port);
    // printf("  Dest: client %d, port %d\n", control_ev.dest.client, control_ev.dest.port);
    // printf("  Channel: %d\n", control_ev.data.control.channel);
    // printf("  Control: %d\n", control_ev.data.control.param);
    // printf("  Value: %d\n", control_ev.data.control.value);

    // Send the event
    int err = snd_seq_event_output_direct(seq_handle, &control_ev);
    if (err < 0) {
         fprintf(stderr, "Error sending control change event: %s\n", snd_strerror(err));
    } else {
         // printf("Control change event sent successfully.\n");
    }
}
