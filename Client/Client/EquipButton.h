#pragma once
#include "Obj.h"
class CEquipButton :
	public CObj
{
public:
	CEquipButton();
	virtual ~CEquipButton();

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void SetDrawKey(int _key) { m_iDrawKey = _key; }

private:
	int m_iDrawKey;
	bool m_bIsUiOn;

	CObj* m_pPlayerEquip;

	bool m_sound;
};

