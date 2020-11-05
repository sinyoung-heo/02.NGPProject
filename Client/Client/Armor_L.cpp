#include "stdafx.h"
#include "Armor_L.h"


CArmor_L::CArmor_L()
{
}

CArmor_L::~CArmor_L()
{
}

void CArmor_L::Initialize()
{
	CBmpMgr::GetInstance()->InsertBmp(L"armor_low", L"../Image/item/armor_low.bmp");
	m_pImageKey = L"armor_low";

	CItem::Initialize();

	m_eItemType = ItemType::ARMOR;

	m_tInfo.szName = L"armor_low";
	m_tInfo.iMaxHp = 1000;
	m_tInfo.iGold = 15000;
}

void CArmor_L::LateInit()
{
}

int CArmor_L::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;

	return NO_EVENT;
}

void CArmor_L::LateUpdate()
{
}

void CArmor_L::Render(HDC hDC)
{
	/*CObj::UpdateRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"armor_low"));

	GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, (int)m_tInfo.fX, (int)m_tInfo.fY,
		hMemDC, 0, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));*/
}

void CArmor_L::Release()
{
}
