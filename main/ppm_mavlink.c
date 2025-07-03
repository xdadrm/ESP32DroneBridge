#include "ppm_mavlink.h"
#include "ppm_decoder.h"
#include "db_protocol.h"
#include "globals.h"
#include "db_mavlink_msgs.h"

void send_rc_channels_override(int *tcp_clients, udp_conn_list_t *udp_conns) {
    if (!ppm_frame_ready) return;

    uint8_t buffer[FASTMAVLINK_MSG_RC_CHANNELS_OVERRIDE_FRAME_LEN_MAX];
    
    // Convert PPM values to MAVLink format
    uint16_t len = fmav_msg_rc_channels_override_pack_to_frame_buf(
        buffer,
        255, 0,
        1,
        0,
        ppm_channels[0],
        ppm_channels[1],
        ppm_channels[2],
        ppm_channels[3],
        ppm_channels[4],
        ppm_channels[5],
        ppm_channels[6],
        ppm_channels[7],
        UINT16_MAX,
        UINT16_MAX,
        UINT16_MAX,
        UINT16_MAX,
        UINT16_MAX,
        UINT16_MAX,
        UINT16_MAX,
        UINT16_MAX,
        UINT16_MAX,
        UINT16_MAX,
        NULL
    );
    
    // Send via existing infrastructure
    db_send_to_all_clients(tcp_clients, udp_conns, buffer, len);
    
    ppm_frame_ready = false;
}