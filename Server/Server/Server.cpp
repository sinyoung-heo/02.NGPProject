#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <iostream>
#include <random>
#include <list>
#include "Protocol.h"
#include "Struct.h"
#include "Timer.h"
#include "Monster.h"

using namespace std;
using namespace PROTOCOL;
using namespace PROTOCOL_TEST;

CINFO g_tClient[PROTOCOL_TEST::MAX_PLAYER]; // Clinet User Info
//CRITICAL_SECTION cs;

CTimer* g_pTimerFPS         { nullptr };
CTimer* g_pTimerTimeDelta   { nullptr };

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

DWORD WINAPI    ClientThread(LPVOID arg);
DWORD WINAPI    CollisionThread(LPVOID arg);

int             ConnectNewClient(SOCKET ns);
void            ProcessPacket(char* ptr, int server_id);
void            ProcessData(char* net_buf, size_t io_byte, int server_id);

void            send_packet(int id, void* p);   // 패킷 전송 함수
void            send_login_ok(int id);          // 새로운 클라이언트 로그인 패킷
void            send_enter_packet(int to, int id);
void            send_move_packet(int to, int id);
void            send_playerstance_packet(int to, int id, int stance, int dir);
void            send_scenechange_packet(int to, int id);
void            send_createDungeonMonster(int to);
void            send_monsterinfo(int to, CMonster* src);

void            ProcessMove(int id, char dir);

#pragma region MONSTER
enum    MONID { COW, NINJA, BORIS, MONID_END };
typedef list<CMonster*> MONLIST;
typedef MONLIST::iterator   MONITER;

// Monster List.
MONLIST g_monLst[MONID_END];

void            ReadyMonsterInfo();
void            FreeMonsterInfo();
#pragma endregion

int main(int argc, char* argv[])
{
    // Ready Monster.
    ReadyMonsterInfo();

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
        int server_id = ConnectNewClient(pClientSocket);

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

    // DeleteCriticalSection(&cs);

    FreeMonsterInfo();

    // CloseSocket.
    closesocket(pListenSocket);
    WSACleanup();
}

// Client Thread.
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
       
        int flags = 0;
        char net_buf[PROTOCOL_TEST::MAX_BUF_SIZE];
        int ret = recv(g_tClient[server_id].m_sock, net_buf, MAX_BUF_SIZE, flags);


        ProcessData(net_buf, static_cast<size_t>(ret), server_id);
    }

    // Close Socket
    closesocket(g_tClient[server_id].m_sock);

    return 0;
}

// Monster Thread.
DWORD __stdcall CollisionThread(LPVOID arg)
{
    // Create Timer.
	g_pTimerFPS = CTimer::Create();
	g_pTimerTimeDelta = CTimer::Create();

    float fFrame    = 1.0f / 60.0f;
    float fTime     = 0.0f;

    while (true)
    {
        // Update Timer.
        if (nullptr != g_pTimerFPS)
            g_pTimerFPS->UpdateTimeDelta();
        if (nullptr != g_pTimerTimeDelta)
            g_pTimerTimeDelta->UpdateTimeDelta();
        
        // Send Monster Info To Client.
        for (auto& monLst : g_monLst)
        {
            for (auto& pMonster : monLst)
            {
                // Set Target.
                CINFO* target = &g_tClient[0];
                float  dist = 9999.0f;
                for (int i = 0; i < MAX_PLAYER; ++i)
                {
                    if (g_tClient[i].in_use)
                    {
                        // Player와 같은 SceneID에 있을 경우에만 Target 설정. & MonsterInfo Send.
                        if (g_tClient[i].scene_id == (pMonster)->scene_id)
                        {
                            float w = g_tClient[i].x - (pMonster)->x;
                            float h = g_tClient[i].y - (pMonster)->y;
                            float result = sqrtf(w * w + h * h);

                            if (dist > result)
                            {
                                dist = result;
                                (pMonster)->SetTarget(&g_tClient[i]);
                            }

                            send_monsterinfo(i, pMonster);
                        }
                    }
                }
            }
        }

        fTime += g_pTimerFPS->GetTimeDelta();
        if (fTime >= fFrame)
        {
            fTime = 0.0f;
            // ++iFPS;

            for (auto& monLst : g_monLst)
            {
                auto iter_begin = monLst.begin();
                auto iter_end   = monLst.end();

                for (; iter_begin != iter_end;)
                {
                    // Monster Update.
                    int iEvent = (*iter_begin)->UpdateMonster(g_pTimerTimeDelta->GetTimeDelta());
                    if (SERVER_DEADOBJ == iEvent)
                    {
                        if (*iter_begin)
                        {
                            delete (*iter_begin);
                            *iter_begin = nullptr;
                        }
                        iter_begin = monLst.erase(iter_begin);
                    }
                    else
                        ++iter_begin;
                }

            }

        }

        //// FPS 60 Limit.
        //if (iFPStime >= 1.0f)
        //{
        //    cout << iFPS << endl;
        //    iFPS = 0;
        //    iFPStime = 0.f;
        //}

    }

    // Free Timer.
    if (g_pTimerFPS)
    {
        delete g_pTimerFPS;
        g_pTimerFPS = nullptr;
    }

    if (g_pTimerTimeDelta)
    {
        delete g_pTimerTimeDelta;
        g_pTimerTimeDelta = nullptr;
    }

    return S_OK;
}

int ConnectNewClient(SOCKET ns)
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
#ifdef SHOW_LOG
        cout << "Max User limit exceeded." << endl;
