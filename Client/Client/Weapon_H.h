#pragma once
#include "Item.h"
class CWeapon_H :
	public CItem
{
public:
	CWeapon_H();
	virtual ~CWeapon_H();

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int	Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

};

