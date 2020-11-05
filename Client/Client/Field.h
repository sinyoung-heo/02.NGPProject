#pragma once
#include "Scene.h"
class CField :
	public CScene
{
public:
	CField();
	virtual ~CField();

	// CScene을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	/*타일 보여주기용*/
	bool m_TileOn;

};

