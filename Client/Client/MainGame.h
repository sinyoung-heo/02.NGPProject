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
	// Server 통신을 위한 함수 선언
	void Ready_Server();
	void Connect_Server();


private:
	HDC m_hDC;

};

