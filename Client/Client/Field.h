#pragma once
#include "Scene.h"
class CField :
	public CScene
{
public:
	CField();
	virtual ~CField();

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

