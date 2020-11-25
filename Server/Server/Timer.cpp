#include "Timer.h"

CTimer::CTimer()
{
    ZeroMemory(&m_FrameTime, sizeof(LARGE_INTEGER));
    ZeroMemory(&m_FixTime, sizeof(LARGE_INTEGER));
    ZeroMemory(&m_LastTime, sizeof(LARGE_INTEGER));
    ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
}

CTimer::~CTimer()
{
    Free();
}

HRESULT CTimer::ReadyTimer()
{
    QueryPerformanceCounter(&m_FrameTime);
    QueryPerformanceCounter(&m_FixTime);
    QueryPerformanceCounter(&m_LastTime);
    QueryPerformanceFrequency(&m_CpuTick);

    return S_OK;
}

void CTimer::UpdateTimeDelta()
{
    /*____________________________________________________________________
    TimeDelta ����.
    ______________________________________________________________________*/
    QueryPerformanceCounter(&m_FrameTime);

    if (m_FrameTime.QuadPart - m_LastTime.QuadPart > m_CpuTick.QuadPart)
    {
        QueryPerformanceFrequency(&m_CpuTick);
        m_LastTime.QuadPart = m_FrameTime.QuadPart;
    }

    m_fTimeDelta = float(m_FrameTime.QuadPart - m_FixTime.QuadPart) / m_CpuTick.QuadPart;

    m_FixTime = m_FrameTime;
}

CTimer* CTimer::Create()
{
    CTimer* pInstance = new CTimer;

    if (FAILED(pInstance->ReadyTimer()))
    {
        if (pInstance)
        {
            delete pInstance;
            pInstance = nullptr;
        }
    }

    return pInstance;
}

void CTimer::Free()
{
}
