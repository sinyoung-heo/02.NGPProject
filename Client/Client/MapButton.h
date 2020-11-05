#pragma once
#include "Obj.h"

class CMapButton :
	public CObj
{
public:
	CMapButton();
	virtual ~CMapButton();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	TCHAR* m_pMapImageKey;
};

