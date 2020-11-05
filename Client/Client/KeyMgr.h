#pragma once

// 0xff -> 1111 1111
#define MAX_VKEY 0xff

class CKeyMgr
{
	DECLARE_SINGLETON(CKeyMgr)


private:
	CKeyMgr();
	~CKeyMgr();

public:
	bool KeyPressing(int Vkey);
	bool KeyDown(int Vkey);
	bool KeyUp(int Vkey);

public:
	// �� �����Ӹ��� ���� Ű ���¸� �������� �Լ�.
	void UpdateKey();

private:
	bool m_VKey[MAX_VKEY];
};

#define KEY_PRESSING(VKey) CKeyMgr::GetInstance()->KeyPressing(VKey)
#define KEY_DOWN(VKey) CKeyMgr::GetInstance()->KeyDown(VKey)
#define KEY_UP(VKey) CKeyMgr::GetInstance()->KeyUp(VKey)
