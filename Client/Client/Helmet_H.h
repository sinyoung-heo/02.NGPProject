#pragma once
#include "Item.h"
class CHelmet_H :
	public CItem
{
public:
	CHelmet_H();
	virtual ~CHelmet_H();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int	Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

};

