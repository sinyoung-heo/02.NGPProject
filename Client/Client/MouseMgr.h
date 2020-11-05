#pragma once
class CMouseMgr
{
	DECLARE_SINGLETON(CMouseMgr)

public:
	enum STANCE { IDLE, ATTACK, FAIL, LCLICK, END };

private:
	CMouseMgr();
	~CMouseMgr();


public:
	void Setm_pFrameKey(wstring pFrameKey) { m_pFrameKey = pFrameKey; }
	void SetMouseStance(STANCE _stance) { m_eCurStance = _stance; }

public:
	void Initialize();
	void Update();
	void LateUpdate();
	void Render(HDC hDC);
	void Release();

public:
	void MoveFrame();
	void SceneChange();

	void UpdateColRect();

private:
	/*충돌박스 정보*/
	COLLINFO m_tCollInfo;
	RECT	 m_tCollRect;

	FRAME m_tFrame;

	wstring m_pFrameKey;
	STANCE m_eCurStance;
	STANCE m_ePreStance;

	BLENDFUNCTION	m_BlendFunction;
};

