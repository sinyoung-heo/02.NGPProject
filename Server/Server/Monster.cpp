#include "Monster.h"
#include <cmath>
#include <iostream>
using namespace std;

int CMonster::UpdateMonster(const float& fTimeDelta)
{
    // cout << fTimeDelta << endl;
    if (is_dead)
        return SERVER_DEADOBJ;

    if (fTimeDelta > 1.f)
        return SERVER_NOEVENT;

    if (nullptr != target && !is_dead)
    {
        angle = CalcDegree();

        if (MON_COW == type)
            UpdateCow(fTimeDelta);
        else if (MON_NINJA == type)
            UpdateNinja(fTimeDelta);
        else if (MON_BORIS == type)
            UpdateBoris(fTimeDelta);
    }

    MoveFrame(fTimeDelta);

    return SERVER_NOEVENT;
}

void CMonster::MoveFrame(const float& fTimeDelta)
{
    if (fTimeDelta > 1.f)
        return;

    frame.frame_start += frame.frame_speed * fTimeDelta;

    if ((int)frame.frame_start > (int)frame.frame_end)
        frame.frame_start = 0.0f;
}

void CMonster::UpdateCow(const float& fTimeDelta)
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
        // Target 추격.
        cur_stance = MON_STANCE_RUN;

        x += cosf(angle) * speed * fTimeDelta;
        y -= sinf(angle) * speed * fTimeDelta;

        dist = CalcDist();
        if (dist < 75.0f)
            cur_state = MON_STATE_RAGE;
    }
    break;

    case MON_STATE_RAGE:
    {
        // Target 공격.
        cur_stance = MON_STANCE_ATTACK;

        dist = CalcDist();
        if (dist > 100.0f)
            cur_state = MON_STATE_ANGRY;
    }
    break;

    }

    SceneChangeCow();
}

void CMonster::SceneChangeCow()
{
    if (cur_stance != pre_stance)
    {
		switch (cur_stance)
		{
		case MON_STANCE_ATTACK:
			frame.frame_start   = 0.0f;
			frame.frame_end     = 9.0f;
			frame.frame_speed   = 9.0f;
			break;
		case MON_STANCE_DEAD:
			frame.frame_start   = 0;
			frame.frame_end     = 10;
			frame.frame_speed   = 10;
			break;
		case MON_STANCE_IDLE:
			frame.frame_start   = 0.0f;
			frame.frame_end     = 5.0f;
			frame.frame_speed   = 5.0f;
			break;
		case MON_STANCE_RUN:
			frame.frame_start   = 0;
			frame.frame_end     = 7;
			frame.frame_speed   = 7;
			break;
		}

        pre_stance = cur_state;
    }
}

void CMonster::UpdateNinja(const float& fTimeDelta)
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

void CMonster::UpdateBoris(const float& fTimeDelta)
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

float CMonster::CalcDist()
{
    float w = target->x - x;
    float h = target->y - y;

    return sqrtf(w * w + h * h);
}

float CMonster::CalcDegree()
{
    float w = target->x - x;
    float h = target->y - y;
    float d = sqrtf(w * w + h * h);

    float fAngle = acosf(w / d);

    if (y < target->y)
        fAngle *= -1.f;

    return fAngle * 180.f / 3.141592f;
}
