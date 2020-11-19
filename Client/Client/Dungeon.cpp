#include "stdafx.h"
#include "Dungeon.h"
#include "Cow.h"
#include "Ninja.h"
#include "PotalTown.h"
#include "PotalDungeon.h"
#include "Player.h"


CDungeon::CDungeon()
{
}


CDungeon::~CDungeon()
{
	Release();
}

void CDungeon::Initialize()
{
	CSoundMgr::GetInstance()->PlayBGM(L"BGM_Dun.mp3");


	/*Ninja Hp Ui*/
	CBmpMgr::GetInstance()->InsertBmp(L"ninjabar", L"../Image/Monster/ninjabar.bmp");

	CTileMgr::GetInstance()->LoadData(L"Dun1");
	CObjMgr::GetInstance()->SetPlayerPos(735.f, 555.f);

	/*Cow 몬스터 배치.*/
	//CObjMgr::GetInstance()->AddObject(CAbstractFactory<CCow>::CreateObj(1230.f, 391.f), ObjID::MONSTER);
	//CObjMgr::GetInstance()->AddObject(CAbstractFactory<CCow>::CreateObj(1227.f, 529.f), ObjID::MONSTER);
	//CObjMgr::GetInstance()->AddObject(CAbstractFactory<CCow>::CreateObj(1222.f, 670.f), ObjID::MONSTER);
	//CObjMgr::GetInstance()->AddObject(CAbstractFactory<CNinja>::CreateObj(1550.f, 870.f), ObjID::MONSTER);

	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CPotalTown>::CreateObj(773.f, 408.f), ObjID::EFFECT);
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CPotalDungeon>::CreateObj(1671.f, 945.f), ObjID::EFFECT);

}

void CDungeon::Update()
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

	

	CTileMgr::GetInstance()->Update();
	CObjMgr::GetInstance()->Update();
	CMouseMgr::GetInstance()->Update();
}

void CDungeon::LateUpdate()
{
	CTileMgr::GetInstance()->LateUpdate();
	CObjMgr::GetInstance()->LateUpdate();
	CMouseMgr::GetInstance()->LateUpdate();
}

void CDungeon::Render(HDC hDC)
{
	CTileMgr::GetInstance()->Render(hDC);

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Dun1"));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();
	/*배경 출력*/
	BitBlt(hDC, (int)fScrollX, (int)fScrollY, DUNGEONCX, DUNGEONCY, hMemDC, 0, 0, SRCCOPY);

	/*Ninja Hp Ui*/
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"ninjabar"));
	GdiTransparentBlt(hDC, 300, 10, 229, 46, hMemDC, 0, 0, 229, 46, RGB(255, 0, 255));

	CObjMgr::GetInstance()->Render(hDC);
	CMouseMgr::GetInstance()->Render(hDC);

}

void CDungeon::Release()
{
	CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNEL::BGM);

	CObjMgr::GetInstance()->DeleteObj(ObjID::MONSTER);
	CObjMgr::GetInstance()->DeleteObj(ObjID::EFFECT);
	CTileMgr::GetInstance()->DestroyInstance();
}

