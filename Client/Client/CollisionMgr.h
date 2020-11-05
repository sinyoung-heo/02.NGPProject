#pragma once
class CObj;


class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	static void CollisionPlayerSkill(OBJLIST& dstLst, OBJLIST& srcLst, CObj* pPlayer);
	static void CollisionMonsterSkill(OBJLIST& dstLst, OBJLIST& srcLst);

	static void CollisionPush(OBJLIST& TileLst, OBJLIST& srcLst);
	static void CollisionSphere(OBJLIST& dstLst, OBJLIST& srcLst);
	static bool CollisionTile(CObj* pTile, CObj* pPlayer);

private:
	static bool CheckSphere(CObj* pDst, CObj* pSrc);
	static bool CheckRect(CObj* pDst, CObj* pSrc, float* pMoveX, float* pMoveY);

	static bool MyIntersectRect(RECT* rt, const RECT* src1, const RECT* src2);

};

