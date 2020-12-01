#include "stdafx.h"
#include "PacketMgr.h"
#include "COthers.h"
#include "Cow.h"
#include "SkillMoon.h"
#include "SkillSoul.h"
#include "MultiAttack.h"
#include "HpPotionEffect.h"
#include "MpPotionEffect.h"
#include "DmgBox.h"
#include "MonCrash.h"

IMPLEMENT_SINGLETON(CPacketMgr)

int CPacketMgr::RecvPacketFromServer()
{
	char net_buf[PROTOCOL_TEST::MAX_BUF_SIZE];

	// Server Data Receive.
	int retval = recv(g_hSocket, net_buf, MAX_BUF_SIZE, 0);

	if (retval == SOCKET_ERROR)
	{
		int state = WSAGetLastError();
		if (state != WSAEWOULDBLOCK)
		{
			wchar_t temp[30] = L"Client Socket Recieve Failed";
			LPCWSTR tempMsg = temp;
			LPVOID	lpMsgBuf;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
			MessageBox(NULL, (LPCTSTR)lpMsgBuf, tempMsg, MB_ICONERROR);
			LocalFree(lpMsgBuf);
			exit(1);
		}
	}

	// 데이터 수신 성공
	if (retval > 0)
	{
		// 패킷 재조립
		ProcessData(net_buf, static_cast<size_t>(retval));
	}

	return S_OK;
}

