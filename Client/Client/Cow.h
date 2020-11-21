#pragma once
#include "Jelly.h"
class CCow :
	public CJelly
{
public:
	enum STANCE { IDLE, RUN, ATTACK, DEAD, END };
	enum STATE	{ REST, ANGRY, RAGE, END_STATE };

public:
	CCow();
	virtual ~CCow();

public:
	const int&		GetIdx() { return m_iIdx; }

	void			SetStance(const STANCE& eStance) { m_eCurStance = eStance; }
	void			SetIdx(const int& idx) { m_iIdx = idx; }
	void			SetHpInfo(const int& hp) { m_tInfo.iHp = hp; }

public:
	virtual void	Initialize() override;
	virtual void	LateInit();
	virtual int		Update() override;
	virtual void	LateUpdate() override;
	virtual void	Render(HDC hDC) override;
	virtual void	Release() override;

public:
	void SetStanceHIT()		{ m_bIsHit = true; }
	void SetStateAngry()	{ m_eState = ANGRY; }

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


	int		m_iIdx = 0;
};

