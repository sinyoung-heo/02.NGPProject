#pragma once
#include "Obj.h"
class CMonSkillColBox :
	public CObj
{
public:
	CMonSkillColBox();
	virtual ~CMonSkillColBox();

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

};

