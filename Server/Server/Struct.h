#pragma once
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

    char    scene_id;
    char    cur_stance;
    char    cur_dir;

    /* �ý��� ������ */
    bool    in_use;
    SOCKET  m_sock;
    char    m_packet_buf[MAX_BUF_SIZE];

}CINFO;
