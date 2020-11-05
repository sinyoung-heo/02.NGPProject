#include "stdafx.h"
#include "Tile.h"


CTile::CTile()
{
}

CTile::~CTile()
{
	Release();
}

void CTile::Initialize()
{
	m_tInfo.fCX = (float)TILECX;
	m_tInfo.fCY = (float)TILECY;

	m_tColInfo.fCX = m_tInfo.fCX;
	m_tColInfo.fCY = m_tInfo.fCY;

	m_iDrawID = 0;
}

void CTile::LateInit()
{

}

int CTile::Update()
{
	CObj::LateInit();


	return NO_EVENT;
}

void CTile::LateUpdate()
{

}

void CTile::Render(HDC hDC)
{
	CObj::UpdateRect();

	TCHAR pImagekey[8];
	swprintf_s(pImagekey, L"tile_%d", m_iDrawID);

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(pImagekey));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	GdiTransparentBlt(hDC, m_tRect.left + (int)fScrollX, m_tRect.top + (int)fScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMemDC, 0, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));

	/*BitBlt(hDC, m_tRect.left + fScrollX, m_tRect.top + fScrollY, TILECX, TILECY,
		hMemDC, 0, 0, SRCCOPY);*/
}

void CTile::Release()
{
}
