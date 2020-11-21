#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <iostream>
#include <random>
#include "Protocol.h"
#include "Struct.h"

using namespace std;
using namespace PROTOCOL;
using namespace PROTOCOL_TEST;


CINFO g_tClient[PROTOCOL_TEST::MAX_PLAYER];
//CRITICAL_SECTION cs;

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

int             Connect_NewClient(SOCKET ns);
DWORD WINAPI    ClientThread(LPVOID arg);
int             ReceiveData(SOCKET s, char* buf, int len, int flags, u_short client_portnum);


/* 패킷 전송 함수 */
void send_packet(int id, void* p)
{
    char* packet = reinterpret_cast<char*>(p);

    //EnterCriticalSection(&cs);
    if (g_tClient[id].in_use)
    {
        int ret = send(g_tClient[id].m_sock, packet, packet[0], 0);
        //LeaveCriticalSection(&cs);

        if (SOCKET_ERROR == ret)
            err_quit("send()");

        cout << id << "번 클라로 보내는 패킷 타입-" << static_cast<int>(packet[1]) << endl;
        cout << id << "번 클라로 보내줘야할 패킷타입 크기: " << static_cast<int>(packet[0]) << endl;
    }
}

void send_enter_packet(int to, int id)
{
    sc_packet_enter p;

    p.size = sizeof(p);
    p.type = SC_PACKET_ENTER;

    p.id = id;
    //EnterCriticalSection(&cs);
    strcpy_s(p.name, g_tClient[id].name);
    //LeaveCriticalSection(&cs);
    p.o_type = 0;
    p.x = g_tClient[id].x;
    p.y = g_tClient[id].y;

    send_packet(to, &p);
}


/* 새로운 클라이언트 로그인 패킷 */
void send_login_ok(int id)
{
    /* 보낼 로그인 정보 채우기 */
    sc_packet_login_ok p;

    p.size = sizeof(p);
    p.type = SC_PACKET_LOGIN_OK;

    p.att = 0;
    p.exp = 0;
    p.hp = 100;
    p.mp = 100;
    p.id = id;
    p.level = 1;
    p.sp = 100;
    p.speed = 100;
    p.x = g_tClient[id].x;
    p.y = g_tClient[id].y;

    p.scene_id = SCENEID_TOWN;

    /* 로그인 처리 후 해당 클라이언트에게 패킷 전송 */
    send_packet(id, &p);
}

void send_move_packet(int to, int id)
{
    sc_packet_move p;
    p.size = sizeof(p);
    p.type = SC_PACKET_MOVE;

    p.id = id;
    p.x = g_tClient[id].x;
    p.y = g_tClient[id].y;

    send_packet(to, &p);
}

void send_scenechange_packet(int to, int id)
{
    sc_packet_scenechange p;
    p.size      = sizeof(p);
    p.type      = SC_PACKET_SCENECHANGE;

    p.id        = id;
    p.x         = g_tClient[id].x;
    p.y         = g_tClient[id].y;
    p.scene_id  = g_tClient[id].scene_id;

    send_packet(to, &p);
}

void process_move(int id, char dir)
{
    float x = g_tClient[id].x;
    float y = g_tClient[id].y;

    switch (dir)
    {
    case MV_UP:
        y -= 2.5f;
        break;
    case MV_DOWN:
        y += 2.5f;
        break;
    case MV_LEFT:
        x -= 2.5f;
        break;
    case MV_RIGHT:
        x += 2.5f;
        break;
    default:
        cout << "Unknown Direction in CS_MOVE packet." << endl;
        while (true);

        break;
    }

    /* 해당 클라이언트가 움직인 후 좌표 */
    g_tClient[id].x = x;
    g_tClient[id].y = y;

    /* 이동 키 입력을 보낸 클라이언트에게 자신의 움직인 결과를 보내준다. */
    send_move_packet(id, id);

    for (int i = 0; i < PROTOCOL_TEST::MAX_PLAYER; ++i)
    {
        if (i != id)
        {
            send_move_packet(i, id);
        }
    }
}

