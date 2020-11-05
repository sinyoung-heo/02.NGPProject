#include "stdafx.h"
#include "Shoes_L.h"


CShoes_L::CShoes_L()
{
}

CShoes_L::~CShoes_L()
{
	Release();
}

void CShoes_L::Initialize()
{
	CBmpMgr::GetInstance()->InsertBmp(L"foot_low", L"../Image/item/foot_low.bmp");
	m_pImageKey = L"foot_low";

	CItem::Initialize();

	m_eItemType = ItemType::SHOES;

	m_tInfo.szName = L"foot_low";
	m_tInfo.iMaxSp = 1000;
	m_tInfo.iGold = 5000;
}

void CShoes_L::LateInit()
{
}

int CShoes_L::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;

	return NO_EVENT;
}

void CShoes_L::LateUpdate()
{

}

void CShoes_L::Render(HDC hDC)
{
	/*CObj::UpdateRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"foot_high"));

	GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, (int)m_tInfo.fX, (int)m_tInfo.fY,
		hMemDC, 0, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));*/
}

void CShoes_L::Release()
{

}
