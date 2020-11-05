#pragma once
#include "Scene.h"
class CTown :
	public CScene
{
public:
	CTown();
	virtual ~CTown();

	// CScene��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	bool m_bIsCreate;

	/*Ÿ�� �����ֱ��*/
	bool m_TileOn;

};

