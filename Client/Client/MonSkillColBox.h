#pragma once
#include "Obj.h"
class CMonSkillColBox :
	public CObj
{
public:
	CMonSkillColBox();
	virtual ~CMonSkillColBox();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

};

