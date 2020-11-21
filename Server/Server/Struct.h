#pragma once
#include "Protocol.h"
using namespace PROTOCOL_TEST;

typedef struct tagClientInfo
{
    /* ∞‘¿” ƒ‹≈Ÿ√˜ */
    char    name[MAX_ID_LEN];
    int     level;
    int     hp;
    int     mp;
    int     exp;
    int     sp;
    int     att;
    float   x, y;
    float   speed;

    char    scene_id;
    char    cur_stance;
    char    cur_dir;

    /* Ω√Ω∫≈€ ƒ‹≈Ÿ√˜ */
    bool    in_use;
    SOCKET  m_sock;
    char    m_packet_buf[MAX_BUF_SIZE];

}CINFO;

class CMonsterInfo
{
public:
    CMonsterInfo() = default;
    ~CMonsterInfo() = default;

public:
    void    SetTarget(CINFO* pTarget) { target = pTarget; };
    int     Update()
    {
        if (is_dead)
            return SERVER_DEADOBJ;

        if (nullptr != target && !is_dead)
        {
            angle = CalcDegree();

            if (MON_COW == type)
                UpdateCow();
            else if (MON_NINJA == type)
                UpdateNinja();
            else if (MON_BORIS == type)
                UpdateBoris();
        }

        return SERVER_NOEVENT;
    }

private:
    void UpdateCow()
    {
        switch (cur_state)
        {
        case MON_STATE_REST:
        {
            dist = CalcDist();

            if (dist < 250.0f)
                cur_state = MON_STATE_ANGRY;

        }
        break;

        case MON_STATE_ANGRY:
        {
            // Target √ﬂ∞›.
            cur_stance = MON_STANCE_RUN;

            x += cosf(angle) * speed;
            y -= sinf(angle) * speed;

            dist = CalcDist();
            if (dist < 75.0f)
                cur_state = MON_STATE_RAGE;
        }
        break;

        case MON_STATE_RAGE:
        {
            // Target ∞¯∞›.
            cur_stance = MON_STANCE_ATTACK;

            dist = CalcDist();
            if (dist > 100.0f)
                cur_state = MON_STATE_ANGRY;
        }
        break;

        }
    }

    void UpdateNinja()
    {
        switch (cur_state)
        {
        case MON_STATE_REST:
        {

        }
        break;

        case MON_STATE_ANGRY:
        {

        }
        break;

        case MON_STATE_RAGE:
        {

        }
        break;

        }
    }

    void UpdateBoris()
    {
        switch (cur_state)
        {
        case MON_STATE_REST:
        {

        }
        break;

        case MON_STATE_ANGRY:
        {

        }
        break;

        case MON_STATE_RAGE:
        {

        }
        break;

        case MON_STATE_CRAZY:
        {

        }
        break;
        }
    }

    float CalcDist()
    {
        float w = target->x - x;
        float h = target->y - y;

        return sqrtf(w * w + h * h);
    }

    float CalcDegree()
    {
        float w = target->x - x;
        float h = target->y - y;
        float d = sqrtf(w * w + h * h);

        float fAngle = acosf(w / d);

        if (y < target->y)
            fAngle *= -1.f;

        return fAngle * 180.f / 3.141592f;
    }

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