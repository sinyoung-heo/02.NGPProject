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

    // ���� �Լ� ���� ���.
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
}
