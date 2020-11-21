#include "stdafx.h"
#include "CollisionMgr.h"
#include "Obj.h"
#include "Tile.h"
#include "Jelly.h"
#include "Player.h"
#include "PlayerCrash.h"
#include "MonCrash.h"
#include "DmgBox.h"


CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

/*Player 스킬 & 몬스터 충돌시*/
void CCollisionMgr::CollisionPlayerSkill(OBJLIST & dstLst, OBJLIST & srcLst, CObj* pPlayer)
{
	for (CObj*& pDst : dstLst)
	{
		for (CObj*& pSrc : srcLst)
		{
			RECT rt = {};

			const RECT dstRect = pDst->GetColRect();
			const RECT srcRect = pSrc->GetRect();

			if (IntersectRect(&rt, &dstRect, &srcRect))
			{
				CObjMgr::GetInstance()->AddObject(CAbstractFactory<CPlayerCrash>::CreateObj(pDst->GetInfo().fX, pDst->GetInfo().fY),
					ObjID::EFFECT);

				static_cast<CJelly*>(pDst)->SetStanceHIT();	//몬스터 피격상태 
				static_cast<CJelly*>(pDst)->SetStateAngry();	//몬스터 state 변화

				/*몬스터 Hp 감소*/
				pDst->SetHp(pSrc->GetInfo().iAtt);

				/*DmgBox*/
				CObjMgr::GetInstance()->AddObject(CAbstractFactory<CDmgBox>::CreateDmgBox(pDst->GetInfo().fX - 20, pDst->GetInfo().fY - 80.f, pSrc->GetInfo().iAtt),
					ObjID::EFFECT);

				/*몬스터 피 <= 0 사망*/
				if (pDst->GetInfo().iHp <= 0)
					pDst->bIsDead();

				/*충돌 후 스킬 삭제.*/
				pSrc->bIsDead();
			}
		}
	}
}

/*Monster 공격 & Player 충돌*/
void CCollisionMgr::CollisionMonsterSkill(OBJLIST & dstLst, OBJLIST & srcLst)
{
	for (CObj*& pDst : dstLst)
	{
		for (CObj*& pSrc : srcLst)
		{
			RECT rt = {};

			const RECT dstRect = pDst->GetColRect();
			const RECT srcRect = pSrc->GetRect();

			if (IntersectRect(&rt, &dstRect, &srcRect))
			{
				CObjMgr::GetInstance()->AddObject(CAbstractFactory<CMonCrash>::CreateObj(pDst->GetInfo().fX, pDst->GetInfo().fY),
					ObjID::EFFECT);

				static_cast<CPlayer*>(pDst)->SetStanceHIT();	//Player 피격상태 

				pDst->SetHp(pSrc->GetInfo().iAtt);

				CObjMgr::GetInstance()->AddObject(CAbstractFactory<CDmgBox>::CreateDmgBox(pDst->GetInfo().fX - 20, pDst->GetInfo().fY - 80.f, pSrc->GetInfo().iAtt),
					ObjID::EFFECT);

				if (pDst->GetInfo().iHp <= 0)
					pDst->bIsDead();


				pSrc->bIsDead();
			}
		}
	}
}


/*충돌 밀어내기*/
void CCollisionMgr::CollisionPush(OBJLIST & TileLst, OBJLIST & srcLst)
{
	OBJITER tile_begin = TileLst.begin();

	for (; tile_begin != TileLst.end(); ++tile_begin)
	{
		OBJITER src_begin = srcLst.begin();

		for (; src_begin != srcLst.end(); ++src_begin)
		{
			RECT rt = {};

			const RECT TileRect = (*tile_begin)->GetColRect();
			const RECT SrcRect = (*src_begin)->GetColRect();

			if (MyIntersectRect(&rt, &TileRect, &SrcRect))
			{
				float w = static_cast<float>(rt.right - rt.left);
				float h = static_cast<float>(rt.bottom - rt.top);

				if (w > h)	//위, 아래에서 충돌.
				{
					if (rt.top == TileRect.top)	//위에서 충돌 했다면
					{
						(*src_begin)->AddSetYpos(-h);
						(*src_begin)->AddSetColInfoY(-h);

					}
					else if (rt.bottom == TileRect.bottom)	//아래에서 충돌했다면
					{
						(*src_begin)->AddSetYpos(h);
						(*src_begin)->AddSetColInfoY(h);
					}

				}
				else        //좌, 우에서 충돌.
				{
					if (rt.left == TileRect.left)			//좌에서 충돌 했다면
					{
						(*src_begin)->AddSetXpos(-w);
						(*src_begin)->AddSetColInfoX(-w);
						
					}
					else if (rt.right == TileRect.right)	//우에서 충돌 했다면
					{
						(*src_begin)->AddSetXpos(w);
						(*src_begin)->AddSetColInfoX(w);
					}
				}
			}
		}
	}
}

