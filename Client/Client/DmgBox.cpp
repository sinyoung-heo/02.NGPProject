#include "stdafx.h"
#include "DmgBox.h"


static int iTime = 0;

CDmgBox::CDmgBox()
	:m_iDmg(0), m_iArrSize(0), m_pTarget(nullptr)
{
	for (int i = 0; i < 8; ++i)
		m_iDmgArr[i] = 0;
}

CDmgBox::~CDmgBox()
{
	Release();
}

void CDmgBox::Initialize()
{
	m_eLayer = Layer::EFFECT;

	m_tInfo.fCX = 23.f;
	m_tInfo.fCY = 30.f;

	m_pTarget = CObjMgr::GetInstance()->GetPlayer();

	m_fV = 12.f;
	m_fT = 0.f;


	m_BlendFunction.BlendOp = 0;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.SourceConstantAlpha = 255;
	m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
}

void CDmgBox::LateInit()
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

	m_fAngle = 88.f;//(float)(rand() % 90 + 85);
}

int CDmgBox::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;
					
	m_tInfo.fX -= m_fV*cosf(m_fAngle*PI / 180.f)*m_fT;
	m_tInfo.fY -= m_fV * m_fT - GRAVITY * m_fT * m_fT * 0.5f;

	m_fT += 0.1f;

	iTime += 5;


	m_BlendFunction.SourceConstantAlpha -= (BYTE)5;


	return NO_EVENT;
}

void CDmgBox::LateUpdate()
{
	if(m_tInfo.fY  > m_pTarget->GetInfo().fY)
		m_bIsDead = true;
}

void CDmgBox::Render(HDC hDC)
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
		AlphaBlend(hDC, m_tRect.left + iXLocate + (int)fScrollX, m_tRect.top + (int)fScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			hMemDC, m_iDmgArr[i] * (int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, m_BlendFunction);

		iXLocate += 23;
	}

}

void CDmgBox::Release()
{

}
