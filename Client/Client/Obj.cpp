#include "stdafx.h"
#include "Obj.h"
#include "COthers.h"

CObj::CObj()
	:m_fAngle(0.f), m_bLateInit(false), m_bIsDead(false), m_eLayer(Layer::GAMEOBJECT)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tColInfo, sizeof(COLLINFO));
	ZeroMemory(&m_tColRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CObj::CObj(const INFO & rInfo)
	:m_tInfo(rInfo)
{
}


CObj::~CObj()
{
}

void CObj::LateInit()
{
	if (!m_bLateInit)
	{
		this->LateInit();
		m_bLateInit = true;
	}
}

void CObj::UpdateRect()
{
	m_tRect.left   = LONG(m_tInfo.fX - m_tInfo.fCX * 0.5f);
	m_tRect.top    = LONG(m_tInfo.fY - m_tInfo.fCY * 0.5f);
	m_tRect.right  = LONG(m_tInfo.fX + m_tInfo.fCX * 0.5f);
	m_tRect.bottom = LONG(m_tInfo.fY + m_tInfo.fCY * 0.5f);
}

/*충돌박스 업데이트*/
void CObj::UpdateColRect()
{
	m_tColRect.left		= LONG(m_tColInfo.fX - m_tColInfo.fCX * 0.5f);
	m_tColRect.top		= LONG(m_tColInfo.fY - m_tColInfo.fCY * 0.5f);
	m_tColRect.right	= LONG(m_tColInfo.fX + m_tColInfo.fCX * 0.5f);
	m_tColRect.bottom	= LONG(m_tColInfo.fY + m_tColInfo.fCY * 0.5f);
}
/*충돌박스 X,Y좌표 업데이트*/
void CObj::UpdateColXYPos()
{
	m_tColInfo.fX = m_tInfo.fX;
	m_tColInfo.fY = m_tInfo.fY;
}

/*이미지 재생*/
void CObj::MoveFrame()
{
	if (m_tFrame.dwOldTime + m_tFrame.dwFrameSpd < GetTickCount())
	{
		m_tFrame.iFrameStart++;
		m_tFrame.dwOldTime = GetTickCount();
	}

	if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		m_tFrame.iFrameStart = 0;
}

int CObj::ReceiveData(SOCKET s, char* buf, int len, int flags)
{
	int     received    = 0;
    char*   ptr         = buf;
    int     left        = len;
	

    while (left > 0)
    {
        received = recv(s,      // 통신할 대상과 연결된 소켓.
                        ptr,    // 받은 데이터를 저장할 버퍼의 주소.
                        left,   // 수신 버퍼로부터 복사할 최대 데이터의 크기.
                        flags);

		if (received == static_cast<int>(ptr[0]))
		{
			cout << "서버에서 받은 패킷 타입-" << static_cast<int>(buf[1]) << endl;
			cout << "서버에서 받은 패킷 크기- " << static_cast<int>(buf[0]) << endl;
			return received;
		}

        if (SOCKET_ERROR == received)
            return SOCKET_ERROR;

        else if (0 == received)
            break;
		
        left -= received;
        ptr += received;
    }

    return (len - left);
}

void CObj::send_move_packet(unsigned char dir)
{
	cs_packet_move packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_MOVE;
	packet.direction = dir;

	send_packet(&packet);
}

void CObj::send_packet(void* packet)
{
	char* p = reinterpret_cast<char*>(packet);

	int iSendBytes = send(g_hSocket, p, p[0], 0);

	if (iSendBytes == SOCKET_ERROR)
	{
		wchar_t temp[30] = L"Client Socket Send Failed";
		LPCWSTR tempMsg = temp;
		LPVOID	lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
		MessageBox(NULL, (LPCTSTR)lpMsgBuf, tempMsg, MB_ICONERROR);
		LocalFree(lpMsgBuf);
		closesocket(g_hSocket);
		exit(1);
	}
}

void CObj::ProcessPacket(char* ptr)
{
	switch (ptr[1])
	{
	case SC_PACKET_LOGIN_OK:
	{
		sc_packet_login_ok* my_packet = reinterpret_cast<sc_packet_login_ok*>(ptr);
		g_iServerID = my_packet->id;
		m_tInfo.fX = my_packet->x;
		m_tInfo.fY = my_packet->y;

		cout << "Login OK! 접속완료!" << endl;
	}
	break;

	case SC_PACKET_ENTER:
	{
		sc_packet_enter* my_packet = reinterpret_cast<sc_packet_enter*>(ptr);
		int id = my_packet->id;
		cout << "Enter_pakcet !" << endl;
		if (id == g_iServerID)
		{
			m_tInfo.fX = my_packet->x;
			m_tInfo.fY = my_packet->y;
		}
		else
		{
			if (id < NPC_ID_START)
			{
				g_umap_serverObj[id] = new COthers();
				g_umap_serverObj[id]->Initialize();
			}
			else
			{
				g_umap_serverObj[id] = new COthers();
				g_umap_serverObj[id]->Initialize();
			}
			strcpy_s(g_umap_serverObj[id]->m_szName, my_packet->name);
			g_umap_serverObj[id]->SetPos(my_packet->x, my_packet->y);
		}
	}
	break;

	case SC_PACKET_MOVE:
	{
		sc_packet_move* my_packet = reinterpret_cast<sc_packet_move*>(ptr);
		int other_id = my_packet->id;
		if (other_id == g_iServerID)
		{
			m_tInfo.fX = my_packet->x;
			m_tInfo.fY = my_packet->y;
		}
		else
		{
			if (0 != g_umap_serverObj.count(other_id))
				g_umap_serverObj[other_id]->SetPos(my_packet->x, my_packet->y);
		}
	}
	break;

	case SC_PACKET_LEAVE:
	{
		sc_packet_leave* my_packet = reinterpret_cast<sc_packet_leave*>(ptr);
		int other_id = my_packet->id;
		if (other_id == g_iServerID)
		{
			if (g_umap_serverObj[other_id])
			{
				delete g_umap_serverObj[other_id];
				g_umap_serverObj[other_id] = nullptr;
			}
			g_umap_serverObj.erase(other_id);
		}
		else {
			if (0 != g_umap_serverObj.count(other_id))
			{
				if (g_umap_serverObj[other_id])
				{
					delete g_umap_serverObj[other_id];
					g_umap_serverObj[other_id] = nullptr;
				}
				g_umap_serverObj.erase(other_id);
			}
		}
	}
	break;

	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);
		break;

	}
}

void CObj::process_data(char* net_buf, size_t io_byte)
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
			ProcessPacket(packet_buffer);
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
