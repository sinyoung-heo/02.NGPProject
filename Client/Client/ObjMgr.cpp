#include "stdafx.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "Jelly.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
}

CObjMgr::~CObjMgr()
{
	Release();
}


void CObjMgr::AddObject(CObj * pObj, ObjID::ID eID)
{
	m_ObjLst[eID].push_back(pObj);
}

void CObjMgr::Update()
{
	for (int i = 0; i < ObjID::END; ++i)
	{
		OBJITER iter_begin = m_ObjLst[i].begin();
		OBJITER iter_end = m_ObjLst[i].end();

		for (; iter_begin != iter_end; )
		{
			int iEvent = (*iter_begin)->Update();

			if (m_ObjLst[i].empty())
				break;

			if (DEAD_OBJ == iEvent)
			{
				SafeDelete(*iter_begin);
				iter_begin = m_ObjLst[i].erase(iter_begin);
			}
			else
				++iter_begin;
		}
	}
}

void CObjMgr::LateUpdate()
{
	for (OBJLIST& objlst : m_ObjLst)
	{
		for (CObj*& pObj : objlst)
		{
			pObj->LateUpdate();

			Layer::ID eLayer = pObj->GetLayer();
			m_RenderLst[eLayer].push_back(pObj);
		}
	}

	/*Monster & Player 충돌시 밀어내기*/
	CCollisionMgr::CollisionPush(m_ObjLst[ObjID::MONSTER], m_ObjLst[ObjID::PLAYER]);

	/*Player Skill & 몬스터 충돌*/
	if(!m_ObjLst[ObjID::PLAYER].empty())
		CCollisionMgr::CollisionPlayerSkill(m_ObjLst[ObjID::MONSTER], m_ObjLst[ObjID::SKILL_COLBOX], m_ObjLst[ObjID::PLAYER].front());

	/*Monster 공격 & Player 충돌*/
	CCollisionMgr::CollisionMonsterSkill(m_ObjLst[ObjID::PLAYER], m_ObjLst[ObjID::MON_ATT_COLBOX]);
}

void CObjMgr::Render(HDC hDC)
{
	/* Y랜더 소팅 */
	for (OBJLIST& renderLst : m_RenderLst)
	{
		renderLst.sort(LessY<CObj*>);

		for (CObj*& pObj : renderLst)
			pObj->Render(hDC);

		renderLst.clear();
	}
}

void CObjMgr::Release()
{
	for (int i = 0; i < ObjID::END; ++i)
	{
		for_each(m_ObjLst[i].begin(), m_ObjLst[i].end(),
			[](auto& ptr)
		{
			if (ptr)
			{
				delete ptr;
				ptr = nullptr;
			}
		});
		m_ObjLst[i].clear();
	}
}

void CObjMgr::RenderlistClear()
{
	for (int i = 0; i < Layer::END; ++i)
		m_RenderLst[i].clear();
}

void CObjMgr::DeleteObj(ObjID::ID eID)
{
	for_each(m_ObjLst[eID].begin(), m_ObjLst[eID].end(), SafeDelete<CObj*>);
	m_ObjLst[eID].clear();
}
