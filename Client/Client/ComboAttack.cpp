#include "stdafx.h"
#include "ComboAttack.h"


CComboAttack::CComboAttack()
{

}

CComboAttack::~CComboAttack()
{
	Release();
}

void CComboAttack::Initialize()
{
	CBmpMgr::GetInstance()->InsertBmp(L"combo", L"../Image/Effect/combo.bmp");

	m_eLayer = Layer::UI;

	m_tInfo.fCX = 100.f;
	m_tInfo.fCY = 46.f;

	m_tInfoRectCnt.fCX = 23.f;
	m_tInfoRectCnt.fCY = 30.f;


	ZeroMemory(&m_iCntArr, sizeof(int) * 2);
	m_iComboCnt = 0;
	m_bIsCombo = false;

	ZeroMemory(&m_tInfoRectCnt, sizeof(TEXTINFO));
	ZeroMemory(&m_tRectCnt, sizeof(RECT));

	m_iArrSize = 0;
	m_iInput = 0;
}

void CComboAttack::LateInit()
{

}

int CComboAttack::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return NO_EVENT;

	m_tInfoRectCnt.fX = m_tInfo.fX + 10.f;
	m_tInfoRectCnt.fY = m_tInfo.fY - 10.f;

	if (m_bIsCombo)
	{
		int i;

		while (true)
		{
			i = 0;

			m_iCntArr[i] = m_iComboCnt % 10;

			++m_iArrSize;
			++i;

			m_iComboCnt = m_iComboCnt / 10;

			if (m_iComboCnt / 10 == 0)
			{
				m_iCntArr[i] = m_iComboCnt % 10;
				++m_iArrSize;

				break;
			}
		}

	}


	return NO_EVENT;
}

void CComboAttack::LateUpdate()
{

}

void CComboAttack::Render(HDC hDC)
{
	if (m_bIsCombo)
	{
		CObj::UpdateRect();
		UpdateRectCnt();

		HDC hMemDC = nullptr;
		NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"combo"));

		float fScrollX = CScrollMgr::GetScrollX();
		float fScrollY = CScrollMgr::GetScrollY();

		GdiTransparentBlt(hDC, m_tRect.left + (int)fScrollX, m_tRect.top + (int)fScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			hMemDC, 0, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));



		NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"DmgFont"));

		/*배열 1자리 수 씩 이미지 읽어와서 출력.*/
		int iXLocate = 0;
		for (int i = m_iArrSize - 1; i >= 0; --i)
		{

		if (m_iComboCnt > 10)
			m_iComboCnt = 10;

			GdiTransparentBlt(hDC, m_tRectCnt.left + iXLocate + (int)fScrollX, m_tRectCnt.top + (int)fScrollY, (int)m_tInfoRectCnt.fCX, (int)m_tInfoRectCnt.fCY,
				hMemDC, m_iComboCnt * (int)m_tInfoRectCnt.fCX, 0, (int)m_tInfoRectCnt.fCX, (int)m_tInfoRectCnt.fCY, RGB(0,0,0));

			iXLocate += 23;
		}

	}
}

void CComboAttack::Release()
{

}

void CComboAttack::UpdateRectCnt()
{
	m_tRectCnt.left		= (LONG)(m_tInfoRectCnt.fX - m_tInfoRectCnt.fCX * 0.5f);
	m_tRectCnt.top		= (LONG)(m_tInfoRectCnt.fY - m_tInfoRectCnt.fCY * 0.5f);
	m_tRectCnt.right	= (LONG)(m_tInfoRectCnt.fX + m_tInfoRectCnt.fCX * 0.5f);
	m_tRectCnt.bottom	= (LONG)(m_tInfoRectCnt.fY + m_tInfoRectCnt.fCY * 0.5f);;
}
