#include "stdafx.h"
#include "Item.h"
#include "SellLst.h"
#include "Helmet_H.h"
#include "Helmet_L.h"
#include "Armor_H.h"
#include "Armor_L.h"
#include "Weapon_H.h"
#include "Weapon_L.h"
#include "Shoes_H.h"
#include "Shoes_L.h"
#include "Inventory.h"

CSellLst::CSellLst()
	:m_bIsSellLst(false), m_pPlayer(nullptr), m_pInventory(nullptr)
{
	ZeroMemory(&m_tBlank, sizeof(RECT) * ITEM_SIZE);
	ZeroMemory(&m_iIsEmpty, sizeof(int) * ITEM_SIZE);
}


CSellLst::~CSellLst()
{
	Release();
}

void CSellLst::Initialize()
{
	CBmpMgr::GetInstance()->InsertBmp(L"trade", L"../Image/Ui/trade.bmp");

	m_tInfo.fCX = 343.f;
	m_tInfo.fCY = 356.f;

	for (int i = 0; i < ITEM_SIZE; ++i)
	{
		m_tBlankInfo[i].fCX = 24.f;
		m_tBlankInfo[i].fCY = 24.f;
	}
	/*아이템 위치 정보*/
	m_tBlankInfo[0].fX = 50.f, m_tBlankInfo[0].fY = 205.f;
	m_tBlankInfo[1].fX = 50.f, m_tBlankInfo[1].fY = 230.f;
	m_tBlankInfo[2].fX = 50.f, m_tBlankInfo[2].fY = 255.f;
	m_tBlankInfo[3].fX = 50.f, m_tBlankInfo[3].fY = 280.f;
	m_tBlankInfo[4].fX = 50.f, m_tBlankInfo[4].fY = 305.f;
	m_tBlankInfo[5].fX = 50.f, m_tBlankInfo[5].fY = 330.f;
	m_tBlankInfo[6].fX = 50.f, m_tBlankInfo[6].fY = 355.f;
	m_tBlankInfo[7].fX = 50.f, m_tBlankInfo[7].fY = 380.f;


	m_pPlayer = CObjMgr::GetInstance()->GetPlayer();
	m_pInventory = CObjMgr::GetInstance()->GetInventory();
}

void CSellLst::LateInit()
{
	m_vecItem.push_back(CAbstractFactory<CArmor_H>::CreateObj(m_tBlankInfo[0].fX, m_tBlankInfo[0].fY));
	m_iIsEmpty[0] = 1;
	m_vecItem.push_back(CAbstractFactory<CArmor_L>::CreateObj(m_tBlankInfo[1].fX, m_tBlankInfo[1].fY));
	m_iIsEmpty[1] = 1;

	m_vecItem.push_back(CAbstractFactory<CWeapon_H>::CreateObj(m_tBlankInfo[2].fX, m_tBlankInfo[2].fY));
	m_iIsEmpty[2] = 1;
	m_vecItem.push_back(CAbstractFactory<CWeapon_L>::CreateObj(m_tBlankInfo[3].fX, m_tBlankInfo[3].fY));
	m_iIsEmpty[3] = 1;

	m_vecItem.push_back(CAbstractFactory<CShoes_H>::CreateObj(m_tBlankInfo[4].fX, m_tBlankInfo[4].fY));
	m_iIsEmpty[4] = 1;
	m_vecItem.push_back(CAbstractFactory<CShoes_L>::CreateObj(m_tBlankInfo[5].fX, m_tBlankInfo[5].fY));
	m_iIsEmpty[5] = 1;

	m_vecItem.push_back(CAbstractFactory<CHelmet_H>::CreateObj(m_tBlankInfo[6].fX, m_tBlankInfo[6].fY));
	m_iIsEmpty[6] = 1;
	m_vecItem.push_back(CAbstractFactory<CHelmet_L>::CreateObj(m_tBlankInfo[7].fX, m_tBlankInfo[7].fY));
	m_iIsEmpty[7] = 1;
}

