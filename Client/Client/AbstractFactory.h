#pragma once
class CObj;

#include "ComboCnt.h"
#include "DmgBox.h"

template<typename T>
class CAbstractFactory
{
public:
	static CObj* CreateObj()
	{
		CObj* pObj = new T;
		pObj->Initialize();

		return pObj;
	}

	static CObj* CreateObj(const INFO& rInfo)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetInfo(rInfo);

		return pObj;
	}

	static CObj* CreateColBox(float cx, float cy, float x, float y, int dmg)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetSize(cx, cy);
		pObj->SetPos(x, y);
		pObj->SetDmg(dmg);

		return pObj;
	}

	static CObj* CreateObj(float x, float y)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(x, y);

		return pObj;
	}

	static CObj* CreateObj(float x, float y, float angle)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(x, y);
		pObj->SetAngle(angle);

		return pObj;
	}

	static CObj* CreateDmgBox(float x, float y, int _dmg)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(x, y);
		static_cast<CDmgBox*>(pObj)->SetDmgInput(_dmg);

		return pObj;
	}

	static CObj* CreateComCnt(float x, float y, int _dmg)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(x, y);
		static_cast<CComboCnt*>(pObj)->SetDmgInput(_dmg);

		return pObj;
	}
};

