#pragma once
#include "Obj.h"
class CMyButton :
	public CObj
{
public:
	CMyButton();
	virtual ~CMyButton();

public:
	void SetImageKey(wstring pImageKey) { m_pImageKey = pImageKey; }

public:
	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	wstring m_pImageKey;
	int	   m_iDrawID;
};

