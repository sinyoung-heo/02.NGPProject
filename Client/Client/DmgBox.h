#pragma once
#include "Obj.h"
class CDmgBox :
	public CObj
{
public:
	CDmgBox();
	virtual ~CDmgBox();

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

private:
	CObj* m_pTarget;

	int m_iDmg;
	int m_iDmgArr[8];
	int m_iArrSize;

	float m_fV;	/* 힘 */
	float m_fT;	/* 가속도 */


	BLENDFUNCTION	m_BlendFunction;

};

