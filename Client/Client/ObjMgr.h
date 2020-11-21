#pragma once
#include "Obj.h"
class CObjMgr
{
	DECLARE_SINGLETON(CObjMgr)

private:
	CObjMgr();
	~CObjMgr();

public:
	CObj* GetPlayer()		{ return m_ObjLst[ObjID::PLAYER].front(); }
	CObj* GetSkillIconUi()	{ return m_ObjLst[ObjID::UI].front(); }
	CObj* GetUiBack()		{ return m_ObjLst[ObjID::UI].back(); }
	CObj* GetSellLst()		{ return m_ObjLst[ObjID::SELL_LST].front(); }
	CObj* GetInventory()	{ return m_ObjLst[ObjID::INVENTORY].front(); }
	CObj* GetEquipment()	{ return m_ObjLst[ObjID::EQUIPMENT].front(); }
	CObj* GetUiButton()		{ return m_ObjLst[ObjID::UI_BUTTON].back(); }
	CObj* GetComboBox()		{ return m_ObjLst[ObjID::COMBO_BOX].back(); }

	OBJLIST& GetPlayerLst() { return m_ObjLst[ObjID::PLAYER];}
	OBJLIST& GetMonLst()	{ return m_ObjLst[ObjID::MONSTER]; }
	OBJLIST& GetObjList(const ObjID::ID& eID) { return m_ObjLst[eID]; }

public:
	void SetPlayerPos(float _x, float _y) { m_ObjLst[ObjID::PLAYER].front()->SetPos(_x, _y); }

public:
	void AddObject(CObj* pObj, ObjID::ID eID);
	void Update();
	void LateUpdate();
	void Render(HDC hDC);
	void Release();

public:
	void RenderlistClear();
public:
	void DeleteObj(ObjID::ID eID);

private:
	OBJLIST m_ObjLst[ObjID::END];
	OBJLIST m_RenderLst[Layer::END];
};

