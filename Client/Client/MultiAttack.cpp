#include "stdafx.h"
#include "MultiAttack.h"
#include "SkillColBox.h"

static int iTime = 0;

CMultiAttack::CMultiAttack()
	:m_pPlayer(nullptr)
{
}

CMultiAttack::~CMultiAttack()
{
	Release();
}

void CMultiAttack::Initialize()
{

	m_tInfo.fCX = 512.f;
	m_tInfo.fCY = 512.f;

	m_tColInfo.fCX = 50.f;
	m_tColInfo.fCY = 50.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 40;
	m_tFrame.dwFrameSpd = 30;
	m_tFrame.dwOldTime = GetTickCount();

	m_pPlayer = CObjMgr::GetInstance()->GetPlayer();
}

void CMultiAttack::LateInit()
{
	CObj::UpdateColXYPos();	//충돌박스 좌표 = 이미지 좌표

	CSoundMgr::GetInstance()->PlaySound(L"BossPotalSkill.wav", CSoundMgr::MULTIATTACK);

	/*스킬 위치 조정.*/
	m_tInfo.fY -= 10.f;
}

int CMultiAttack::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;
	
	CObj::UpdateColXYPos();

	iTime += 10;

	

	///*충돌박스 생성 횟수*/
	//if(iTime % 130 == 0)
	//	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CSkillColBox>::CreateColBox(50.f, 50.f, m_tInfo.fX, m_tInfo.fY, (int)(m_pPlayer->GetInfo().iAtt*0.8f)),
	//		ObjID::SKILL_COLBOX);

	return NO_EVENT;
}

void CMultiAttack::LateUpdate()
{
	/*스킬 재생시간.*/
	if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd || iTime > 1000)
	{
		CObjMgr::GetInstance()->DeleteObj(ObjID::SKILL_COLBOX);
		bIsDead();
		iTime = 0;
	}

	CObj::MoveFrame();
}

void CMultiAttack::Render(HDC hDC)
{
	CObj::UpdateRect();

	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"MultiAttack"));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	GdiTransparentBlt(hDC, m_tRect.left + (int)fScrollX, m_tRect.top + (int)fScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
		hMemDC, m_tFrame.iFrameStart * (int)m_tInfo.fCX, 0,
		(int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255,0,255));
}

void CMultiAttack::Release()
{
}
