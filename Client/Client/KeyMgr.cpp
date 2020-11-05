#include "stdafx.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr)


CKeyMgr::CKeyMgr()
{
	ZeroMemory(m_VKey, sizeof(m_VKey));
}
CKeyMgr::~CKeyMgr()
{

}


bool CKeyMgr::KeyPressing(int Vkey) // ��� ������ ���� ��
{
	if (GetAsyncKeyState(Vkey) & 0x8000)
		return true;

	return false;
}

bool CKeyMgr::KeyDown(int Vkey)	// �ѹ��� ������ ��
{
	if (!m_VKey[Vkey] && (GetAsyncKeyState(Vkey) & 0x8000))
	{
		m_VKey[Vkey] = !m_VKey[Vkey];
		return true;
	}

	return false;
}


bool CKeyMgr::KeyUp(int Vkey) // �����ٰ� ���� ��
{
	if (m_VKey[Vkey] && !(GetAsyncKeyState(Vkey) & 0x8000))
	{
		m_VKey[Vkey] = !m_VKey[Vkey];
		return true;
	}

	return false;
}


void CKeyMgr::UpdateKey()
{
	for (int i = 0; i < MAX_VKEY; ++i)
	{
		if (m_VKey[i] && !(GetAsyncKeyState(i) & 0x8000))
			m_VKey[i] = !m_VKey[i];

		if (!m_VKey[i] && (GetAsyncKeyState(i) & 0x8000))
			m_VKey[i] = !m_VKey[i];
	}
}
