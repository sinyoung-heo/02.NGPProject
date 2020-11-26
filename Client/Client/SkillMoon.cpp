#include "stdafx.h"
#include "SkillMoon.h"
#include "SkillColBox.h"

static int iTime = 0;

CSkillMoon::CSkillMoon()
	:m_pPlayer(nullptr)
{
}

CSkillMoon::~CSkillMoon()
{
	Release();
}

void CSkillMoon::Initialize()
{
	m_tInfo.fCX = 242.f;
	m_tInfo.fCY = 309.f;

	m_tColInfo.fCX = 50.f;
	m_tColInfo.fCY = 80.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 12;
	m_tFrame.dwFrameSpd = 110;
	m_tFrame.dwOldTime = GetTickCount();

	m_pPlayer = CObjMgr::GetInstance()->GetPlayer();



	m_BlendFunction.BlendOp = 0;
	m_BlendFunction.BlendFlags = 0;
	m_BlendFunction.SourceConstantAlpha = 255;
	m_BlendFunction.AlphaFormat = AC_SRC_ALPHA;
}

void CSkillMoon::LateInit()
{
	CSoundMgr::GetInstance()->PlaySound(L"SKILL_A.wav", CSoundMgr::PLAYER_SKILL);

	CObj::UpdateColXYPos();	//충돌박스 좌표 = 이미지 좌표

}

int CSkillMoon::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;

	CObj::UpdateColXYPos();

	iTime += 10;

	///*충돌박스 생성 횟수*/
	//if(iTime % 110 == 0)
	//	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CSkillColBox>::CreateColBox(50.f, 50.f, m_tInfo.fX, m_tInfo.fY, (int)(m_pPlayer->GetInfo().iAtt*0.9f)),
	//		ObjID::SKILL_COLBOX);


	m_BlendFunction.SourceConstantAlpha -= (BYTE)2;


	return NO_EVENT;
}

void CSkillMoon::LateUpdate()
{
	/*스킬 재생시간.*/
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	{
		CObjMgr::GetInstance()->DeleteObj(ObjID::SKILL_COLBOX);
		bIsDead();
		iTime = 0;
	}

	CObj::MoveFrame();
}

void CSkillMoon::Render(HDC hDC)
{
	CObj::UpdateRect();
	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Skill_Moon"));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();


	AlphaBlend(hDC,
		m_tRect.left + (int)fScrollX, 
		m_tRect.top + (int)fScrollY, 
		(int)m_tInfo.fCX, 
		(int)m_tInfo.fCY,
		hMemDC, m_tFrame.iFrameStart * (int)m_tInfo.fCX,
		0, 
		(int)m_tInfo.fCX, 
		(int)m_tInfo.fCY,
		m_BlendFunction);
}

void CSkillMoon::Release()
{

}
