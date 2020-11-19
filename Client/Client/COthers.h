#pragma once
#include "Obj.h"
class COthers :
    public CObj
{
public:
    enum STANCE { IDLE, WALK, ATTACK, HIT, DEAD, END };
public:
    COthers();
    virtual ~COthers();

public:
    // CObj을(를) 통해 상속됨
    virtual void	Initialize() override;
    virtual void	LateInit();
    virtual int		Update() override;
    virtual void	LateUpdate() override;
    virtual void	Render(HDC hDC) override;
    virtual void	Release() override;

private:
    void SceneChange();

private:
    wstring m_pFrameKey;
    STANCE	m_eCurStance;
    STANCE	m_ePreStance;
};

