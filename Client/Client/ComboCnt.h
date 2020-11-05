#pragma once
#include "Obj.h"
class CComboCnt :
	public CObj
{
public:
	CComboCnt();
	virtual ~CComboCnt();

public:
	void SetDmgInput(int _dmg) { m_iDmg = _dmg; }

public:
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;


public:
	int m_iDmg;
	int m_iDmgArr[8];
	int m_iArrSize;

};