#endif
        closesocket(ns);
    }
    else
    {
        /* 접속 완료 */
#ifdef SHOW_LOG
        cout << "New Client Accepted [" << i << "]" << endl;
#endif

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

void ProcessPacket(char* ptr, int server_id)
{
    ///* 패킷 타입 확인 */
    //char p_type = g_tClient[server_id].m_packet_buf[1];

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
#ifdef SHOW_LOG
        cout << "CS_PACKET_MOVE" << endl;
#endif
        cs_packet_move* p = reinterpret_cast<cs_packet_move*>(ptr);
        ProcessMove(server_id, p->direction);
    }
    break;

    // Scene ID
    case CS_PACKET_SCENECHANGE:
    {
        cs_packet_scenechange* p = reinterpret_cast<cs_packet_scenechange*>(ptr);
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

            // Create Monster - Cow & Ninja
            send_createDungeonMonster(server_id);

        }
        else if (SCENEID_BOSS == p->scene_id)
        {
            g_tClient[server_id].x = 1125.f;
            g_tClient[server_id].y = 705.f;

            // Create Monster - Boris.

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
#ifdef SHOW_LOG
        cout << "CS_PACKET_PLAYERSTANCE" << endl;
#endif
        cs_packet_playerstance* p = reinterpret_cast<cs_packet_playerstance*>(g_tClient[server_id].m_packet_buf);
        g_tClient[server_id].cur_stance = p->cur_stance;
        g_tClient[server_id].cur_dir = p->cur_dir;

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

void ProcessData(char* net_buf, size_t io_byte, int server_id)
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

#ifdef SHOW_LOG
        cout << id << "번 클라로 보내는 패킷 타입-" << static_cast<int>(packet[1]) << endl;
        cout << id << "번 클라로 보내줘야할 패킷타입 크기: " << static_cast<int>(packet[0]) << endl;
#endif
    }
}

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

void send_playerstance_packet(int to, int id, int stance, int dir)
{
    sc_packet_playerstance p;
    p.size = sizeof(p);
    p.type = SC_PACKET_PLAYERSTANCE;
    p.id = id;
    p.cur_stance = stance;
    p.cur_dir = dir;

    send_packet(to, &p);
}

void send_scenechange_packet(int to, int id)
{
    sc_packet_scenechange p;
    p.size = sizeof(p);
    p.type = SC_PACKET_SCENECHANGE;

    p.id = id;
    p.x = g_tClient[id].x;
    p.y = g_tClient[id].y;
    p.scene_id = g_tClient[id].scene_id;

    send_packet(to, &p);
}

void send_createDungeonMonster(int to)
{
    for (auto& pCow : g_monLst[COW])
    {
        sc_packet_monstercreateinfo p;
        p.size          = sizeof(p);
        p.type          = SC_PACKET_MONSTERCREATE;
       
        p.montype       = pCow->type;
        p.idx           = pCow->idx;
        p.hp            = pCow->hp;
        p.att           = pCow->att;
        p.exp           = pCow->exp;
        p.x             = pCow->x;
        p.y             = pCow->y;
        p.cx            = pCow->cx;
        p.cy            = pCow->cy;
        p.speed         = pCow->speed;
        p.angle         = pCow->angle;
        p.is_dead       = pCow->is_dead;

        p.scene_id      = pCow->scene_id;
        p.cur_stance    = pCow->cur_stance;
        p.cur_state     = pCow->cur_state;
        p.cur_dir       = pCow->cur_dir;

        send_packet(to, &p);
    }
}

void send_monsterinfo(int to, CMonster* src)
{
    sc_packet_monsterinfo p;
    p.size          = sizeof(p);
    p.type          = SC_PACKET_MONSTERINFO;

    p.montype       = src->type;
    p.idx           = src->idx;
    p.hp            = src->hp;
    p.x             = src->x;
    p.y             = src->y;
    p.angle         = src->angle;
    p.is_dead       = src->is_dead;
    p.cur_stance    = src->cur_stance;

    send_packet(to, &p);
}

void ProcessMove(int id, char dir)
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
#ifdef SHOW_LOG
        cout << "Unknown Direction in CS_MOVE packet." << endl;
#endif
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


void ReadyMonsterInfo()
{
    HANDLE hThread;
    hThread = CreateThread(NULL,                // 핸들 상속과 보안 디스크립터 정보.
                           0,                   // 스레드에 할당되는 스택 크기. 기본 값은 1MB.
                           CollisionThread,     // 스레드 함수의 시작 주소.
                           nullptr,             // 스레드 함수 전달 인자. 
                           0,                   // 스레드 생성을 제어하는 값.  0 또는 CREATE_SUSPENDED
                           NULL);               // DWORD변수를 전달하면 스레드 ID가 저장됨. 필요 없다면 NULL.
    CloseHandle(hThread);

    // MonsterCow.
    float pos = 390.f;
    for (int i = 0; i < 4; ++i)
    {
        CMonster* temp  = new CMonster;
        temp->type          = MON_COW;
        temp->idx           = i;
        temp->cur_stance    = MON_STANCE_IDLE;
        temp->cur_state     = MON_STATE_REST;
        temp->cur_dir       = DIR_LEFT;
        temp->scene_id      = SCENEID_DUNGEON;

        temp->hp            = 100'000;
        temp->att           = 10;
        temp->speed         = 300.0f;
        temp->exp           = 150;
        temp->cx            = 256.f;
        temp->cy            = 256.f;
        temp->x             = 1230.f;
        temp->y             = pos;
        temp->angle         = 90.0f;
        temp->is_dead       = false;
        temp->target        = nullptr;

        g_monLst[COW].push_back(temp);
        pos += 100.f;
    }

}

void FreeMonsterInfo()
{
    for (auto& monLst : g_monLst)
    {
        for (auto& pMonInfo : monLst)
        {
            if (pMonInfo)
            {
                delete pMonInfo;
                pMonInfo = nullptr;
            }
        }
   }
}