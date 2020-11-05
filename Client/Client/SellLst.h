#pragma once
#include "Obj.h"

#define ITEM_SIZE 8

class CSellLst :
	public CObj
{
public:
	CSellLst();
	virtual ~CSellLst();

public:
	void SetbIsSellLst(bool _IsOn) { m_bIsSellLst = _IsOn; }

public:
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	void UpdateBlankRect();

private:
	CObj*		m_pPlayer;			//Player 정보.
	CObj*		m_pInventory;		//Inventory 정보.

	bool		m_bIsSellLst;		//판매창 켜젔는지 On/Off

	vector<CObj*>	m_vecItem;		//판매중인 Item 목록
	int				m_iSelect;		//선택한 Item Index

	TEXTINFO  m_tBlankInfo[ITEM_SIZE];	//아이템 Box 위치정보
	RECT	  m_tBlank[ITEM_SIZE];		//아이템이 들어갈 정보 RECT
	int		  m_iIsEmpty[ITEM_SIZE];	//아이템이 들어갈 자리가 비어있는가?

};

