#pragma once
#include "Scene.h"
class CMyEdit :
	public CScene
{
public:
	CMyEdit();
	virtual ~CMyEdit();

public:
	void SetMapImageKey(TCHAR* pMapImageKey) { m_pMapImageKey = pMapImageKey; }

public:
	// CScene을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;


private:
	void IsPicking();

private:
	wstring m_pMapImageKey;
	int m_iDrawID;
};