void CCollisionMgr::CollisionSphere(OBJLIST & dstLst, OBJLIST & srcLst)
{
	for (CObj*& pDst : dstLst)
	{
		for (CObj*& pSrc : srcLst)
		{
			if (CheckSphere(pDst,pSrc))
			{
				pDst->bIsDead();
				pSrc->bIsDead();
			}
		}
	}
}

/*타일 DrawID 별 분기해서 충돌 체크*/
bool CCollisionMgr::CollisionTile(CObj * pTile, CObj * pPlayer)
{
	if (pPlayer == nullptr)
		return false;

	RECT rt = {};

	const RECT TileRect = pTile->GetRect();
	const RECT PlayerRect = pPlayer->GetColRect();

	int iDrawID = static_cast<CTile*>(pTile)->GetDrawID();

	float w;
	float h;

	
	if (IntersectRect(&rt, &TileRect, &PlayerRect))
	{
		switch (iDrawID)
		{
		case 1:
			w = (float)(rt.right - rt.left);
			h = (float)(rt.bottom - rt.top);
			if (w > h)	//위, 아래 충돌
			{
				if (rt.top == TileRect.top)	//위에서 충돌 했다면
				{	//충돌박스 & 이미지 박스 같이 연산.
					pPlayer->AddSetColInfoY(-h);
					pPlayer->AddSetYpos(-h);
					return true;
				}
				else if (rt.bottom == TileRect.bottom)	//아래에서 충돌 했다면
				{
					pPlayer->AddSetColInfoY(h);
					pPlayer->AddSetYpos(h);
					return true;
				}
			}
			else
			{	//좌, 우 충돌
				if (rt.left == TileRect.left)	//좌에서 충돌했다면
				{
					pPlayer->AddSetColInfoX(-w);
					pPlayer->AddSetXpos(-w);
					return true;
				}
				else
				{								//우에서 충돌
					pPlayer->AddSetColInfoX(w);
					pPlayer->AddSetXpos(w);
					return true;
				}
			}
			break;
		case 2:
			CSceneMgr::GetInstance()->SceneChange(CSceneMgr::SC_TOWN);
			CObjMgr::GetInstance()->GetPlayer()->SetCurScene(CSceneMgr::SC_TOWN);
			CObjMgr::GetInstance()->DeleteObj(ObjID::MONSTER_COW);
			CObjMgr::GetInstance()->DeleteObj(ObjID::MONSTER_NINJA);
			CObjMgr::GetInstance()->DeleteObj(ObjID::MONSTER_BORIS);

			CPacketMgr::GetInstance()->SendPlayerSceneID(SCENEID_TOWN);
			
			return true;

			break;

		case 3:
			CSceneMgr::GetInstance()->SceneChange(CSceneMgr::SC_STORE);
			CObjMgr::GetInstance()->GetPlayer()->SetCurScene(CSceneMgr::SC_STORE);

			CPacketMgr::GetInstance()->SendPlayerSceneID(SCENEID_STORE);
			return true;

			break;

		case 4:
			CSceneMgr::GetInstance()->SceneChange(CSceneMgr::SC_FIELD);
			CObjMgr::GetInstance()->GetPlayer()->SetCurScene(CSceneMgr::SC_FIELD);
			CPacketMgr::GetInstance()->SendPlayerSceneID(SCENEID_FIELD);
			return true;

			break;

		case 5:
			CSceneMgr::GetInstance()->SceneChange(CSceneMgr::SC_DUNGEON);
			CObjMgr::GetInstance()->GetPlayer()->SetCurScene(CSceneMgr::SC_DUNGEON);
			CObjMgr::GetInstance()->DeleteObj(ObjID::MONSTER_BORIS);


			CPacketMgr::GetInstance()->SendPlayerSceneID(SCENEID_DUNGEON);
			return true;

			break;

		case 6:
			CSceneMgr::GetInstance()->SceneChange(CSceneMgr::SC_BOSS);
			CObjMgr::GetInstance()->GetPlayer()->SetCurScene(CSceneMgr::SC_BOSS);
			CObjMgr::GetInstance()->DeleteObj(ObjID::MONSTER_COW);
			CObjMgr::GetInstance()->DeleteObj(ObjID::MONSTER_NINJA);

			CPacketMgr::GetInstance()->SendPlayerSceneID(SCENEID_BOSS);
			return true;

			break;

		}
	}

	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	pt.x -= (LONG)CScrollMgr::GetScrollX();
	pt.y -= (LONG)CScrollMgr::GetScrollY();

	if (PtInRect(&TileRect, pt))
	{
		if (iDrawID == 1)
		{
			CMouseMgr::GetInstance()->Setm_pFrameKey(L"Mouse_Fail");
		}
		else
		{
			CMouseMgr::GetInstance()->Setm_pFrameKey(L"Mouse");
		}
	}

	return true;
}


