#pragma once
#include "Obj.h"
class CMonCrash :
	public CObj
{
public:
	CMonCrash();
	virtual ~CMonCrash();

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

};

