#pragma once
#include "Struct.h"
#include "Protocol.h"
using namespace PROTOCOL_TEST;

class CMonster
{
public:
    CMonster() = default;
    ~CMonster() = default;

public:
    void    SetTarget(CINFO* pTarget) { target = pTarget; };
    int     UpdateMonster(const float& fTimeDelta);

private:
    void UpdateCow(const float& fTimeDelta);
    void UpdateNinja(const float& fTimeDelta);
    void UpdateBoris(const float& fTimeDelta);

    float CalcDist();
    float CalcDegree();

public:
    char    type        = MON_COW;  // Monster Type
    int     idx         = 0;        // Monster Index

    int     hp          = 0; 
    int     att         = 0;
    int     exp         = 0;
    float   x   = 0, y  = 0;
    float   cx  = 0, cy = 0;
    float   speed       = 0.0f;
    float   angle       = 0.0f;
    bool    is_dead     = false;

    char    scene_id    = 0;
    char    cur_stance  = 0;
    char    cur_state   = 0;
    char    cur_dir     = 0;

    CINFO*  target      = nullptr;
    float   dist        = 0.0f;
};

