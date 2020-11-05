#pragma once
#include "Skill.h"
class CSkillMoon :
	public CSkill
{
public:
	CSkillMoon();
	virtual ~CSkillMoon();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	CObj* m_pPlayer;
	BLENDFUNCTION	m_BlendFunction;
};

