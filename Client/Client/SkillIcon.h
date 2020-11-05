#pragma once
#include "Obj.h"
class CSkillIcon :
	public CObj
{
public:
	CSkillIcon();
	virtual ~CSkillIcon();

public:
	void SetImageKey(int _imgkey) { m_iImageKey = _imgkey; }

public:
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	int m_iImageKey;

};

