#pragma once
#include <WinSock2.h>
#include "Protocol.h"
using namespace PROTOCOL_TEST;

typedef struct tagClientInfo
{
    /* 霸烙 能刨明 */
    char    name[MAX_ID_LEN];
    int     level;
    int     hp;
    int     mp;
    int     exp;
    int     sp;
    int     att;
    float   x, y;
    float   speed;
    float   radius;

    // Attack Info.
    float   angle;  // Attack 规氢.
    float   attlen; // Attack 府摹
    float   attX;   // Attack 府摹 X谅钎.
    float   attY;   // Attack 府摹 Y谅钎.
    float   att_radius;

    char    scene_id;
    char    cur_stance;
    char    cur_dir;
    char    cur_skill;

    /* 矫胶袍 能刨明 */
    bool    in_use;
    SOCKET  m_sock;
    char    m_packet_buf[MAX_BUF_SIZE];

} CINFO;
