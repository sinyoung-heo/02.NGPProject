#pragma once
#include "Skill.h"
class CMultiAttack :
	public CSkill
{
public:
	CMultiAttack();
	virtual ~CMultiAttack();

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	CObj* m_pPlayer;

};

