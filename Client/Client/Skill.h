#pragma once
#include "Obj.h"
class CSkill :
	public CObj
{
public:
	CSkill();
	virtual ~CSkill();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	CObj* m_pPlayer;

};

