#pragma once
#include "Obj.h"

#define EQUIP_SIZE 4

class CEquipment :
	public CObj
{
public:
	CEquipment();
	virtual ~CEquipment();

public:
	void SetbIsEquipOn(bool _bIsOn) { m_bIsEquipOn = _bIsOn; }
	void SetInventory(CObj* _Inven) { m_pInventory = _Inven; }

public:
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	
public:
	void PutOnItem(int _index, ItemType::TYPE _eTpye);
	void PutOffItem(int _index);

private:
	void UpdateBlankRect();

private:
	/*아이템을 담을 vector*/
	vector<CObj*>	m_vecEquip;
	int				m_iMaxSize;
	int				m_iSelect;		//선택한 Item Index

	TEXTINFO		m_tBlankInfo[EQUIP_SIZE];	//아이템 Box 위치정보
	RECT			m_tBlank[EQUIP_SIZE];		//아이템이 들어갈 정보 RECT
	int				m_iIsEmpty[EQUIP_SIZE];	//아이템이 들어갈 자리가 비어있는가?

	/*Player 정보*/
	CObj* m_pPlayer;
	bool  m_bIsEquipOn;

	/*Inventory 정보*/
	CObj* m_pInventory;


	/*인벤토리 이동시 필요한 좌표.*/
	float m_fOldXpos;
	float m_fOldYpos;

	float m_fMoveXpos;
	float m_fMoveYpos;

};

