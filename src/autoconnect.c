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
#include "autoconnect.h"
#include <stdio.h>

int auto_connect(snd_seq_t *seq_handle, int my_port) {
    snd_seq_client_info_t *client_info;
    snd_seq_port_info_t *port_info;
    int valeton_client = -1;
    int valeton_port = -1;
    
    // Allocate client and port info structures
    snd_seq_client_info_alloca(&client_info);
    snd_seq_port_info_alloca(&port_info);
    
    // Iterate through all clients
    snd_seq_client_info_set_client(client_info, -1);
    while (snd_seq_query_next_client(seq_handle, client_info) >= 0) {
        const char *client_name = snd_seq_client_info_get_name(client_info);
        int client_id = snd_seq_client_info_get_client(client_info);
        
        // Check if this is the VALETON GP-100
        if (strstr(client_name, "VALETON GP-100") != NULL) {
            fprintf(stderr,"Found at client %d\n", client_id);
            valeton_client = client_id;
            
            // Now find its output port
            snd_seq_port_info_set_client(port_info, client_id);
            snd_seq_port_info_set_port(port_info, -1);
            while (snd_seq_query_next_port(seq_handle, port_info) >= 0) {
                // Check if this is an output port (we want to connect to device's output)
                unsigned int caps = snd_seq_port_info_get_capability(port_info);
                if ((caps & SND_SEQ_PORT_CAP_READ) && 
                    (caps & SND_SEQ_PORT_CAP_SUBS_READ) &&
                    !(caps & SND_SEQ_PORT_CAP_NO_EXPORT)) {
                    
                    const char *port_name = snd_seq_port_info_get_name(port_info);
                    fprintf(stderr,"  Found output port '%s' at %d:%d\n", 
                           port_name,
                           snd_seq_port_info_get_client(port_info),
                           snd_seq_port_info_get_port(port_info));
                    
                    // Assuming the first suitable port is the one we want
                    valeton_port = snd_seq_port_info_get_port(port_info);
                    break;
                }
            }
            break;
        }
    }
    
    if (valeton_client == -1 || valeton_port == -1) {
        fprintf(stderr, "Could not find output port\n");
        return -1;
    }
    
    // Create the connection
    fprintf(stderr,"Connecting our client %d, port %d:0 to  %d:%d\n", 
           snd_seq_client_id(seq_handle), my_port,
           valeton_client, valeton_port);
    
    if (snd_seq_connect_from(seq_handle, my_port, valeton_client, valeton_port) < 0) {
        fprintf(stderr, "Connection failed: %s\n", snd_strerror(errno));
        return -1;
    }
    
    fprintf(stderr,"Successfully connected\n");
    return 0;
}