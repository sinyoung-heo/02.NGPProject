#pragma once
#include "Scene.h"
class CDungeon :
	public CScene
{
public:
	CDungeon();
	virtual ~CDungeon();

	// CScene��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

};

