#pragma once
#include "Scene.h"
class CStore :
	public CScene
{
public:
	CStore();
	virtual ~CStore();

	// CScene��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	/*Ÿ�� �����ֱ��*/
	bool m_TileOn;

};