void CPacketMgr::ProcessData(char* net_buf, size_t io_byte)
{
	char* ptr = net_buf;

	// 우리가 처리해야 할 원래 패킷 크기
	static size_t in_packet_size = 0;
	// 처리하지 못한 패킷 크기
	static size_t saved_packet_size = 0;
	// recv 받은 패킷을 저장해놓을 버퍼
	static char packet_buffer[PROTOCOL_TEST::MAX_BUF_SIZE];

	while (0 != io_byte)
	{
		if (0 == in_packet_size) in_packet_size = ptr[0];

		if (io_byte + saved_packet_size >= in_packet_size)
		{
			memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
			ProcessPacket(packet_buffer);
			
			// 다음 recv 받을 위치
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

void CPacketMgr::ProcessPacket(char* ptr)
{
	switch (ptr[1])
	{
	case SC_PACKET_LOGIN_OK:
	{
		sc_packet_login_ok* my_packet = reinterpret_cast<sc_packet_login_ok*>(ptr);
		g_iServerID = my_packet->id;
		
		if (!CObjMgr::GetInstance()->GetPlayerLst().empty())
		{
			CObjMgr::GetInstance()->GetPlayer()->SetPos(my_packet->x, my_packet->y);
			CObjMgr::GetInstance()->GetPlayer()->SetPlayerHp(my_packet->hp);
			CObjMgr::GetInstance()->GetPlayer()->SetPlayerMp(my_packet->mp);
			CObjMgr::GetInstance()->GetPlayer()->SetPlayerSp(my_packet->sp);
			CObjMgr::GetInstance()->GetPlayer()->SetPlayerAtt(my_packet->att);
		}

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
			if (!CObjMgr::GetInstance()->GetPlayerLst().empty())
				CObjMgr::GetInstance()->GetPlayer()->SetPos(my_packet->x, my_packet->y);

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

			g_umap_serverObj[id]->SetName(my_packet->name);
			g_umap_serverObj[id]->SetPos(my_packet->x, my_packet->y);
			g_umap_serverObj[id]->SetCurScene(static_cast<CSceneMgr::SCENE>(my_packet->scene_id));

		}
	}
	break;

	case SC_PACKET_MOVE:
	{
		sc_packet_move* my_packet = reinterpret_cast<sc_packet_move*>(ptr);
		int other_id = my_packet->id;
		if (other_id == g_iServerID)
		{
			if (!CObjMgr::GetInstance()->GetPlayerLst().empty())
				CObjMgr::GetInstance()->GetPlayer()->SetPos(my_packet->x, my_packet->y);

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
		else 
		{
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

	case SC_PACKET_SCENECHANGE:
	{
		sc_packet_scenechange* my_packet = reinterpret_cast<sc_packet_scenechange*>(ptr);

		int other_id = my_packet->id;
		if (other_id == g_iServerID)
		{
			if (!CObjMgr::GetInstance()->GetPlayerLst().empty())
			{
				CObjMgr::GetInstance()->GetPlayer()->SetPos(my_packet->x, my_packet->y);
				CObjMgr::GetInstance()->GetPlayer()->SetCurScene(static_cast<CSceneMgr::SCENE>(my_packet->scene_id));
			}

		}
		else
		{
			if (0 != g_umap_serverObj.count(other_id))
			{
				g_umap_serverObj[other_id]->SetPos(my_packet->x, my_packet->y);
				g_umap_serverObj[other_id]->SetCurScene(static_cast<CSceneMgr::SCENE>(my_packet->scene_id));
			}
		}

	}
	break;

	// Other Player Animation Update.
	case SC_PACKET_PLAYERSTANCE:
	{
		sc_packet_playerstance* my_packet = reinterpret_cast<sc_packet_playerstance*>(ptr);
		int other_id = my_packet->id;

		if (other_id != g_iServerID)
		{
			if (0 != g_umap_serverObj.count(other_id))
			{
				static_cast<COthers*>(g_umap_serverObj[other_id])->SetStance(static_cast<COthers::STANCE>(my_packet->cur_stance));
				static_cast<COthers*>(g_umap_serverObj[other_id])->SetCurDir(static_cast<ObjDir::DIR>(my_packet->cur_dir));
			}
		}

	}
	break;

	// Create Monster
	case SC_PACKET_MONSTERCREATE:
	{
		sc_packet_monstercreateinfo* my_packet = reinterpret_cast<sc_packet_monstercreateinfo*>(ptr);
		
		if (MON_COW == my_packet->montype)
		{
			CCow* pCow = new CCow;
			pCow->Initialize();
			pCow->SetPos(my_packet->x, my_packet->y);
			pCow->SetIdx(my_packet->idx);
			pCow->SetStance(static_cast<CCow::STANCE>(my_packet->cur_stance));
			pCow->SetAngle(my_packet->angle);

			CObjMgr::GetInstance()->AddObject(pCow, ObjID::MONSTER_COW);
		}
		else if (MON_NINJA == my_packet->montype)
		{

		}
		else if (MON_BORIS == my_packet->montype)
		{

		}

	}
	break;

	// Recv MonsterInfo
	case SC_PACKET_MONSTERINFO:
	{
		sc_packet_monsterinfo* my_packet = reinterpret_cast<sc_packet_monsterinfo*>(ptr);
		
		if (MON_COW == my_packet->montype)
		{
			OBJLIST monLst = CObjMgr::GetInstance()->GetObjList(ObjID::MONSTER_COW);

			// 받은 패킷의 index와 같은 번호인 monster의 정보를 set.
			for (auto& pCow : monLst)
			{
				if (my_packet->idx == static_cast<CCow*>(pCow)->GetIdx())
				{
					static_cast<CCow*>(pCow)->SetPos(my_packet->x, my_packet->y);
					static_cast<CCow*>(pCow)->SetHpInfo(my_packet->hp);
					static_cast<CCow*>(pCow)->SetAngle(my_packet->angle);
					static_cast<CCow*>(pCow)->SetStance(static_cast<CCow::STANCE>(my_packet->cur_stance));
					static_cast<CCow*>(pCow)->SetCurFrame(static_cast<int>(my_packet->cur_frame));
					pCow->SetIsDead(my_packet->is_dead);
				}
			}
		}
	}
	break;

	case SC_PACKET_PLAYERATTACK:
	{
		sc_packet_playerattack* my_packet = reinterpret_cast<sc_packet_playerattack*>(ptr);
		
		int other_id = my_packet->id;
		
#ifdef SHOW_LOG
		cout << "Player Attack Packet, ID: " << other_id <<
			" / Skill Num: " << static_cast<int>(my_packet->cur_skill) << endl;
#endif // SHOW_LOG
		
		if (0 != g_umap_serverObj.count(other_id))
		{
			CObj* others = static_cast<COthers*>(g_umap_serverObj[other_id]);
			switch (my_packet->cur_skill)
			{
			case SKILL_MOON:
				CObjMgr::GetInstance()->AddObject(static_cast<COthers*>(g_umap_serverObj[other_id])
					->CreateSkill<CSkillMoon>(my_packet->skillPos_x, my_packet->skillPos_y), ObjID::SKILL);
				break;

			case SKILL_SOUL:
				CObjMgr::GetInstance()->AddObject(static_cast<COthers*>(g_umap_serverObj[other_id])
					->CreateSkill<CSkillSoul>(my_packet->skillPos_x, my_packet->skillPos_y), ObjID::SKILL);
				break;
			case SKILL_MULTI:
				CObjMgr::GetInstance()->AddObject(static_cast<COthers*>(g_umap_serverObj[other_id])
					->CreateSkill<CMultiAttack>(my_packet->skillPos_x, my_packet->skillPos_y), ObjID::SKILL);
				break;
			case SKILL_HP:
				CObjMgr::GetInstance()->AddObject(CAbstractFactory<CHpPotionEffect>::CreateObj(my_packet->skillPos_x, my_packet->skillPos_y - 1.f),
					ObjID::EFFECT);
				break;
			case SKILL_SP:
				CObjMgr::GetInstance()->AddObject(CAbstractFactory<CMpPotionEffect>::CreateObj(my_packet->skillPos_x, my_packet->skillPos_y - 1.f),
					ObjID::EFFECT);
				break;
			case SKILL_MP:
				CObjMgr::GetInstance()->AddObject(CAbstractFactory<CMpPotionEffect>::CreateObj(my_packet->skillPos_x, my_packet->skillPos_y - 1.f),
					ObjID::EFFECT);
				break;
			default:
				break;
			}
		}
		
		
	}
		break;

	case SC_PACKET_DMGBOXCREATE:
	{
		sc_packet_dmgboxcreate* my_packet = reinterpret_cast<sc_packet_dmgboxcreate*>(ptr);

		CObjMgr::GetInstance()->AddObject(CAbstractFactory<CDmgBox>::CreateDmgBox(my_packet->x - 20, my_packet->y - 80.f, my_packet->dmg),
			ObjID::EFFECT);

	}
		break;

	case SC_PACKET_MONCRASHCREATE:
	{
		sc_packet_moncrashcreate* my_packet = reinterpret_cast<sc_packet_moncrashcreate*>(ptr);

		if (!CObjMgr::GetInstance()->GetPlayerLst().empty())
			CObjMgr::GetInstance()->GetPlayer()->SetHpInfo(my_packet->hp);

		CObjMgr::GetInstance()->AddObject(CAbstractFactory<CMonCrash>::CreateObj(my_packet->x, my_packet->y),
			ObjID::EFFECT);
	}
		break;

	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);
		break;

	}
}

void CPacketMgr::SendPlayerMovePacket(unsigned char dir)
{
	cs_packet_move packet;
	packet.size			= sizeof(packet);
	packet.type			= CS_PACKET_MOVE;
	packet.direction	= dir;

	SendPacketToServer(&packet);
}

void CPacketMgr::SendPlayerSceneID(unsigned char scene_id)
{
	cs_packet_scenechange packet;
	packet.size		= sizeof(packet);
	packet.type		= CS_PACKET_SCENECHANGE;
	packet.scene_id = scene_id;

	SendPacketToServer(&packet);
}

void CPacketMgr::SendPlayerStance(int stance, int dir)
{
	cs_packet_playerstance packet;
	packet.size			= sizeof(packet);
	packet.type			= CS_PACKET_PLAYERSTANCE;
	packet.cur_stance	= stance;
	packet.cur_dir		= dir;

	SendPacketToServer(&packet);
}

void CPacketMgr::SendPlayerAttack(float attX, float attY, unsigned char skill)
{
	cs_packet_playerattack packet;
	packet.size			= sizeof(packet);
	packet.type			= CS_PACKET_PLAYERATTACK;
	packet.cur_skill	= skill;
	packet.skillPos_x	= attX;
	packet.skillPos_y	= attY;

	SendPacketToServer(&packet);
}

void CPacketMgr::SendPacketToServer(void* packet)
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
