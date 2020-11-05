#include "stdafx.h"
#include "TileMgr.h"
#include "Tile.h"
#include "Player.h"

#define GAME_START

IMPLEMENT_SINGLETON(CTileMgr)

CTileMgr::CTileMgr()
{
}


CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{
	float fX = 0.f, fY = 0.f;

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			fX = TILECX * 0.5f + TILECX * j;
			fY = TILECY * 0.5f + TILECY * i;

			m_vecTile.push_back(CAbstractFactory<CTile>::CreateObj(fX, fY));
		}
	}
}

void CTileMgr::Update()
{
	
	int iScrollX = (int)CScrollMgr::GetScrollX();
	int iScrollY = (int)CScrollMgr::GetScrollY();

	
	int iCullX = -iScrollX / TILECX;
	int iCullY = -iScrollY / TILECY;

	
	int iCullEndX = iCullX + WINCX / TILECX;
	int iCullEndY = iCullY + WINCY / TILECY;

	for (int i = iCullY; i < iCullEndY; ++i)
	{
		for (int j = iCullX; j < iCullEndX; ++j)
		{
			int iIndex = j + (TILEX*i);

			if (iIndex < 0 || (size_t)iIndex >= m_vecTile.size())
				continue;

			m_vecTile[iIndex]->Update();
		}
	}
}

void CTileMgr::LateUpdate()
{
	
	int iScrollX = (int)CScrollMgr::GetScrollX();
	int iScrollY = (int)CScrollMgr::GetScrollY();

	
	int iCullX = -iScrollX / TILECX;
	int iCullY = -iScrollY / TILECY;

	
	int iCullEndX = iCullX + WINCX / TILECX;
	int iCullEndY = iCullY + WINCY / TILECY;

	for (int i = iCullY; i < iCullEndY; ++i)
	{
		for (int j = iCullX; j < iCullEndX; ++j)
		{
			int iIndex = j + (TILEX*i);

			if (iIndex < 0 || (size_t)iIndex >= m_vecTile.size())
				continue;

			if (m_vecTile.size() > TILEX * TILEY)
				return;


			m_vecTile[iIndex]->LateUpdate();
			
#ifdef GAME_START
			/*Tile & Player 충돌체크*/
			if (!CObjMgr::GetInstance()->GetPlayerLst().empty())
			{
				CObj* pPlayer = CObjMgr::GetInstance()->GetPlayer();
				if (nullptr != pPlayer)
					CCollisionMgr::CollisionTile(m_vecTile[iIndex], pPlayer);
			}
#endif
		}
	}
}

void CTileMgr::Render(HDC hDC)
{
	
	int iScrollX = (int)CScrollMgr::GetScrollX();
	int iScrollY = (int)CScrollMgr::GetScrollY();

	
	int iCullX = -iScrollX / TILECX;
	int iCullY = -iScrollY / TILECY;

	
	int iCullEndX = iCullX + WINCX / TILECX;
	int iCullEndY = iCullY + WINCY / TILECY;

	for (int i = iCullY; i < iCullEndY + 2; ++i)
	{
		for (int j = iCullX; j < iCullEndX + 2; ++j)
		{
			int iIndex = j + (TILEX*i);

			if (iIndex < 0 || (size_t)iIndex >= m_vecTile.size())
				continue;

			m_vecTile[iIndex]->Render(hDC);
		}
	}
}

void CTileMgr::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), SafeDelete<CObj*>);

	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

/*저장*/
void CTileMgr::SaveData(wstring pMapImageKey)
{
	/*이미지 키 값을 받아서 파일명으로 지정.*/
	TCHAR  pFileFormat[6] = L".dat";
	TCHAR  pFilePath[32] = L"../Data/";

	lstrcat(pFilePath, pMapImageKey.c_str());
	lstrcat(pFilePath, pFileFormat);
	/*  L"../Data/pMapImageKey.dat"  */

	HANDLE hFile = CreateFile(pFilePath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, 0);


	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Save Failed", L"Fail", MB_OK);
		return;
	}

	INFO tInfo		= {};
	int iDrawID		= 0;
	DWORD dwByte	= 0;

	for (auto& pTile : m_vecTile)
	{
		tInfo = pTile->GetInfo();
		iDrawID = dynamic_cast<CTile*>(pTile)->GetDrawID();

		WriteFile(hFile, &tInfo, sizeof(INFO), &dwByte, 0);
		WriteFile(hFile, &iDrawID, sizeof(int), &dwByte, 0);
	}
	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Save Successed", L"Successed", MB_OK);
}

/*불러오기*/
void CTileMgr::LoadData(wstring pMapImageKey)
{
	/*불러오기 전에 초기화 벡터 비워준 후 불러옴.*/
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();

	/*이미지 키 값을 받아서 파일명으로 지정.*/
	TCHAR  pFileFormat[6] = L".dat";
	TCHAR  pFilePath[32] = L"../Data/";

	lstrcat(pFilePath, pMapImageKey.c_str());
	lstrcat(pFilePath, pFileFormat);
	/*  L"../Data/pMapImageKey.dat"  */

	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Load Failed", L"Fail", MB_OK);
		return;
	}

	INFO tInfo = {};
	int iDrawID = 0;
	DWORD dwByte = 0;

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO) - sizeof(wstring) + sizeof(TCHAR*), &dwByte, 0);
		ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, 0);

		if (dwByte == 0)
			break;

		CObj* pTile = CAbstractFactory<CTile>::CreateObj(tInfo);
		dynamic_cast<CTile*>(pTile)->SetDrawID(iDrawID);

		m_vecTile.push_back(pTile);
	}

	CloseHandle(hFile);

}

void CTileMgr::PickTile(int iIndex, int iDrawID)
{
	if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
		return;

	dynamic_cast<CTile*>(m_vecTile[iIndex])->SetDrawID(iDrawID);
}
