#pragma once
#include "Item.h"
class CShoes_H :
	public CItem
{
public:
	CShoes_H();
	virtual ~CShoes_H();

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int	Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

};

