#include "stdafx.h"
#include "Skill.h"
#include "SkillColBox.h"

static int iTime = 0;

CSkill::CSkill()
{
}

CSkill::~CSkill()
{
	Release();
}

void CSkill::Initialize()
{
	m_eLayer = Layer::EFFECT;

	m_tColInfo.fCX = 50.f;
	m_tColInfo.fCY = 50.f;

	m_pPlayer = CObjMgr::GetInstance()->GetPlayer();
}

void CSkill::LateInit()
{
	//CObjMgr::GetInstance()->AddObject(CAbstractFactory<CSkillColBox>::CreateColBox(50.f, 50.f, m_tInfo.fX, m_tInfo.fY, m_pPlayer->GetInfo().iAtt),
	//	ObjID::SKILL_COLBOX);
}

int CSkill::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;

	iTime += 5;


	return NO_EVENT;
}

void CSkill::LateUpdate()
{
	/*스킬 재생시간.*/
	if (iTime > 100)
	{
		CObjMgr::GetInstance()->DeleteObj(ObjID::SKILL_COLBOX);
		bIsDead();
		iTime = 0;
	}
}

void CSkill::Render(HDC hDC)
{
	CObj::UpdateColRect();

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	Rectangle(hDC, m_tColRect.left + (int)fScrollX, m_tColRect.top + (int)fScrollY,
		m_tColRect.right + (int)fScrollX, m_tColRect.bottom + (int)fScrollY);
}

void CSkill::Release()
{

}
