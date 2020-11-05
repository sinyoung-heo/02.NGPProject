#pragma once

class CObj;

class CTileMgr
{
	DECLARE_SINGLETON(CTileMgr)

private:
	CTileMgr();
	~CTileMgr();

public:
	void Initialize();
	void Update();
	void LateUpdate();
	void Render(HDC hDC);
	void Release();

public:
	vector<CObj*>& GetVecTile() { return m_vecTile; }

public:
	void SaveData(wstring pMapImageKey);
	void LoadData(wstring pMapImageKey);

public:
	void PickTile(int iIndex, int iDrawID);

private:
	vector<CObj*>	m_vecTile;
};

