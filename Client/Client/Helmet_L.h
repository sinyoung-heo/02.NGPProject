#pragma once
#include "Item.h"
class CHelmet_L :
	public CItem
{
public:
	CHelmet_L();
	virtual ~CHelmet_L();

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int	Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

};

