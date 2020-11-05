#include "stdafx.h"
#include "Shoes_H.h"


CShoes_H::CShoes_H()
{
}

CShoes_H::~CShoes_H()
{
	Release();
}

void CShoes_H::Initialize()
{
	CBmpMgr::GetInstance()->InsertBmp(L"foot_high", L"../Image/item/foot_high.bmp");
	m_pImageKey = L"foot_high";

	CItem::Initialize();

	m_eItemType = ItemType::SHOES;

	m_tInfo.szName = L"foot_high";
	m_tInfo.iMaxSp = 3000;
	m_tInfo.iGold = 15000;
}

void CShoes_H::LateInit()
{
}

int CShoes_H::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;

	return NO_EVENT;
}

void CShoes_H::LateUpdate()
{

}

void CShoes_H::Render(HDC hDC)
{
	/*CObj::UpdateRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"foot_high"));

	GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, (int)m_tInfo.fX, (int)m_tInfo.fY,
		hMemDC, 0, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));*/
}

void CShoes_H::Release()
{

}
