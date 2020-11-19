#pragma once
#include "Obj.h"
class COthers :
    public CObj
{
public:
    enum STANCE			{ IDLE, WALK, RUN, ATTACK, SKILL, HIT, END };
	enum SKILL_STATE	{ SOUL, MOON, MULTI, BASIC, SKILL_END };
public:
    COthers();
    virtual ~COthers();

public:
    const STANCE& GetStance() { return m_eCurStance; }
    const ObjDir::DIR& GetDir() { return m_eCurDir; }
    const wstring& GetFrameKey() { return m_pFrameKey; }

    void SetStance(STANCE eStance)   { m_eCurStance = eStance; }
    void SetCurDir(ObjDir::DIR eDir) { m_eCurDir = eDir; }

    // CObj을(를) 통해 상속됨
    virtual void	Initialize() override;
    virtual void	LateInit();
    virtual int		Update() override;
    virtual void	LateUpdate() override;
    virtual void	Render(HDC hDC) override;
    virtual void	Release() override;

private:
    void SceneChange();
    void UpdateAnimationKey();

private:
    wstring m_pFrameKey;

    STANCE	m_eCurStance;
    STANCE	m_ePreStance;
};

