#ifndef PPM_MAVLINK_H
#define PPM_MAVLINK_H

#include "globals.h"

void send_rc_channels_override(int *tcp_clients, udp_conn_list_t *udp_conns);

#endif // PPM_MAVLINK_H