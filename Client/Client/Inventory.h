#pragma once
#include "Obj.h"

#define INVEN_SIZE 12

class CInventory :
	public CObj
{
public:
	CInventory();
	virtual ~CInventory();

public:
	void SetbIsInvenOn(bool _bIsOn)			{ m_bIsInvenOn = _bIsOn; }
	void SetbIsSellLstOn(bool _bIsOn)		{ m_bIsSellLstOn = _bIsOn; }
	void SetIsEmpty(int _index, int _value) { m_iIsEmpty[_index] = _value; }
	void SetEquipment(CObj* _equip)			{ m_pEquipment = _equip; }
	void SetItemInfo(int _index, const INFO& _info, wstring _imagekey, ItemType::TYPE _type);

public:
	const int& GetMaxSize() const { return m_iMaxSize; }
	CObj* GetItemInfo(int _index) { return m_vecInven[_index]; }
	vector<CObj*>& GetInvenItemVec() { return m_vecInven; }

public:
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void BuyItem(CObj* pObj);
	void SellItem(int _index);
	void EraseItem(int _index);

private:
	void UpdateBlankRect();
	void UpdateTextRect();

private:
	/*Player 정보*/
	CObj* m_pPlayer;
	bool  m_bIsInvenOn;

	/*Equipment 정보*/
	CObj* m_pEquipment;

	/*아이템을 담을 vector*/
	vector<CObj*>	m_vecInven;	
	int				m_iMaxSize;
	int				m_iSelect;		//선택한 Item Index

	TEXTINFO  m_tBlankInfo[INVEN_SIZE];	//아이템 Box 위치정보
	RECT	  m_tBlank[INVEN_SIZE];		//아이템이 들어갈 정보 RECT
	int		  m_iIsEmpty[INVEN_SIZE];	//아이템이 들어갈 자리가 비어있는가?

	/*인벤토리 이동시 필요한 좌표.*/
	float m_fOldXpos;
	float m_fOldYpos;

	float m_fMoveXpos;
	float m_fMoveYpos;

	/*상점 창이 켜져있는가 확인*/
	bool  m_bIsSellLstOn;

	/*보유 골드를 출력해줄 텍스트 정보.*/
	TEXTINFO m_tSeedTextInfo;
	RECT	 m_tSeedTextRect;

};

