#pragma once
class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void Initialize();
	void Update();
	void LateUpdate();
	void Render();
	void Release();

public:
	// Server ����� ���� �Լ� ����
	void Ready_Server();
	void Connect_Server();


private:
	HDC m_hDC;

};

