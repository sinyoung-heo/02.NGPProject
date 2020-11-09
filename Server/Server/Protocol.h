#pragma once

namespace PROTOCOL
{
	constexpr u_int SERVER_PORT = 9000;
	constexpr char SERVER_IP[32] = "127.0.0.1";


    void err_quit(char* msg)
    {
        LPVOID lpMsgBuf;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL, WSAGetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf, 0, NULL);
        MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
        LocalFree(lpMsgBuf);
        exit(1);
    }

    // 소켓 함수 오류 출력.
    void err_display(char* msg)
    {
        LPVOID lpMsgBuf;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL, WSAGetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf, 0, NULL);
        printf("[%s] %s", msg, (char*)lpMsgBuf);
        LocalFree(lpMsgBuf);
    }

    typedef struct sv_send
    {
        bool is_connected;
        float cx;
        float cy;
        float x;
        float y;
        char name[64];
        int level;
        int hp;
        int mp;
        int sp;
        int att;
        float speed;
        int exp;

        int stance;
        int skill;

        int dir;
    }SV_SEND;

    typedef struct sv_recv
    {
        bool is_connected;
        char input;
    }SV_RECV;

    typedef struct cl_recv
    {
        bool is_connected;
        float cx;
        float cy;
        float x;
        float y;
        char name[64];
        int level;
        int hp;
        int mp;
        int sp;
        int att;
        float speed;
        int exp;

        int stance;
        int skill;

        int dir;
    }CL_RECV;

    typedef struct cl_send
    {
        bool is_connected;
        char input;
    }CL_SEND;
}
