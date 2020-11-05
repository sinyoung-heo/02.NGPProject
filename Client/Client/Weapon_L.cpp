#include "stdafx.h"
#include "Weapon_L.h"


CWeapon_L::CWeapon_L()
{
}

CWeapon_L::~CWeapon_L()
{
	Release();

}

void CWeapon_L::Initialize()
{
	CBmpMgr::GetInstance()->InsertBmp(L"arms_low", L"../Image/item/arms_low.bmp");
	m_pImageKey = L"arms_low";

	CItem::Initialize();

	m_eItemType = ItemType::WEAPON;

	m_tInfo.szName = L"arms_low";
	m_tInfo.iAtt = 250;
	m_tInfo.iGold = 5000;
}

void CWeapon_L::LateInit()
{
}

int CWeapon_L::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;

	return NO_EVENT;
}

void CWeapon_L::LateUpdate()
{
}

void CWeapon_L::Render(HDC hDC)
{
	/*CObj::UpdateRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"arms_low"));

	GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, (int)m_tInfo.fX, (int)m_tInfo.fY,
		hMemDC, 0, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));*/
}

void CWeapon_L::Release()
{
}