bool CCollisionMgr::CheckSphere(CObj * pDst, CObj * pSrc)
{
	float fSumRad = (pDst->GetInfo().fCX + pSrc->GetInfo().fCX) * 0.5f;

	float w = pDst->GetInfo().fX - pSrc->GetInfo().fX;
	float h = pDst->GetInfo().fY - pSrc->GetInfo().fY;

	float d = sqrtf(w*w + h*h);


	return (fSumRad >= d);
}

bool CCollisionMgr::CheckRect(CObj * pDst, CObj * pSrc, float * pMoveX, float * pMoveY)
{
	// 두 사각형의 가로, 세로 축 반지름의 합을 구한다.
	float fSumRadX = (pDst->GetInfo().fCX + pSrc->GetInfo().fCX) * 0.5f;
	float fSumRadY = (pDst->GetInfo().fCY + pSrc->GetInfo().fCY) * 0.5f;

	// 두 사각형의 가로, 세로 중점의 거리를 구한다.
	// fabs(X): X의 절대 값을 구하는 함수. <cmath>에서 제공. 
	float fDistX = fabs(pDst->GetInfo().fX - pSrc->GetInfo().fX);
	float fDistY = fabs(pDst->GetInfo().fY - pSrc->GetInfo().fY);

	// 가로 축 반지름 합이 가로 축 거리보다 클 때와
	// 세로 축 반지름 합이 세로 축 거리보다 클 때
	if (fSumRadX > fDistX && fSumRadY > fDistY)
	{
		// 두 사각형이 겹쳤을 때 파고든 길이도 구한다.
		*pMoveX = fSumRadX - fDistX;
		*pMoveY = fSumRadY - fDistY;

		return true;
	}

	return false;
}

bool CCollisionMgr::MyIntersectRect(RECT * rt, const RECT * src1, const RECT * src2)
{
	bool bIsHorizon = false;	//수평충돌
	bool bIsVertical = false;	//수직충돌

	if (src1->left < src2->right && src1->right > src2->left)	//수평충돌
	{
		bIsHorizon = true;

		if (rt)
		{
			if (src1->left < src2->left)
				rt->left = src2->left;
			else
				rt->left = src1->left;

			if (src1->right < src2->right)
				rt->right = src1->right;
			else
				rt->right = src2->right;
		}
	}

	if (src1->bottom > src2->top && src1->top < src2->bottom)
	{
		bIsVertical = true;

		if (rt)
		{
			rt->top = max(src1->top, src2->top);
			rt->bottom = min(src1->bottom, src2->bottom);
		}
	}


	if (bIsHorizon && bIsVertical)
		return true;


	if (rt)
	{
		rt->left = 0;
		rt->top = 0;
		rt->right = 0;
		rt->bottom = 0;
	}

	return false;
}