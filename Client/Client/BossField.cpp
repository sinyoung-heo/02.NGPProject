#include "stdafx.h"
#include "BossField.h"
#include "Boris.h"
#include "PotalDungeon.h"
#include "Player.h"


CBossField::CBossField()
	:m_bIsClear(false)
{
}


CBossField::~CBossField()
{
	Release();
}

void CBossField::Initialize()
{
	// CSoundMgr::GetInstance()->PlayBGM(L"BGM_Boss.mp3");


	CBmpMgr::GetInstance()->InsertBmp(L"BorisBar", L"../Image/Monster/BorisBar.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"StageState", L"../Image/Effect/StageState.bmp");

	CTileMgr::GetInstance()->LoadData(L"Boss");

	CObjMgr::GetInstance()->SetPlayerPos(1125.f, 705.f);

	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CBoris>::CreateObj(1976.f, 1144.f), ObjID::MONSTER);

	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CPotalDungeon>::CreateObj(1012.f, 654.f), ObjID::EFFECT);


	m_clearsound = false;
}

void CBossField::Update()
{
	CTileMgr::GetInstance()->Update();

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


	CObjMgr::GetInstance()->Update();
	CMouseMgr::GetInstance()->Update();
}

void CBossField::LateUpdate()
{
	CObjMgr::GetInstance()->LateUpdate();
	CMouseMgr::GetInstance()->LateUpdate();
	CTileMgr::GetInstance()->LateUpdate();

	if (CObjMgr::GetInstance()->GetMonLst().empty())
	{
		if(!m_clearsound)
			CSoundMgr::GetInstance()->PlaySound(L"Success.wav", CSoundMgr::STAGE_CLEAR);

		m_clearsound = true;

		m_bIsClear = true;
	}

}

void CBossField::Render(HDC hDC)
{
	CTileMgr::GetInstance()->Render(hDC);

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Boss"));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();
	/*배경 출력*/
	BitBlt(hDC, (int)fScrollX, (int)fScrollY, BOSSCX, BOSSCY, hMemDC, 0, 0, SRCCOPY);

	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"BorisBar"));
	GdiTransparentBlt(hDC, 300, 10, 229, 46, hMemDC, 0, 0, 229, 46, RGB(255, 0, 255));

	if (m_bIsClear)
	{
		NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"StageState"));
		GdiTransparentBlt(hDC, 300, 60, 229, 49, hMemDC, 0, 0, 229, 49, RGB(255, 0, 255));
	}


	CObjMgr::GetInstance()->Render(hDC);
	CMouseMgr::GetInstance()->Render(hDC);
}

void CBossField::Release()
{
	// CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNEL::BGM);

	CObjMgr::GetInstance()->DeleteObj(ObjID::MONSTER);
	CObjMgr::GetInstance()->DeleteObj(ObjID::EFFECT);
	CTileMgr::GetInstance()->DestroyInstance();
}
