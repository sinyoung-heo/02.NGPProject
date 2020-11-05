#pragma once
#include "Jelly.h"
class CNinja :
	public CJelly
{
	enum STANCE { IDLE,RUN,SKILL,ATTACK, DEAD };
	enum STATE  { REST, ANGRY, RAGE, END_STATE };

public:
	CNinja();
	virtual ~CNinja();


	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void SetStanceHIT() { m_bIsHit = true; }
	void SetStateAngry() { m_eState = ANGRY; }

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

