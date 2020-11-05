#include "stdafx.h"
#include "ComboCnt.h"

static int iLifeTime = 0;

CComboCnt::CComboCnt()
	:m_iDmg(0), m_iArrSize(0)
{
	for (int i = 0; i < 8; ++i)
		m_iDmgArr[i] = 0;
}

CComboCnt::~CComboCnt()
{
	Release();
}

void CComboCnt::Initialize()
{
	m_eLayer = Layer::EFFECT;

	m_tInfo.fCX = 23.f;
	m_tInfo.fCY = 30.f;
}

void CComboCnt::LateInit()
{
	/* DMG를 1자리 수 씩 끊어서 배열에 넣어준다.*/
	int i = 0;

	while (true)
	{
		m_iDmgArr[i] = m_iDmg % 10;

		++m_iArrSize;
		++i;

		m_iDmg = m_iDmg / 10;

		if (m_iDmg / 10 == 0)
		{
			m_iDmgArr[i] = m_iDmg % 10;
			++m_iArrSize;

			break;
		}
	}
}

int CComboCnt::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;


	++iLifeTime;


	return NO_EVENT;
}

void CComboCnt::LateUpdate()
{
	if (iLifeTime > 50)
	{
		m_bIsDead = true;
		iLifeTime = 0;
	}
}

void CComboCnt::Render(HDC hDC)
{
	CObj::UpdateRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"DmgFont"));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	/*배열 1자리 수 씩 이미지 읽어와서 출력.*/
	int iXLocate = 0;
	for (int i = m_iArrSize - 1; i >= 0; --i)
	{
		GdiTransparentBlt(hDC, m_tRect.left + iXLocate + (int)fScrollX, m_tRect.top + (int)fScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			hMemDC, m_iDmgArr[i] * (int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(0,0,0));

		iXLocate += 23;
	}
}

void CComboCnt::Release()
{

}