int CSellLst::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;

	/*상점 꺼진 상태면 인벤토리 off*/
	static_cast<CInventory*>(m_pInventory)->SetbIsInvenOn(false);
	static_cast<CInventory*>(m_pInventory)->SetbIsSellLstOn(false);

	if (m_bIsSellLst)
	{
		/*상점 켜진 상태면 인벤토리 on*/
		static_cast<CInventory*>(m_pInventory)->SetbIsInvenOn(true);
		static_cast<CInventory*>(m_pInventory)->SetbIsSellLstOn(true);

		POINT pt = {};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		//float fScrollX = CScrollMgr::GetScrollX();
		//float fScrollY = CScrollMgr::GetScrollY();

		

		for (int i = 0; i < ITEM_SIZE; ++i)
		{
			if (PtInRect(&m_tBlank[i], pt))
			{
				if (KEY_DOWN(VK_LBUTTON))
				{	
					CSoundMgr::GetInstance()->PlaySound(L"Buy.wav", CSoundMgr::ITEM_BUY);

					/*클릭한 인덱스 번호 저장.*/
					m_iSelect = i;

					/*선택한 item과 똑같은 정보의 객체 생성.*/
					CObj* pNewItem = new CItem(m_vecItem[m_iSelect]->GetInfo());
					static_cast<CItem*>(pNewItem)->SetImageKey(static_cast<CItem*>(m_vecItem[m_iSelect])->GetImageKey());
					static_cast<CItem*>(pNewItem)->SetItemType(static_cast<CItem*>(m_vecItem[m_iSelect])->GetItemType());

					/*pInventory -> BuyItem() 호출 --> Inventory에 push back.*/
					static_cast<CInventory*>(m_pInventory)->BuyItem(pNewItem);
				}
			}
		}
	}

	return NO_EVENT;
}

void CSellLst::LateUpdate()
{
	if (m_bIsSellLst)
	{




	}
}

void CSellLst::Render(HDC hDC)
{

	if (m_bIsSellLst)
	{
		CObj::UpdateRect();
		UpdateBlankRect();

		Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

		for (int i = 0; i < ITEM_SIZE; ++i)
			Rectangle(hDC, m_tBlank[i].left, m_tBlank[i].top, m_tBlank[i].right, m_tBlank[i].bottom);

		HDC hMemDC = nullptr;
		NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"trade"));

		GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			hMemDC, 0, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));



		/*아이템 출력*/
		int i = 0;
		for (CObj*& pObj : m_vecItem)
		{
			NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(static_cast<CItem*>(pObj)->GetImageKey().c_str()));

			GdiTransparentBlt(hDC, m_tBlank[i].left, m_tBlank[i].top, 24, 24, hMemDC, 0, 0, 24, 24, RGB(255, 0, 255));
			
			++i;
			if (i > ITEM_SIZE)
				break;	//아이템 size 넘어가면탈출.
		}


		/*아이템 text box*/
		SetBkMode(hDC, TRANSPARENT);
		TCHAR szBuf[32] = L"";

		int j = 0;
		for (CObj*& pObj : m_vecItem)
		{
			//swprintf_s(szBuf, L"%s", pObj->GetInfo().szName.c_str());
			TextOut(hDC, 80, m_tBlank[j].top + 7, pObj->GetInfo().szName.c_str(), pObj->GetInfo().szName.size());

			swprintf_s(szBuf, L"%d", pObj->GetInfo().iGold);
			TextOut(hDC, 300, m_tBlank[j].top + 7, szBuf, lstrlen(szBuf));

			++j;
			if (j > ITEM_SIZE)
				break;	//아이템 size 넘어가면탈출.
		}
	}
}

void CSellLst::Release()
{
	for_each(m_vecItem.begin(), m_vecItem.end(), SafeDelete<CObj*>);	//아이템 삭제.
	m_vecItem.clear();
}

void CSellLst::UpdateBlankRect()
{
	for (int i = 0; i < ITEM_SIZE; ++i)
	{
		m_tBlank[i].left	= (LONG)(m_tBlankInfo[i].fX - m_tBlankInfo[i].fCX * 0.5f);
		m_tBlank[i].top		= (LONG)(m_tBlankInfo[i].fY - m_tBlankInfo[i].fCY * 0.5f);
		m_tBlank[i].right	= (LONG)(m_tBlankInfo[i].fX + m_tBlankInfo[i].fCX * 0.5f);
		m_tBlank[i].bottom	= (LONG)(m_tBlankInfo[i].fY + m_tBlankInfo[i].fCY * 0.5f);
	}
}
