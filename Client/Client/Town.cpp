#include "stdafx.h"
#include "Town.h"
#include "Player.h"
#include "PotalDungeon.h"


CTown::CTown()
	:m_bIsCreate(false), m_TileOn(false)
{
}


CTown::~CTown()
{
	Release();
}

void CTown::Initialize()
{	
	CSoundMgr::GetInstance()->PlayBGM(L"Vanilla Mood-06-Tales Are About To Be Weaved~And, Don`t Forget~Carnival Town~Laputa.mp3");

	/*Player 최초 한 번만 생성하기 위한 작업.*/
	if (!CObjMgr::GetInstance()->GetPlayerLst().empty())
	{
		CObj* pTemp = CObjMgr::GetInstance()->GetPlayer();
		m_bIsCreate = dynamic_cast<CPlayer*>(pTemp)->GetIsCreate();
	}
	if (!m_bIsCreate)
	{
		CObjMgr::GetInstance()->AddObject(CAbstractFactory<CPlayer>::CreateObj(),
			ObjID::PLAYER);
	}
	CObjMgr::GetInstance()->SetPlayerPos(1005.f, 555.f);
	CTileMgr::GetInstance()->LoadData(L"Town");

	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CPotalDungeon>::CreateObj(705.f, 860.f),
		ObjID::EFFECT);
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CPotalDungeon>::CreateObj(1410.f, 284.f),
		ObjID::EFFECT);


}

void CTown::Update()
{
	if (KEY_DOWN('1'))
		CSceneMgr::GetInstance()->SceneChange(CSceneMgr::SC_TOWN);
	if (KEY_DOWN('2'))
		CSceneMgr::GetInstance()->SceneChange(CSceneMgr::SC_STORE);
	if (KEY_DOWN('3'))
		CSceneMgr::GetInstance()->SceneChange(CSceneMgr::SC_FIELD);
	if (KEY_DOWN('4'))
		CSceneMgr::GetInstance()->SceneChange(CSceneMgr::SC_DUNGEON);
	if (KEY_DOWN('5'))
		CSceneMgr::GetInstance()->SceneChange(CSceneMgr::SC_BOSS);

	if (KEY_DOWN('6'))
		m_TileOn = !m_TileOn;


	CObjMgr::GetInstance()->Update();
	CMouseMgr::GetInstance()->Update();
	CTileMgr::GetInstance()->Update();
}

void CTown::LateUpdate()
{
	CObjMgr::GetInstance()->LateUpdate();
	CMouseMgr::GetInstance()->LateUpdate();

	CTileMgr::GetInstance()->LateUpdate();
}

void CTown::Render(HDC hDC)
{
	CTileMgr::GetInstance()->Render(hDC);

	/*배경 출력*/
	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Town"));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	BitBlt(hDC, (int)fScrollX, (int)fScrollY, TOWNCX , TOWNCY, hMemDC, 0, 0, SRCCOPY);

	if(m_TileOn)
		CTileMgr::GetInstance()->Render(hDC);


	CObjMgr::GetInstance()->Render(hDC);
	CMouseMgr::GetInstance()->Render(hDC);
}

void CTown::Release()
{
	CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNEL::BGM);

	CTileMgr::GetInstance()->DestroyInstance();
	CObjMgr::GetInstance()->DeleteObj(ObjID::EFFECT);
}
