#pragma once
#include "Obj.h"
class CHpPotionEffect :
	public CObj
{
public:
	CHpPotionEffect();
	virtual ~CHpPotionEffect();

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	BLENDFUNCTION	m_BlendFunction;

};

