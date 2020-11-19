#pragma once

class CScene;


class CSceneMgr
{
	DECLARE_SINGLETON(CSceneMgr)

public:
	enum SCENE 
	{
		SC_LOGO, SC_MENU, SC_EDIT, SC_TOWN,
		SC_FIELD, SC_DUNGEON, SC_BOSS, SC_STORE,SC_END
	};

public:
	SCENE& GetCurScene() { return m_eCurScene; }

private:
	CSceneMgr();
	~CSceneMgr();

public:
	void Update();
	void LateUpdate();
	void Render(HDC hDC);
	void Release();

	void SceneChange(SCENE eScene);

private:
	CScene* m_pScene;

	SCENE m_eCurScene;
	SCENE m_ePreScene;
};

