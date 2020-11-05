#pragma once
#include "Obj.h"
class CPlayerCrash :
	public CObj
{
public:
	CPlayerCrash();
	virtual ~CPlayerCrash();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

};

