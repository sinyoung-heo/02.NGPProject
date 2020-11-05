#pragma once
#include "Obj.h"
class CInvenButton :
	public CObj
{
public:
	CInvenButton();
	virtual ~CInvenButton();

	// CObj을(를) 통해 상속됨
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

	CObj* m_pPlayerInven;

	bool m_sound;

};