void ProcessPacket(char* ptr, int server_id)
{
    switch (ptr[1])
    {
    case CS_PACKET_LOGIN:
    {
        /* 로그인 확인 처리 */
        cs_packet_login* p = reinterpret_cast<cs_packet_login*>(g_tClient[server_id].m_packet_buf);

        //EnterCriticalSection(&cs);
        strcpy_s(g_tClient[server_id].name, p->name);
        //LeaveCriticalSection(&cs);

        /* 로그인 확인 처리 완료 후 작업 */
        send_login_ok(server_id);

        /* 다른 유저에게 접속 사실을 알림 */
        for (int i = 0; i < MAX_PLAYER; ++i)
        {
            if (g_tClient[i].in_use)
            {
                if (server_id != i)
                {
                    send_enter_packet(i, server_id);
                    send_enter_packet(server_id, i);
                }
            }
        }
    }
    break;

    case CS_PACKET_MOVE:
    {
        cs_packet_move* p = reinterpret_cast<cs_packet_move*>(g_tClient[server_id].m_packet_buf);
        process_move(server_id, p->direction);
    }
    break;
    default:
        break;
    }
}

void process_data(char* net_buf, size_t io_byte, int server_id)
{
    char* ptr = net_buf;
    static size_t in_packet_size = 0;
    static size_t saved_packet_size = 0;
    static char packet_buffer[PROTOCOL_TEST::MAX_BUF_SIZE];

    while (0 != io_byte)
    {
        if (0 == in_packet_size) in_packet_size = ptr[0];

        if (io_byte + saved_packet_size >= in_packet_size)
        {
            memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
            ProcessPacket(packet_buffer, server_id);
            ptr += in_packet_size - saved_packet_size;
            io_byte -= in_packet_size - saved_packet_size;
            in_packet_size = 0;
            saved_packet_size = 0;
        }
        else
        {
            memcpy(packet_buffer + saved_packet_size, ptr, io_byte);
            saved_packet_size += io_byte;
            io_byte = 0;
        }
    }
}

void send_playerstance_packet(int to ,int id, int stance, int dir)
{
    sc_packet_playerstance p;
    p.size          = sizeof(p);
    p.type          = SC_PACKET_PLAYERSTANCE;
    p.id            = id;
    p.cur_stance    = stance;
    p.cur_dir       = dir;

    send_packet(to, &p);
}


int main(int argc, char* argv[])
{
    // Err_Display() -> 한국어 지원
    wcout.imbue(std::locale("korean"));

    // Client Object Init
    for (auto& cl : g_tClient)
        cl.in_use = false;

    //InitializeCriticalSection(&cs);

	int retval = 0;

    /*__________________________________________________________________________________________________________
	[ Ready WSA ]
    ____________________________________________________________________________________________________________*/
	WSADATA tWSA;
	if (WSAStartup(MAKEWORD(2, 2), &tWSA))
		return 1;

    /*__________________________________________________________________________________________________________
	[ Ready ListenSocket ]
    ____________________________________________________________________________________________________________*/
	SOCKET pListenSocket = socket(AF_INET,      // IPv4
                                  SOCK_STREAM,  // TCP 프로토콜
                                  0);

    if (INVALID_SOCKET == pListenSocket)
        err_quit("socket()");

    /*__________________________________________________________________________________________________________
	[ Bind ListenSocket ]
    ____________________________________________________________________________________________________________*/
	SOCKADDR_IN tServerAddr;
    ZeroMemory(&tServerAddr, sizeof(tServerAddr));
    tServerAddr.sin_family      = AF_INET;             // IPv4
    tServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);   // 클라이언트의 어떤 IP주소로든 접속 허용.
    tServerAddr.sin_port        = htons(PROTOCOL_TEST::SERVER_PORT);  // 서버의 지역 포트 번호 설정.

    retval = bind(pListenSocket,                       // 클라이언트 접속을 수용할 목적으로 만든 소켓.
                  (SOCKADDR*)&tServerAddr,             // 소켓 구조 구조체.
                  sizeof(tServerAddr));                // 소켓 구조 구조체의 길이.
    if (SOCKET_ERROR == retval)
        err_quit("bind()");

    /*__________________________________________________________________________________________________________
    [ Make ListenState ]
    ____________________________________________________________________________________________________________*/
    retval = listen(pListenSocket, // 클라이언트 접속을 수용할 목적으로 만든 소켓.
                    SOMAXCONN);    // 접속 가능한 클라이언트 개수. 하부 프로토콜에서 지원 가능한 최댓값.
    if (SOCKET_ERROR == retval)
        err_quit("listen()");


    /*__________________________________________________________________________________________________________
    [ Ready ClientSocket ]
    ____________________________________________________________________________________________________________*/
    int         iAddrLen        = 0;
    SOCKET      pClientSocket   = NULL;
    SOCKADDR_IN tClientAddr;
    ZeroMemory(&tClientAddr, sizeof(SOCKADDR_IN));

    HANDLE hThread;

    /*__________________________________________________________________________________________________________
     [ Server MainLoop ]
    ____________________________________________________________________________________________________________*/
    while (true)
    {
        cout << "[ Server ] - listening ... " << endl;

        // Accept Client
        iAddrLen = sizeof(tClientAddr);

        pClientSocket = accept(pListenSocket,           // 클라이언트 접속을 수용할 목적으로 만든 소켓.
                               (SOCKADDR*)&tClientAddr, // 접속한 클라이언트의 주소 정보가 채워진다.
                               &iAddrLen);              // accept() 함수가 채워넣은 주소 정보의 크기.

        if (INVALID_SOCKET == pClientSocket)
        {
            err_display("accept()");
            break;
        }

        /* 새로운 유저 접속 처리 */
        int server_id = Connect_NewClient(pClientSocket);

        // 1Client - 1Thread Context Model.
        // Create Thread Context.
        hThread = CreateThread(NULL,                    // 핸들 상속과 보안 디스크립터 정보.
                               0,                       // 스레드에 할당되는 스택 크기. 기본 값은 1MB.
                               ClientThread,            // 스레드 함수의 시작 주소.
                               (LPVOID)server_id,       // 스레드 함수 전달 인자. 
                               0,                       // 스레드 생성을 제어하는 값.  0 또는 CREATE_SUSPENDED
                               NULL);                   // DWORD변수를 전달하면 스레드 ID가 저장됨. 필요 없다면 NULL.
        
        if (NULL == hThread)
            closesocket(pClientSocket);
        else
            CloseHandle(hThread);
    }

    //DeleteCriticalSection(&cs);

    // CloseSocket.
    closesocket(pListenSocket);
    WSACleanup();
}

