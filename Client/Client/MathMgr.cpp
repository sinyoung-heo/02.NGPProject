#include "stdafx.h"
#include "MathMgr.h"
#include "Obj.h"


CMathMgr::CMathMgr()
{
}


CMathMgr::~CMathMgr()
{
}

float CMathMgr::CalcDistance(CObj * pDst, CObj * pSrc)
{
	float w = pDst->GetInfo().fX - pSrc->GetInfo().fX;
	float h = pDst->GetInfo().fY - pSrc->GetInfo().fY;

	return sqrtf(w * w + h * h);
}

float CMathMgr::CalcRadian(CObj * pDst, CObj * pSrc)
{
	float w = pDst->GetInfo().fX - pSrc->GetInfo().fX;
	float h = pDst->GetInfo().fY - pSrc->GetInfo().fY;
	float d = sqrtf(w * w + h * h);

	float fAngle = acosf(w / d);

	if (pSrc->GetInfo().fY < pDst->GetInfo().fY)
		fAngle *= -1.f;

	return fAngle;
}

float CMathMgr::CalcDegree(CObj * pDst, CObj * pSrc)
{
	float w = pDst->GetInfo().fX - pSrc->GetInfo().fX;
	float h = pDst->GetInfo().fY - pSrc->GetInfo().fY;
	float d = sqrtf(w * w + h * h);

	float fAngle = acosf(w / d);

	if (pSrc->GetInfo().fY < pDst->GetInfo().fY)
		fAngle *= -1.f;

	return fAngle * 180.f / PI;

}
