#pragma once
#include "Item.h"
class CArmor_H :
	public CItem
{
public:
	CArmor_H();
	virtual ~CArmor_H();

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int	Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

};

