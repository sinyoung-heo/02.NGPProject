#include "stdafx.h"
#include "Store.h"
#include "Player.h"
#include "Seller.h"
#include "SellLst.h"

CStore::CStore()
	:m_TileOn(false)
{
}

CStore::~CStore()
{
	Release();
}

void CStore::Initialize()
{
	CSoundMgr::GetInstance()->PlayBGM(L"BGM_Store.wav");

	CTileMgr::GetInstance()->LoadData(L"Store");

	CObjMgr::GetInstance()->SetPlayerPos(1005.f, 765.f);



	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CSellLst>::CreateObj(200, 330),
		ObjID::SELL_LST);
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CSeller>::CreateObj(784.f, 463.f),
		ObjID::NPC);
}

void CStore::Update()
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

	/*POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();
	*/
	

	CObjMgr::GetInstance()->Update();
	CMouseMgr::GetInstance()->Update();
	CTileMgr::GetInstance()->Update();
}

void CStore::LateUpdate()
{

	CObjMgr::GetInstance()->LateUpdate();
	CMouseMgr::GetInstance()->LateUpdate();
	CTileMgr::GetInstance()->LateUpdate();

}

void CStore::Render(HDC hDC)
{
	CTileMgr::GetInstance()->Render(hDC);

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Store"));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();
	/*배경 출력*/
	BitBlt(hDC, (int)fScrollX, (int)fScrollY, STORECX, STORECY, hMemDC, 0, 0, SRCCOPY);


	if (m_TileOn)
		CTileMgr::GetInstance()->Render(hDC);


	CObjMgr::GetInstance()->Render(hDC);
	CMouseMgr::GetInstance()->Render(hDC);
}

void CStore::Release()
{
	CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNEL::BGM);

	//CObjMgr::GetInstance()->DestroyInstance();

	CTileMgr::GetInstance()->DestroyInstance();


	CObjMgr::GetInstance()->DeleteObj(ObjID::NPC);
	CObjMgr::GetInstance()->DeleteObj(ObjID::SELL_LST);
}
