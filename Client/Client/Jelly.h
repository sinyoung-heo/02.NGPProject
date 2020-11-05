#pragma once
#include "Obj.h"
class CJelly :
	public CObj
{
public:
	enum STANCE { ATTACK, HIT, RUN, WALK, END };
	enum STATE { REST, ANGRY, RAGE, END_STATE };

public:
	CJelly();
	virtual ~CJelly();

public:
	void SetStanceHIT() { m_eCurStance = HIT, m_bIsHit = true; }
	void SetStateAngry() { m_eState = ANGRY; }

public:
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	void SceneChange();
	void ChangeImageKey();

private:
	CObj* m_pTarget;

	wstring m_pFrameKey;
	STANCE m_eCurStance;
	STANCE m_ePreStance;

	STATE  m_eState;

	bool	m_bIsHit;
	

	float	m_fAttLen;
	float	m_fDist;

};

