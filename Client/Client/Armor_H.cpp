#include "stdafx.h"
#include "Armor_H.h"


CArmor_H::CArmor_H()
{
}

CArmor_H::~CArmor_H()
{
	Release();
}

void CArmor_H::Initialize()
{
	CBmpMgr::GetInstance()->InsertBmp(L"armor_high", L"../Image/item/armor_high.bmp");
	m_pImageKey = L"armor_high";

	CItem::Initialize();

	m_eItemType = ItemType::ARMOR;

	m_tInfo.szName = L"armor_high";
	m_tInfo.iMaxHp = 3000;
	m_tInfo.iGold = 15000;
}

void CArmor_H::LateInit()
{

}

int CArmor_H::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;

	return NO_EVENT;
}

void CArmor_H::LateUpdate()
{

}

void CArmor_H::Render(HDC hDC)
{
	//CObj::UpdateRect();

	//HDC hMemDC = nullptr;
	//NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"armor_high"));

	//GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, (int)m_tInfo.fX, (int)m_tInfo.fY,
	//	hMemDC, 0, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));
}

void CArmor_H::Release()
{

}
