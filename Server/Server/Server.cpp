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
using namespace PROTOCOL_TEST;

CINFO g_tClient[PROTOCOL_TEST::MAX_PLAYER]; // Clinet User Info
CRITICAL_SECTION cs;

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

void            send_packet(int id, void* p);                                   // ��Ŷ ���� �Լ�.
void            send_login_ok(int id);                                          // ���ο� Ŭ���̾�Ʈ �α��� ��Ŷ
void            send_enter_packet(int to, int id);
void            send_move_packet(int to, int id);
void            send_playerstance_packet(int to, int id, int stance, int dir);  // Player Animation ����.
void            send_scenechange_packet(int to, int id);                        // Player Scene ����.
void            send_createDungeonMonster(int to);                              // Monster ���� ����.
void            send_monsterinfo(int to, CMonster* src);                        // Server�� �����ϴ� Monster ����.

void            ProcessMove(int id, char dir);
bool            CheckSphere(const float& dst_x, const float& dst_y, const float& dst_radius,
                            const float& src_x, const float& src_y, const float& src_radius);


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

    // Err_Display() -> �ѱ��� ����
    wcout.imbue(std::locale("korean"));

    // Client Object Init
    for (auto& cl : g_tClient)
        cl.in_use = false;

    InitializeCriticalSection(&cs);

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
                                  SOCK_STREAM,  // TCP ��������
                                  0);

    if (INVALID_SOCKET == pListenSocket)
        err_quit("socket()");

    /*__________________________________________________________________________________________________________
	[ Bind ListenSocket ]
    ____________________________________________________________________________________________________________*/
	SOCKADDR_IN tServerAddr;
    ZeroMemory(&tServerAddr, sizeof(tServerAddr));
    tServerAddr.sin_family      = AF_INET;             // IPv4
    tServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);   // Ŭ���̾�Ʈ�� � IP�ּҷε� ���� ���.
    tServerAddr.sin_port        = htons(PROTOCOL_TEST::SERVER_PORT);  // ������ ���� ��Ʈ ��ȣ ����.

    retval = bind(pListenSocket,                       // Ŭ���̾�Ʈ ������ ������ �������� ���� ����.
                  (SOCKADDR*)&tServerAddr,             // ���� ���� ����ü.
                  sizeof(tServerAddr));                // ���� ���� ����ü�� ����.
    if (SOCKET_ERROR == retval)
        err_quit("bind()");

    /*__________________________________________________________________________________________________________
    [ Make ListenState ]
    ____________________________________________________________________________________________________________*/
    retval = listen(pListenSocket, // Ŭ���̾�Ʈ ������ ������ �������� ���� ����.
                    SOMAXCONN);    // ���� ������ Ŭ���̾�Ʈ ����. �Ϻ� �������ݿ��� ���� ������ �ִ�.
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

        pClientSocket = accept(pListenSocket,           // Ŭ���̾�Ʈ ������ ������ �������� ���� ����.
                               (SOCKADDR*)&tClientAddr, // ������ Ŭ���̾�Ʈ�� �ּ� ������ ä������.
                               &iAddrLen);              // accept() �Լ��� ä������ �ּ� ������ ũ��.

        if (INVALID_SOCKET == pClientSocket)
        {
            err_display("accept()");
            break;
        }

        /* ���ο� ���� ���� ó�� */
        int server_id = ConnectNewClient(pClientSocket);

        // 1Client - 1Thread Context Model.
        // Create Thread Context.
        hThread = CreateThread(NULL,                    // �ڵ� ��Ӱ� ���� ��ũ���� ����.
                               0,                       // �����忡 �Ҵ�Ǵ� ���� ũ��. �⺻ ���� 1MB.
                               ClientThread,            // ������ �Լ��� ���� �ּ�.
                               (LPVOID)server_id,       // ������ �Լ� ���� ����. 
                               0,                       // ������ ������ �����ϴ� ��.  0 �Ǵ� CREATE_SUSPENDED
                               NULL);                   // DWORD������ �����ϸ� ������ ID�� �����. �ʿ� ���ٸ� NULL.
        
        if (NULL == hThread)
            closesocket(pClientSocket);
        else
            CloseHandle(hThread);
    }

    DeleteCriticalSection(&cs);

    FreeMonsterInfo();

    // CloseSocket.
    closesocket(pListenSocket);
    WSACleanup();
}