DWORD __stdcall ClientThread(LPVOID arg)
{
    int retval{ 0 };
    
    // 클라이언트가 할당받은 SERVER ID
    int server_id{ (int)arg };

    SOCKADDR_IN tClientAddr;
    ZeroMemory(&tClientAddr, sizeof(SOCKADDR_IN));
    int iAddrLen{ sizeof(tClientAddr) };

    while (true)
    {
      /*__________________________________________________________________________________________________________
        [ Data Receive ] :: Server <---- Client
        ____________________________________________________________________________________________________________*/
        retval = ReceiveData(g_tClient[server_id].m_sock,               // 통신할 대상과 연결된 소켓.
                             g_tClient[server_id].m_packet_buf,         // 받은 데이터를 저장할 버퍼의 주소.
                             sizeof(g_tClient[server_id].m_packet_buf), // 수신 버퍼로부터 복사할 최대 데이터의 크기.
                             0,
                             ntohs(tClientAddr.sin_port));              // 클라이언트 포트번호.
        if (SOCKET_ERROR == retval)
        {
            err_display("recv()");
            break;
        }
        else if (0 == retval)
            continue;


        /* 패킷 타입 확인 */
        char p_type = g_tClient[server_id].m_packet_buf[1];

        switch (p_type)
        {
        case CS_PACKET_LOGIN:
        {
            /* 로그인 확인 처리 */
            cs_packet_login* p = reinterpret_cast<cs_packet_login*>(g_tClient[server_id].m_packet_buf);

            //EnterCriticalSection(&cs);
            strcpy_s(g_tClient[server_id].name, p->name);
            //LeaveCriticalSection(&cs);

            /* 로그인 확인 처리 완료 후 작업 */
            send_login_ok(server_id);

            /* 다른 유저에게 접속 사실을 알림 */
            for (int i = 0; i < MAX_PLAYER; ++i)
            {
                if (g_tClient[i].in_use)
                {
                    if (server_id != i)
                    {
                        send_enter_packet(i, server_id);
                        send_enter_packet(server_id, i);
                    }
                }
            }
        }
        break;

        case CS_PACKET_MOVE:
        {
            cout << "CS_PACKET_MOVE" << endl;
            cs_packet_move* p = reinterpret_cast<cs_packet_move*>(g_tClient[server_id].m_packet_buf);
            process_move(server_id, p->direction);
        }
        break;

        // Scene ID
        case CS_PACKET_SCENECHANGE:
        {
            cs_packet_scenechange* p = reinterpret_cast<cs_packet_scenechange*>(g_tClient[server_id].m_packet_buf);
            g_tClient[server_id].scene_id = p->scene_id;

            // Scene의 시작 위치로 Player 위치 조정.
            if (SCENEID_TOWN == p->scene_id)
            {
                g_tClient[server_id].x = 1005.f;
                g_tClient[server_id].y = 555.f;
            }
            else if (SCENEID_STORE == p->scene_id)
            {
                g_tClient[server_id].x = 1005.f;
                g_tClient[server_id].y = 765.f;
            }
            else if (SCENEID_DUNGEON == p->scene_id)
            {
                g_tClient[server_id].x = 735.f;
                g_tClient[server_id].y = 555.f;
            }
            else if (SCENEID_BOSS == p->scene_id)
            {
                g_tClient[server_id].x = 1125.f;
                g_tClient[server_id].y = 705.f;
            }
            else if (SCENEID_FIELD == p->scene_id)
            {
                g_tClient[server_id].x = 1515.f;
                g_tClient[server_id].y = 1035.f;
            }

            send_scenechange_packet(server_id, server_id);
            for (int i = 0; i < PROTOCOL_TEST::MAX_PLAYER; ++i)
            {
                if (i != server_id)
                {
                    send_scenechange_packet(i, server_id);
                }
            }

        }
        break;

        // Player Stance & Dir
        case CS_PACKET_PLAYERSTANCE:
        {
            cout << "CS_PACKET_PLAYERSTANCE"<< endl;
            cs_packet_playerstance* p = reinterpret_cast<cs_packet_playerstance*>(g_tClient[server_id].m_packet_buf);
            g_tClient[server_id].cur_stance = p->cur_stance;
            g_tClient[server_id].cur_dir    = p->cur_dir;

            for (int i = 0; i < PROTOCOL_TEST::MAX_PLAYER; ++i)
            {
                if (i != server_id)
                    send_playerstance_packet(i, server_id, p->cur_stance, p->cur_dir);
            }

        }
        break;
        default:
            break;
        }

    }

    // Close Socket
    closesocket(g_tClient[server_id].m_sock);

    return 0;
}

