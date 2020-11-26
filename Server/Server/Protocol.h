#pragma once

namespace PROTOCOL_TEST
{
// #define SHOW_LOG 0

#define SERVER_NOEVENT 0
#define SERVER_DEADOBJ 1

    constexpr unsigned int  SERVER_PORT = 9000;
    constexpr char          SERVER_IP[32] = "127.0.0.1";
    // constexpr char SERVER_IP[32] = "192.168.141.140";

    constexpr int MAX_BUF_SIZE  = 4'096;
    constexpr int MAX_ID_LEN    = 64;
    constexpr int MAX_PLAYER    = 3;
    constexpr int WORLD_WIDTH   = 800;
    constexpr int WORLD_HEIGHT  = 600;
    constexpr int MAX_STR_LEN   = 100;

    // Monster Type.
    constexpr int MON_COW                   = 0;
    constexpr int MON_NINJA                 = 1;
    constexpr int MON_BORIS                 = 2;

    // Monster Stance.
    constexpr int MON_STANCE_IDLE           = 0;
    constexpr int MON_STANCE_RUN            = 1;
    constexpr int MON_STANCE_ATTACK         = 2;
    constexpr int MON_STANCE_DEAD           = 3;
    constexpr int MON_STANCE_SKILL          = 4;
    constexpr int MON_STANCE_PORTAL_ATTACK  = 5;

    // Monster State.
    constexpr int MON_STATE_REST            = 0;
    constexpr int MON_STATE_ANGRY           = 1;
    constexpr int MON_STATE_RAGE            = 2;
    constexpr int MON_STATE_CRAZY           = 3;


#pragma pack(push,1)
    constexpr char SC_PACKET_LOGIN_OK       = 0;
    constexpr char SC_PACKET_MOVE           = 1;
    constexpr char SC_PACKET_ENTER          = 2;
    constexpr char SC_PACKET_LEAVE          = 3;
    constexpr char SC_PACKET_SCENECHANGE    = 4;
    constexpr char SC_PACKET_PLAYERSTANCE   = 5;
    constexpr char SC_PACKET_MONSTERCREATE  = 6;
    constexpr char SC_PACKET_MONSTERINFO    = 7;

    constexpr char CS_PACKET_LOGIN          = 0;
    constexpr char CS_PACKET_MOVE           = 1;
    constexpr char CS_PACKET_SCENECHANGE    = 2;
    constexpr char CS_PACKET_PLAYERSTANCE   = 3;
    constexpr char CS_PACKET_PLAYERATTACK   = 4;


    // SERVER -> CLIENT
    struct sc_packet_login_ok
    {
        char    size;
        char    type;

        int     level;
        int     id;
        float   x, y;
        int     hp;
        int     mp;
        int     exp;
        int     sp;
        int     att;
        float   speed;
        char    scene_id;
    };

    struct sc_packet_move
    {
        char    size;
        char    type;

        int     id;
        float   x, y;
    };

    struct sc_packet_enter
    {
        char    size;
        char    type;

        int     id;
        char    name[MAX_ID_LEN];
        char    o_type;
        float   x, y;
    };

    struct sc_packet_leave
    {
        char    size;
        char    type;

        int     id;
    };


    struct sc_packet_playerstance
    {
        char size;
        char type;

        int     id;
        int     cur_stance;
        int     cur_dir;
    };

    struct sc_packet_scenechange
    {
        char    size;
        char    type;

        int     id;
        float   x, y;
        int     scene_id;
    };

    struct sc_packet_monstercreateinfo
    {
        char    size;
        char    type;

        char    montype;        // Monster Type
        int     idx;            // Monster Idx
        int     hp;
        int     att;
        int     exp;
        float   x, y;
        float   cx, cy;
        float   speed;
        float   angle;
        bool    is_dead;

        char    scene_id;
        int     cur_stance;
        char    cur_state;
        char    cur_dir;
    };

    struct sc_packet_monsterinfo
    {
        char    size;
        char    type;

        char    montype;        // Monster Type
        int     idx;            // Monster Idx
        float   x, y;
        float   angle;
        int     cur_stance;
        int     hp;
        float   cur_frame;

        bool    is_dead;
    };

    ////////////////////////////////////////////////////////////////////////////////////////

    // CLIENT -> SERVER
    struct cs_packet_login
    {
        char size;
        char type;

        char name[MAX_ID_LEN];
    };


    constexpr char MV_UP = 0;
    constexpr char MV_DOWN = 1;
    constexpr char MV_LEFT = 2;
    constexpr char MV_RIGHT = 3;

    struct cs_packet_move
    {
        char size;
        char type;

        char direction;
    };


    constexpr char SCENEID_TOWN     = 3;
    constexpr char SCENEID_STORE    = 7;
    constexpr char SCENEID_DUNGEON  = 5;
    constexpr char SCENEID_BOSS     = 6;
    constexpr char SCENEID_FIELD    = 4;

    struct cs_packet_scenechange
    {
        char size;
        char type;

        char scene_id;
    };



    constexpr char STANCE_IDLE      = 0;
    constexpr char STANCE_WALK      = 1;
    constexpr char STANCE_RUN       = 2;
    constexpr char STANCE_ATTACK    = 3;
    constexpr char STANCE_SKILL     = 4;
    constexpr char STANCE_HIT       = 5;

    constexpr char DIR_DOWN         = 0;
    constexpr char DIR_UP           = 7;
    constexpr char DIR_LEFT         = 1;
    constexpr char DIR_RIGHT        = 4;

    struct cs_packet_playerstance
    {
        char size;
        char type;

        int cur_stance;
        int cur_dir;
    };



    constexpr char SKILL_SOUL   = 0;
    constexpr char SKILL_MOON   = 1;
    constexpr char SKILL_MULTI  = 2;
    constexpr char SKILL_BASIC  = 3;

    struct cs_packet_playerattack
    {
        char size;
        char type;

        char cur_skill;
    };
#pragma pack(pop)
}
