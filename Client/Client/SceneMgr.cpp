#include "stdafx.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Logo.h"
#include "MyMenu.h"
#include "Town.h"
#include "MyEdit.h"
#include "Field.h"
#include "Dungeon.h"
#include "BossField.h"
#include "Store.h"

IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr()
	:m_pScene(nullptr), m_eCurScene(SC_END), m_ePreScene(SC_END)
{
}

CSceneMgr::~CSceneMgr()
{
	Release();
}


void CSceneMgr::Update()
{
	m_pScene->Update();
}

void CSceneMgr::LateUpdate()
{
	m_pScene->LateUpdate();
}

void CSceneMgr::Render(HDC hDC)
{
	m_pScene->Render(hDC);
}

void CSceneMgr::Release()
{
	SafeDelete(m_pScene);
}



void CSceneMgr::SceneChange(SCENE eScene)
{
	m_eCurScene = eScene;

	if (m_eCurScene != m_ePreScene)
	{
		CObjMgr::GetInstance()->RenderlistClear();
		SafeDelete(m_pScene);

		switch (eScene)
		{
		case SC_LOGO:
			m_pScene = new CLogo;
			break;
		case SC_MENU:
			m_pScene = new CMyMenu;
			break;
		case SC_EDIT:
			m_pScene = new CMyEdit;
			break;
		case SC_TOWN:
			m_pScene = new CTown;
			break;
		case SC_STORE:
			m_pScene = new CStore;
			break;
		case SC_FIELD:
			m_pScene = new CField;
			break;
		case SC_DUNGEON:
			m_pScene = new CDungeon;
			break;
		case SC_BOSS:
			m_pScene = new CBossField;
			break;
		}

		m_pScene->Initialize();

		m_ePreScene = m_eCurScene;
	}
}