// Client Thread.
DWORD __stdcall ClientThread(LPVOID arg)
{
    int retval{ 0 };
    
    // Ŭ���̾�Ʈ�� �Ҵ���� SERVER ID
    int server_id{ (int)arg };

    SOCKADDR_IN tClientAddr;
    ZeroMemory(&tClientAddr, sizeof(SOCKADDR_IN));
    int iAddrLen{ sizeof(tClientAddr) };

    while (true)
    {
      /*__________________________________________________________________________________________________________
        [ Data Receive ] :: Server <---- Client
        ____________________________________________________________________________________________________________*/
        int     flags = 0;
        char    net_buf[PROTOCOL_TEST::MAX_BUF_SIZE];
        int     ret = recv(g_tClient[server_id].m_sock,
                           net_buf,
                           MAX_BUF_SIZE, 
                           flags);

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
	g_pTimerFPS         = CTimer::Create();
	g_pTimerTimeDelta   = CTimer::Create();

    float fFrame        = 1.0f / 60.0f;
    float fTime         = 0.0f;

    while (true)
    {
        // Update Timer.
        if (nullptr != g_pTimerFPS)
            g_pTimerFPS->UpdateTimeDelta();

        // Limit FPS 60.
        fTime += g_pTimerFPS->GetTimeDelta();
        if (fTime >= fFrame)
        {
            fTime = 0.0f;

            if (nullptr != g_pTimerTimeDelta)
                g_pTimerTimeDelta->UpdateTimeDelta();

            // Monster Update.
            for (auto& monLst : g_monLst)
            {
                auto iter_begin = monLst.begin();
                auto iter_end   = monLst.end();

                for (; iter_begin != iter_end;)
                {
                    // Set Target.
                    CINFO* target = &g_tClient[0];
                    float  dist = 9999.0f;
                    for (int i = 0; i < MAX_PLAYER; ++i)
                    {
                        if (g_tClient[i].in_use)
                        {
                            // Player�� ���� SceneID�� ���� ��쿡�� Target ����. & MonsterInfo Send.
                            if (g_tClient[i].scene_id == (*iter_begin)->scene_id)
                            {
                                float w = g_tClient[i].x - (*iter_begin)->x;
                                float h = g_tClient[i].y - (*iter_begin)->y;
                                float result = sqrtf(w * w + h * h);

                                if (dist > result)
                                {
                                    dist = result;
                                    (*iter_begin)->SetTarget(&g_tClient[i]);
                                }

                                send_monsterinfo(i, *iter_begin);
                            }
                        }
                    }

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

            // Check Collision.
            for (int i = 0; i < PROTOCOL_TEST::MAX_PLAYER; ++i)
            {
                // ������� Client�� ������ Monster �浹�˻�.
                // Player�� ���°� Attack �̳� Skill�� �ƴ� ��쿡�� ��ŵ.
                if (g_tClient[i].in_use &&
                    (STANCE_ATTACK == g_tClient[i].cur_stance) ||
                    (STANCE_SKILL == g_tClient[i].cur_stance))
                {
					for (auto& monList : g_monLst)
					{
						for (auto& pMonster : monList)
						{
                            if (pMonster->is_dead)
                                continue;

							if (CheckSphere(g_tClient[i].attX, g_tClient[i].attY, g_tClient[i].att_radius,
								pMonster->x, pMonster->y, pMonster->radius))
							{
								if (SKILL_BASIC == g_tClient[i].cur_skill)
                                    pMonster->updatetime = 0.5f;
								else if (SKILL_MOON == g_tClient[i].cur_skill)
                                    pMonster->updatetime = 0.1f;
								else if (SKILL_MULTI == g_tClient[i].cur_skill)
                                    pMonster->updatetime = 0.15f;
								else if (SKILL_SOUL == g_tClient[i].cur_skill)
                                    pMonster->updatetime = 0.5f;

                                if (pMonster->is_collision)
                                {
                                    pMonster->is_collision = !pMonster->is_collision;

                                    // Monster Hp ����.
                                    pMonster->hp -= g_tClient[i].att;

                                    cout << "Collision Player - Monster" << endl;
                                }

							}

						}

					}

                }
            }

        }

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
    /* ���ο� ���� ���� �Ϸ� */
    int i;

    for (i = 0; i < PROTOCOL_TEST::MAX_PLAYER; ++i)
    {
        if (!g_tClient[i].in_use)
            break;
    }

    /* ���� �����ο� �ʰ� */
    if (PROTOCOL_TEST::MAX_PLAYER == i)
    {
#ifdef SHOW_LOG
        cout << "Max User limit exceeded." << endl;
#endif
        closesocket(ns);
    }
    else
    {
        /* ���� �Ϸ� */
#ifdef SHOW_LOG
        cout << "New Client Accepted [" << i << "]" << endl;
#endif

        /* ���� ������ ������ Ŭ���̾�Ʈ ��ü ���� ���� */
        /* 1) �ý��� ������ */
        //EnterCriticalSection(&cs);
        g_tClient[i].m_sock = ns;
        g_tClient[i].in_use = true;
        g_tClient[i].name[0] = 0;
        //LeaveCriticalSection(&cs);

        /* 2) ���� ������ */
        g_tClient[i].att        = 5'000;
        g_tClient[i].exp        = 0;
        g_tClient[i].hp         = 9'999;
        g_tClient[i].mp         = 9'999;
        g_tClient[i].level      = 1;
        g_tClient[i].sp         = 9'999;
        g_tClient[i].speed      = 100;
        g_tClient[i].radius     = 50.0f;

        g_tClient[i].scene_id   = SCENEID_TOWN;
        g_tClient[i].cur_stance = STANCE_IDLE;
        g_tClient[i].cur_dir    = DIR_DOWN;

        g_tClient[i].att_radius = 50.0f;

        /* ���ο� �÷��̾��� �ʱ� ��ġ �� ���� */
        g_tClient[i].x = 1005.f;
        g_tClient[i].y = 555.f;

    }
    return i;
}

void ProcessPacket(char* ptr, int server_id)
{
    ///* ��Ŷ Ÿ�� Ȯ�� */
    //char p_type = g_tClient[server_id].m_packet_buf[1];

    switch (ptr[1])
    {
    case CS_PACKET_LOGIN:
    {
        /* �α��� Ȯ�� ó�� */
        cs_packet_login* p = reinterpret_cast<cs_packet_login*>(ptr);

        //EnterCriticalSection(&cs);
        strcpy_s(g_tClient[server_id].name, p->name);
        //LeaveCriticalSection(&cs);

        /* �α��� Ȯ�� ó�� �Ϸ� �� �۾� */
        send_login_ok(server_id);

        /* �ٸ� �������� ���� ����� �˸� */
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

        // Scene�� ���� ��ġ�� Player ��ġ ����.
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
        cs_packet_playerstance* p = reinterpret_cast<cs_packet_playerstance*>(ptr);
        g_tClient[server_id].cur_stance = p->cur_stance;
        g_tClient[server_id].cur_dir    = p->cur_dir;

        for (int i = 0; i < PROTOCOL_TEST::MAX_PLAYER; ++i)
        {
            if (i != server_id)
                send_playerstance_packet(i, server_id, p->cur_stance, p->cur_dir);
        }

    }
    break;

    case CS_PACKET_PLAYERATTACK:
    {
        cs_packet_playerattack* p = reinterpret_cast<cs_packet_playerattack*>(ptr);
        g_tClient[server_id].cur_skill = p->cur_skill;
    }
    break;

    default:
        break;
    }
}

void ProcessData(char* net_buf, size_t io_byte, int server_id)
{
    char*           ptr               = net_buf;
    static size_t   in_packet_size    = 0;
    static size_t   saved_packet_size = 0;
    static char     packet_buffer[PROTOCOL_TEST::MAX_BUF_SIZE];

    while (0 != io_byte)
    {
        if (0 == in_packet_size) 
            in_packet_size = ptr[0];

        if (io_byte + saved_packet_size >= in_packet_size)
        {
            memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
            ProcessPacket(packet_buffer, server_id);

            ptr     += in_packet_size - saved_packet_size;
            io_byte -= in_packet_size - saved_packet_size;

            in_packet_size      = 0;
            saved_packet_size   = 0;
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
        cout << id << "�� Ŭ��� ������ ��Ŷ Ÿ��-" << static_cast<int>(packet[1]) << endl;
        cout << id << "�� Ŭ��� ��������� ��ŶŸ�� ũ��: " << static_cast<int>(packet[0]) << endl;
#endif
    }
}

void send_login_ok(int id)
{
    /* ���� �α��� ���� ä��� */
    sc_packet_login_ok p;

    p.size = sizeof(p);
    p.type = SC_PACKET_LOGIN_OK;

    p.att   = 5'000;
    p.exp   = 0;
    p.hp    = 9'999;
    p.mp    = 9'999;
    p.sp    = 9'999;
    p.id    = id;
    p.level = 1;
    p.speed = 100;
    p.x     = g_tClient[id].x;
    p.y     = g_tClient[id].y;

    p.scene_id = SCENEID_TOWN;

    /* �α��� ó�� �� �ش� Ŭ���̾�Ʈ���� ��Ŷ ���� */
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
    p.size          = sizeof(p);
    p.type          = SC_PACKET_PLAYERSTANCE;
    p.id            = id;
    p.cur_stance    = stance;
    p.cur_dir       = dir;

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
    p.cur_frame     = src->frame.frame_start;

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
        g_tClient[id].angle = 270.0f;

        break;
    case MV_DOWN:
        y += 2.5f;
        g_tClient[id].angle = 90.0f;

        break;
    case MV_LEFT:
        x -= 2.5f;
        g_tClient[id].angle = 180.0f;

        break;
    case MV_RIGHT:
        x += 2.5f;
        g_tClient[id].angle = 360.0f;

        break;
    default:
#ifdef SHOW_LOG
        cout << "Unknown Direction in CS_MOVE packet." << endl;
#endif
        while (true);

        break;
    }

    // �ش� Ŭ���̾�Ʈ�� ������ �� ��ǥ.
    g_tClient[id].x = x;
    g_tClient[id].y = y;

    // Player�� ���� Object�� �����ϴ� ��ġ ��ǥ.
    g_tClient[id].attX = g_tClient[id].x + cosf(g_tClient[id].angle * 3.141592f / 180.0f) * g_tClient[id].attlen;
    g_tClient[id].attY = g_tClient[id].y + sinf(g_tClient[id].angle * 3.141592f / 180.0f) * g_tClient[id].attlen;


    // �̵� Ű �Է��� ���� Ŭ���̾�Ʈ���� �ڽ��� ������ ����� �����ش�.
    send_move_packet(id, id);

    for (int i = 0; i < PROTOCOL_TEST::MAX_PLAYER; ++i)
    {
        if (i != id)
        {
            send_move_packet(i, id);
        }
    }
}

bool CheckSphere(const float& dst_x, const float& dst_y, const float& dst_radius, 
                 const float& src_x, const float& src_y, const float& src_radius)
{
    float sum_radius = dst_radius + src_radius;

    float w = dst_x - src_x;
    float h = dst_y - src_y;
    float d = sqrtf(w * w + h * h);

    return (sum_radius >= d);
}


void ReadyMonsterInfo()
{
    HANDLE hThread;
    hThread = CreateThread(NULL,                // �ڵ� ��Ӱ� ���� ��ũ���� ����.
                           0,                   // �����忡 �Ҵ�Ǵ� ���� ũ��. �⺻ ���� 1MB.
                           CollisionThread,     // ������ �Լ��� ���� �ּ�.
                           nullptr,             // ������ �Լ� ���� ����. 
                           0,                   // ������ ������ �����ϴ� ��.  0 �Ǵ� CREATE_SUSPENDED
                           NULL);               // DWORD������ �����ϸ� ������ ID�� �����. �ʿ� ���ٸ� NULL.
    CloseHandle(hThread);

    // MonsterCow.
    float pos = 390.f;
    for (int i = 0; i < 4; ++i)
    {
        CMonster* temp  = new CMonster;
        temp->type          = MON_COW;
        temp->idx           = i;
        temp->cur_stance    = MON_STANCE_IDLE;
        temp->pre_stance    = MON_STANCE_IDLE;
        temp->cur_state     = MON_STATE_REST;
        temp->cur_dir       = DIR_LEFT;
        temp->scene_id      = SCENEID_DUNGEON;

        temp->frame.frame_start = 0.0f;
        temp->frame.frame_end   = 5.0f;
        temp->frame.frame_speed = 5.0f;

        temp->hp            = 100'000;
        temp->att           = 10;
        temp->speed         = 100.0f;
        temp->exp           = 150;
        temp->cx            = 256.f;
        temp->cy            = 256.f;
        temp->x             = 1230.f;
        temp->y             = pos;
        temp->angle         = 90.0f;
        temp->radius        = 50.0f;
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