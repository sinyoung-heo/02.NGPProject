#pragma once
#include "Obj.h"
class CLevelUpEffect :
	public CObj
{
public:
	CLevelUpEffect();
	virtual ~CLevelUpEffect();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	BLENDFUNCTION	m_BlendFunction;

};

