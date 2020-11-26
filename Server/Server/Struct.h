#pragma once
#include <WinSock2.h>
#include "Protocol.h"
using namespace PROTOCOL_TEST;

typedef struct tagClientInfo
{
    /* ���� ������ */
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
    float   angle;  // Attack ����.
    float   attlen; // Attack ��ġ
    float   attX;   // Attack ��ġ X��ǥ.
    float   attY;   // Attack ��ġ Y��ǥ.
    float   att_radius;

    char    scene_id;
    char    cur_stance;
    char    cur_dir;
    char    cur_skill;

    /* �ý��� ������ */
    bool    in_use;
    SOCKET  m_sock;
    char    m_packet_buf[MAX_BUF_SIZE];

} CINFO;
