#pragma once
#include "Item.h"
class CArmor_L :
	public CItem
{
public:
	CArmor_L();
	virtual ~CArmor_L();

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int	Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

};

