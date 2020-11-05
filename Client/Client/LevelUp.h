#pragma once
#include "Obj.h"
class CLevelUp :
	public CObj
{
public:
	CLevelUp();
	virtual ~CLevelUp();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

};

