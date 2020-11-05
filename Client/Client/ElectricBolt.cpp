#include "stdafx.h"
#include "ElectricBolt.h"
#include "MonSkillColBox.h"


static int iTime = 0;

CElectricBolt::CElectricBolt()
{
}

CElectricBolt::~CElectricBolt()
{
	Release();
}

void CElectricBolt::Initialize()
{
	m_eLayer = Layer::EFFECT;

	m_tInfo.fCX = 150.f;
	m_tInfo.fCY = 150.f;

	m_tColInfo.fCX = 150.f;
	m_tColInfo.fCY = 150.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.dwFrameSpd = 200;
	m_tFrame.dwOldTime = GetTickCount();

	bIsCreate = false;
}

void CElectricBolt::LateInit()
{
	CObj::UpdateColXYPos();

	CSoundMgr::GetInstance()->PlaySound(L"ThunderBolt.mp3", CSoundMgr::BOSS_SKILL);

}

int CElectricBolt::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;

	CObj::UpdateColXYPos();

	iTime += 5;

	/*충돌박스 생성 횟수*/
	if (!bIsCreate)
	{
		CObjMgr::GetInstance()->AddObject(CAbstractFactory<CMonSkillColBox>::CreateColBox(m_tColInfo.fCX, m_tColInfo.fCY, m_tInfo.fX, m_tInfo.fY, 100),
			ObjID::MON_ATT_COLBOX);

		bIsCreate = true;
	}



	return NO_EVENT;
}

void CElectricBolt::LateUpdate()
{
	/*스킬 재생시간.*/
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	{

		CObjMgr::GetInstance()->DeleteObj(ObjID::MON_ATT_COLBOX);
		bIsDead();
		iTime = 0;
	}

	CObj::MoveFrame();
}

void CElectricBolt::Render(HDC hDC)
{
	CObj::UpdateRect();
	CObj::UpdateColRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"ElectricBolt"));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	GdiTransparentBlt(hDC, m_tRect.left + (int)fScrollX, m_tRect.top -90 + (int)fScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMemDC, m_tFrame.iFrameStart*(int)m_tInfo.fCX, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));
}

void CElectricBolt::Release()
{

}
