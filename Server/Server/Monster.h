#pragma once
#include "Struct.h"
#include "Protocol.h"
using namespace PROTOCOL_TEST;

typedef struct tagFrame
{
    float   frame_start = 0.0f;
    float   frame_end   = 0.0f;	
    float   frame_speed = 0.0f;

}FRAME;

class CMonster
{
public:
    CMonster() = default;
    ~CMonster() = default;

public:
    void        SetTarget(CINFO* pTarget) { target = pTarget; };

public:
    int     UpdateMonster(const float& fTimeDelta);

private:
    void MoveFrame(const float& fTimeDelta);

    void UpdateCow(const float& fTimeDelta);
    void SceneChangeCow();

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
    float   radius      = 50.0f;

    FRAME   frame;

    char    scene_id        = 0;
    char    cur_stance      = 0;
    char    cur_state       = 0;
    char    cur_dir         = 0;

    char    pre_stance      = 0;

    CINFO*  target          = nullptr;
    float   dist            = 0.0f;

    // Collision.
    bool    is_collision    = true;
    float   updatetime      = 0.5f;
    float   collision_time  = 0.0f;

    float att_x             = 0.0f;
    float att_y             = 0.0f;
    float att_len           = 0.0f;
    float att_radius        = 50.0f;
};

