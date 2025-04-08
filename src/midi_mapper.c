#include <alsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "./send_event.h"
#include <stdio.h>
#include <inttypes.h>
#include <getopt.h>

snd_seq_t *seq_handle;
int in_port, out_port;

int output_control_channel;
int output_control_param;

#define RECORDED_PEDAL_VALUES_LENGTH 31
uint64_t recorded_pedal_values[] = {
    0x00000000,
    0xc7c6c63f,
    0x15159540,
    0x72720241,
    0x5a5a3a41,
    0x0c0c6c41,
    0xfaf99141,
    0xeeedad41,
    0xc7c6c641,
    0xbbbae241,
    0xafaefe41,
    0xc4c30b42,
    0xbebd1942,
    0xb8b72742,
    0x24243442,
    0x1e1e4242,
    0x8b8a4e42,
    0x85845c42,
    0x7e7e6a42,
    0xebea7642,
    0x72728242,
    0x6f6f8942,
    0xa6a58f42,
    0xa3a29642,
    0xa09f9d42,
    0xd6d5a342,
    0xd3d2aa42,
    0xd0cfb142,
    0x0606b842,
    0x0303bf42,
    0x3939c542
};
uint8_t controller_0_127_value[RECORDED_PEDAL_VALUES_LENGTH];
void fill_controller_values_array(uint8_t* array, int start, int end, int count){
    for (int i = 0; i < count; ++i) {            
       array[i] = start + (i * (end - start)) / (count - 1);
    }    
}


int find_recorded_value_index(uint64_t pedal_value){
    for (int i = 0 ; i < RECORDED_PEDAL_VALUES_LENGTH ; i++){
        if (recorded_pedal_values[i] == pedal_value)
        return i;
    }
    return -1;
}

uint64_t get_nibbles_uint64_value(unsigned char *bytes){
    uint64_t pedal_value = 0;                
    for (size_t i = 0 ; i< 8 ;i++){
        pedal_value <<= 4;
        pedal_value |= bytes[i];
    }
    return pedal_value; 
}

void debug_print_sysex(snd_seq_event_t * ev){
    void *sysex_data = ev->data.ext.ptr;
    size_t sysex_len = ev->data.ext.len;
    unsigned char *bytes = (unsigned char *)sysex_data;    
    for (size_t i = 0 ; i < sysex_len ;i++){
        printf("%02x ",bytes[i]);
    }
    printf("\n");
}

bool map_event(snd_seq_event_t *ev) {    
    //int port = ev->dest.port;    
    if (ev->type == SND_SEQ_EVENT_NOTEON) {
        //return false;
      //  on_note_on(port, ev->data.note.channel, ev->data.note.note, ev->data.note.velocity);
    } else if (ev->type == SND_SEQ_EVENT_NOTEOFF) {
//        on_note_off(port, ev->data.note.channel, ev->data.note.note, ev->data.note.velocity);
    } else if (ev->type == SND_SEQ_EVENT_CONTROLLER) {
        //on_controller(port, ev->data.control.channel, ev->data.control.param, ev->data.control.value);
    } else if (ev->type == SND_SEQ_EVENT_SYSEX) {
        //printf("===============port:%d event type : 0x%x\n",port, ev->type);
        void *sysex_data = ev->data.ext.ptr;
        size_t sysex_len = ev->data.ext.len;
        unsigned char *bytes = (unsigned char *)sysex_data;                        
        if (sysex_len == 34){          
            int data_offset = 25;
            uint64_t pedal_value = get_nibbles_uint64_value(bytes + data_offset);
            int value_index = find_recorded_value_index(pedal_value);
            if (value_index == -1){
                //printf("Unknow position %" PRIx64 "\n",pedal_value);
            } else {
                int controller_value = controller_0_127_value[value_index];
                fprintf(stderr,"%" PRIx64 " -> %d -> %d \n",pedal_value,value_index,controller_value );
                send_control(seq_handle, out_port, output_control_channel, output_control_param, controller_value) ;
            }            
        }        
    } else {
        //printf("port:%d event type : 0x%x\n",port, ev->type);
    }
    return true;
}

void create_virtual_midi_device() {
    int err;
    if ((err = snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_DUPLEX, 0)) < 0) {
        fprintf(stderr, "Error opening ALSA sequencer: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }
    snd_seq_set_client_name(seq_handle, "MIDI Mapper");

    in_port = snd_seq_create_simple_port(seq_handle, "source",
                                                SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
                                                SND_SEQ_PORT_TYPE_MIDI_GENERIC);
    if (in_port < 0) {
        fprintf(stderr, "Error creating input source port: %s\n", snd_strerror(in_port));
        exit(EXIT_FAILURE);
    }

    out_port = snd_seq_create_simple_port(seq_handle, "output",
                                          SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
                                          SND_SEQ_PORT_TYPE_MIDI_GENERIC);
    if (out_port < 0) {
        fprintf(stderr, "Error creating output port: %s\n", snd_strerror(out_port));
        exit(EXIT_FAILURE);
    }

    fprintf(stderr,"Virtual MIDI device created. Input source port: %d, Output port: %d\n", in_port, out_port);
}

void process_midi_events() {
    snd_seq_event_t *ev;
    bool encore = true;
    while (encore) {
        snd_seq_event_input(seq_handle, &ev);
        encore = map_event(ev);
        snd_seq_free_event(ev);
    }
    fprintf(stderr,"bye");
}

//#include "./testjs.h"


int parse_arguments(int argc, char *argv[]) {
    int channel = 0;    // default
    int param = 15;     // default

    // Option structure for getopt_long
    static struct option long_options[] = {
        {"channel", required_argument, 0, 'c'},
        {"param",   required_argument, 0, 'p'},
        {"help",    no_argument,       0, 'h'},  
        {0, 0, 0, 0}  // Termination entry
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "c:p:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'c':
                channel = atoi(optarg);
                break;
            case 'p':
                param = atoi(optarg);
                break;
            case '?':
                // getopt_long already printed an error message
                return 1;
            case 'h':
            default:
                fprintf(stderr, "Usage: %s [--channel|-c N] [--param|-p N]\n", argv[0]);
                return 1;
        }
    }

    output_control_channel = channel;
    output_control_param = param;

    fprintf(stderr,"output control change param %d on channel %d\n", output_control_param, output_control_channel);
    return 0;
}

int main(int argc, char *argv[]) {
    if (parse_arguments( argc, argv)){
        return EXIT_FAILURE;
    }
    fill_controller_values_array(controller_0_127_value, 0,127,RECORDED_PEDAL_VALUES_LENGTH );
    create_virtual_midi_device();
    fprintf(stderr,"Listening for MIDI events...\n");
    process_midi_events();
    snd_seq_close(seq_handle);
    return 0;
}