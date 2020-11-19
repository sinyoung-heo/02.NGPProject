#pragma once

namespace PROTOCOL
{
	constexpr u_int SERVER_PORT = 9000;
	constexpr char SERVER_IP[32] = "127.0.0.1";


    //void err_quit(char* msg)
    //{
    //    LPVOID lpMsgBuf;
    //    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
    //        NULL, WSAGetLastError(),
    //        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    //        (LPTSTR)&lpMsgBuf, 0, NULL);
    //    MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
    //    LocalFree(lpMsgBuf);
    //    exit(1);
    //}

    //// 소켓 함수 오류 출력.
    //void err_display(char* msg)
    //{
    //    LPVOID lpMsgBuf;
    //    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
    //        NULL, WSAGetLastError(),
    //        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    //        (LPTSTR)&lpMsgBuf, 0, NULL);
    //    printf("[%s] %s", msg, (char*)lpMsgBuf);
    //    LocalFree(lpMsgBuf);
    //}

    ////////////////////////////////////////////////////////////////////
    typedef struct sv_send
    {
        bool    is_connected;
        float   cx;
        float   cy;
        float   x;
        float   y;
        char    name[64];
        int     level;
        int     hp;
        int     mp;
        int     sp;
        int     att;
        float   speed;
        int     exp;

        int     stance;
        int     skill;

        int     dir;
    } SV_SEND;

    typedef struct sv_recv
    {
        bool is_connected;
        char input;
    } SV_RECV;

    ////////////////////////////////////////////////////////////////////

    typedef struct cl_recv
    {
        bool    is_connected;
        float   cx;
        float   cy;
        float   x;
        float   y;
        char    name[64];
        int     level;
        int     hp;
        int     mp;
        int     sp;
        int     att;
        float   speed;
        int     exp;

        int     stance;
        int     skill;

        int     dir;

    } CL_RECV;

    typedef struct cl_send
    {
        bool is_connected;
        char input;
    } CL_SEND;
}

namespace PROTOCOL_TEST
{
    constexpr u_int SERVER_PORT = 9000;
    constexpr char SERVER_IP[32] = "127.0.0.1";

    constexpr int MAX_BUF_SIZE = 1024;
    constexpr int MAX_ID_LEN = 64;
    constexpr int MAX_PLAYER = 3;
    constexpr int WORLD_WIDTH = 800;
    constexpr int WORLD_HEIGHT = 600;
    constexpr int MAX_STR_LEN = 100;

#pragma pack(push,1)
    constexpr char SC_PACKET_LOGIN_OK = 0;
    constexpr char SC_PACKET_MOVE = 1;
    constexpr char SC_PACKET_ENTER = 2;
    constexpr char SC_PACKET_LEAVE = 3;

    constexpr char CS_PACKET_LOGIN = 0;
    constexpr char CS_PACKET_MOVE = 1;

    /* SERVER -> CLIENT */
    struct sc_packet_login_ok
    {
        char size;
        char type;
        int level;
        int id;
        float x, y;
        int hp;
        int mp;
        int exp;
        int sp;
        int att;
        float  speed;
    };

    struct sc_packet_move
    {
        char size;
        char type;
        int id;
        float x, y;
    };

    struct sc_packet_enter
    {
        char size;
        char type;
        int id;
        char name[MAX_ID_LEN];
        char o_type;
        float x, y;
    };

    struct sc_packet_leave
    {
        char size;
        char type;
        int id;
    };

    /* CLIENT -> SERVER */
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
#pragma pack(pop)
}