int Connect_NewClient(SOCKET ns)
{
    /* 새로운 유저 접속 완료 */
    int i;

    for (i = 0; i < PROTOCOL_TEST::MAX_PLAYER; ++i)
    {
        if (!g_tClient[i].in_use)
            break;
    }

    /* 접속 수용인원 초과 */
    if (PROTOCOL_TEST::MAX_PLAYER == i)
    {
        cout << "Max User limit exceeded." << endl;
        closesocket(ns);
    }
    else
    {
        /* 접속 완료 */
        cout << "New Client Accepted [" << i << "]" << endl;

        /* 새로 접속한 유저의 클라이언트 객체 정보 설정 */
        /* 1) 시스템 콘텐츠 */
        //EnterCriticalSection(&cs);
        g_tClient[i].m_sock = ns;
        g_tClient[i].in_use = true;
        g_tClient[i].name[0] = 0;
        //LeaveCriticalSection(&cs);

        /* 2) 게임 콘텐츠 */
        g_tClient[i].att        = 0;
        g_tClient[i].exp        = 0;
        g_tClient[i].hp         = 0;
        g_tClient[i].mp         = 0;
        g_tClient[i].level      = 0;
        g_tClient[i].sp         = 0;
        g_tClient[i].speed      = 0;

        g_tClient[i].scene_id   = SCENEID_TOWN;
        g_tClient[i].cur_stance = STANCE_IDLE;
        g_tClient[i].cur_dir    = DIR_DOWN;

        /* 새로운 플레이어의 초기 위치 값 설정 */
        g_tClient[i].x = 1005.f;
        g_tClient[i].y = 555.f;

    }
    return i;
}

int ReceiveData(SOCKET s, char* buf, int len, int flags, u_short client_portnum)
{
    int     received    = 0;
    char*   ptr         = buf;
    int     left        = len;
    int finish_size = 0;

    while (left > 0)
    {
        received = recv(s, ptr, left, flags);

        cout << "클라에서 받은 패킷 타입-" << static_cast<int>(ptr[1]) << endl;
        if (SOCKET_ERROR == received)
        {
            err_display("recv()");
        }

        else if (0 == received)
            break;

        if (received == static_cast<int>(ptr[0]))
            return received;

        left -= received;
        ptr += received;
    }
    return (len - left);
}
