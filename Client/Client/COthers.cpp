#include "stdafx.h"
#include "COthers.h"

COthers::COthers()
{
}

COthers::~COthers()
{
	Release();
}

void COthers::Initialize()
{
	/*Player Info*/
	m_tInfo.fCX = 256.f;
	m_tInfo.fCY = 256.f;
	m_tInfo.szName = L"Sin_Young";
	m_tInfo.iLevel = 1;
	m_tInfo.iMaxHp = 7000;
	m_tInfo.iHp = m_tInfo.iMaxHp;
	m_tInfo.iMaxMp = 3000;
	m_tInfo.iMp = m_tInfo.iMaxMp;
	m_tInfo.iMaxSp = 5000;
	m_tInfo.iSp = m_tInfo.iMaxSp;
	m_tInfo.iAtt = 123;
	m_tInfo.iExp = 0;
	m_tInfo.iMaxExp = 200;
	m_tInfo.iGold = 1000000;

	/*애니메이션 변수 IDLE 상태로 초기화.*/
	m_pFrameKey = L"Player_Down_";
	m_eCurStance = IDLE;
	m_ePreStance = m_eCurStance;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iScene = 5;
	m_tFrame.dwOldTime = GetTickCount();
	m_tFrame.dwFrameSpd = 200;


	/* 서버에서 사용하게 될 NAME */
	strcpy_s(m_szName, g_szServerName);


	/*충돌박스 좌표 = Player의 좌표*/
	CObj::UpdateColXYPos();
	/*충돌박스 크기*/
	m_tColInfo.fCX = 30.f;
	m_tColInfo.fCY = 30.f;

	/*이동속도*/
	m_fSpeed = 2.5f;

	/*스킬&공격 방향*/
	m_fAngle = 270.f;

	m_eCurDir = ObjDir::DOWN;
	m_eCurScene = CSceneMgr::SC_TOWN;
}

void COthers::LateInit()
{
}

int COthers::Update()
{
	CObj::LateInit();

	if (m_bIsDead)
		return DEAD_OBJ;

	return 0;
}

void COthers::LateUpdate()
{
	CObj::UpdateColXYPos();

	// 애니메이션 함수.

	for (auto& ob : g_umap_serverObj)
	{
		ob.second->MoveFrame();
		static_cast<COthers*>(ob.second)->SceneChange();
		static_cast<COthers*>(ob.second)->UpdateAnimationKey();
	}
	//SceneChange();
	//UpdateAnimationKey();
}

void COthers::Render(HDC hDC)
{
	CObj::UpdateRect();		//이미지박스 업데이트.
	CObj::UpdateColRect();	//충돌박스  업데이트.

	/*Player Render*/
	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	HDC hMemDC = nullptr;
	for (auto& ob : g_umap_serverObj)
	{
		// 서로 다른 씬에 있다면 Rendering X
		if (!CObjMgr::GetInstance()->GetPlayerLst().empty())
		{
			CSceneMgr::SCENE eSceneID = CObjMgr::GetInstance()->GetPlayer()->GetSceneID();

			if (eSceneID != ob.second->GetSceneID())
				continue;
		}


		NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(static_cast<COthers*>(ob.second)->GetFrameKey().c_str()));
		
		m_tRect.left = LONG(ob.second->GetInfo().fX - ob.second->GetInfo().fCX * 0.5f);
		m_tRect.top = LONG(ob.second->GetInfo().fY - ob.second->GetInfo().fCY * 0.5f);
		m_tRect.right = LONG(ob.second->GetInfo().fX + ob.second->GetInfo().fCX * 0.5f);
		m_tRect.bottom = LONG(ob.second->GetInfo().fY + ob.second->GetInfo().fCY * 0.5f);

		/*스크롤 적용하여 렌더*/
		GdiTransparentBlt(hDC,
						  m_tRect.left + (int)fScrollX,
						  m_tRect.top + (int)fScrollY,
						  (int)m_tInfo.fCX,
						  (int)m_tInfo.fCY,
						  hMemDC,
						  ob.second->GetFrame().iFrameStart * (int)m_tInfo.fCX,
						  ob.second->GetFrame().iScene * (int)m_tInfo.fCY,
						  (int)m_tInfo.fCX,
						  (int)m_tInfo.fCY,
						  RGB(255, 0, 255));
	}
}

void COthers::Release()
{
}

void COthers::SceneChange()
{
	if (m_eCurStance != m_ePreStance)
	{
		switch (m_eCurStance)
		{
		case IDLE:
			m_tFrame.iFrameStart	= 0;
			m_tFrame.iFrameEnd		= 4;
			m_tFrame.iScene			= 5;
			m_tFrame.dwOldTime		= GetTickCount();
			m_tFrame.dwFrameSpd		= 100;
			break;
		case WALK:
			m_tFrame.iFrameStart	= 0;
			m_tFrame.iFrameEnd		= 8;
			m_tFrame.iScene			= 6;
			m_tFrame.dwOldTime		= GetTickCount();
			m_tFrame.dwFrameSpd		= 100;
			break;
		case RUN:
			m_tFrame.iFrameStart	= 0;
			m_tFrame.iFrameEnd		= 9;
			m_tFrame.iScene			= 3;
			m_tFrame.dwOldTime		= GetTickCount();
			m_tFrame.dwFrameSpd		= 100;
			break;
		case ATTACK:
			m_tFrame.iFrameStart	= 0;
			m_tFrame.iFrameEnd		= 5;
			m_tFrame.iScene			= 0;
			m_tFrame.dwOldTime		= GetTickCount();
			m_tFrame.dwFrameSpd		= 100;
			break;
		case SKILL:
			m_tFrame.iFrameStart	= 0;
			m_tFrame.iFrameEnd		= 8;
			m_tFrame.iScene			= 2;
			m_tFrame.dwOldTime		= GetTickCount();
			m_tFrame.dwFrameSpd		= 100;
			break;
		case HIT:
			m_tFrame.iFrameStart	= 1;
			m_tFrame.iFrameEnd		= 1;
			m_tFrame.iScene			= 1;
			m_tFrame.dwOldTime		= GetTickCount();
			m_tFrame.dwFrameSpd		= 200;
			break;
		}

		m_ePreStance = m_eCurStance;
	}
}

void COthers::UpdateAnimationKey()
{
	switch (m_eCurDir)
	{
	case ObjDir::DOWN:
		m_pFrameKey = L"Player_Down_";
		break;
	case ObjDir::LEFT:
		m_pFrameKey = L"Player_Left_";
		break;
	case ObjDir::RIGHT:
		m_pFrameKey = L"Player_Right_";
		break;
	case ObjDir::UP:
		m_pFrameKey = L"Player_Up_";
		break;
	default:
		break;
	}
}
