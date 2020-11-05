#pragma once
#include "Scene.h"
class CBossField :
	public CScene
{
public:
	CBossField();
	virtual ~CBossField();

	// CScene을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	bool m_bIsClear;

	bool m_clearsound;
};
