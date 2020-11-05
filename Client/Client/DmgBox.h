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
	// CObj��(��) ���� ��ӵ�
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

	float m_fV;	/* �� */
	float m_fT;	/* ���ӵ� */


	BLENDFUNCTION	m_BlendFunction;

};

