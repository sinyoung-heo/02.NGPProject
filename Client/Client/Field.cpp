#include "stdafx.h"
#include "Field.h"
#include "Jelly.h"
#include "PotalTown.h"
#include "Player.h"


CField::CField()
	:m_TileOn(false)
{
}


CField::~CField()
{
	Release();
}

void CField::Initialize()
{
	// CSoundMgr::GetInstance()->PlayBGM(L"BGM_Field.wav");


	CTileMgr::GetInstance()->LoadData(L"Field");

	CObjMgr::GetInstance()->SetPlayerPos(1515.f, 1035.f);


	//CObjMgr::GetInstance()->AddObject(CAbstractFactory<CJelly>::CreateObj(1545.f, 1155.f),
	//	ObjID::MONSTER);
	//CObjMgr::GetInstance()->AddObject(CAbstractFactory<CJelly>::CreateObj(1695.f, 1155.f),
	//	ObjID::MONSTER);
	//CObjMgr::GetInstance()->AddObject(CAbstractFactory<CJelly>::CreateObj(1755.f, 1155.f),
	//	ObjID::MONSTER);

	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CPotalTown>::CreateObj(1615.f, 910.f),
		ObjID::EFFECT);


}

void CField::Update()
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


	CTileMgr::GetInstance()->Update();
	CObjMgr::GetInstance()->Update();
	CMouseMgr::GetInstance()->Update();
	
}

void CField::LateUpdate()
{
	CTileMgr::GetInstance()->LateUpdate();
	CObjMgr::GetInstance()->LateUpdate();
	CMouseMgr::GetInstance()->LateUpdate();
	
}

void CField::Render(HDC hDC)
{
	CTileMgr::GetInstance()->Render(hDC);

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Field"));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();
	/*배경 출력*/
	BitBlt(hDC, (int)fScrollX, (int)fScrollY, FIELDCX, FIELDCY, hMemDC, 0, 0, SRCCOPY);


	if (m_TileOn)
		CTileMgr::GetInstance()->Render(hDC);

	CObjMgr::GetInstance()->Render(hDC);
	CMouseMgr::GetInstance()->Render(hDC);

}

void CField::Release()
{
	// CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNEL::BGM);

	CObjMgr::GetInstance()->DeleteObj(ObjID::MONSTER);
	CObjMgr::GetInstance()->DeleteObj(ObjID::EFFECT);
	CTileMgr::GetInstance()->DestroyInstance();
}
