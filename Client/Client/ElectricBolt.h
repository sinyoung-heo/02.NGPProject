#pragma once
#include "Skill.h"
class CElectricBolt :
	public CSkill
{
public:
	CElectricBolt();
	virtual ~CElectricBolt();

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	bool bIsCreate;
	CObj* m_pObj;

};

