#pragma once
#include <Windows.h>

class CTimer
{
public:
	explicit CTimer();
	~CTimer();

public:
	float	GetTimeDelta() { return m_fTimeDelta; }
	HRESULT			ReadyTimer();
	void			UpdateTimeDelta();

private:
	LARGE_INTEGER m_FrameTime;
	LARGE_INTEGER m_FixTime;
	LARGE_INTEGER m_LastTime;
	LARGE_INTEGER m_CpuTick;

	float m_fTimeDelta = 0.0f;

public:
	static CTimer*	Create();
	void			Free();
};

