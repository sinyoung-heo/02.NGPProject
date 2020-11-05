#include "stdafx.h"
#include "Weapon_H.h"


CWeapon_H::CWeapon_H()
{
}

CWeapon_H::~CWeapon_H()
{
	Release();
}

void CWeapon_H::Initialize()
{
	CBmpMgr::GetInstance()->InsertBmp(L"arms_high", L"../Image/item/arms_high.bmp");
	m_pImageKey = L"arms_high";

	CItem::Initialize();

	m_eItemType = ItemType::WEAPON;

	m_tInfo.szName = L"arms_high";
	m_tInfo.iAtt = 500;
	m_tInfo.iGold = 15000;
}

void CWeapon_H::LateInit()
{
}

int CWeapon_H::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;

	return NO_EVENT;
}

void CWeapon_H::LateUpdate()
{
}

void CWeapon_H::Render(HDC hDC)
{
	/*CObj::UpdateRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"arms_high"));

	GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, (int)m_tInfo.fX, (int)m_tInfo.fY,
		hMemDC, 0, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));*/
}

void CWeapon_H::Release()
{
}
