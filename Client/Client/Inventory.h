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
	// CObj��(��) ���� ��ӵ�
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
	/*Player ����*/
	CObj* m_pPlayer;
	bool  m_bIsInvenOn;

	/*Equipment ����*/
	CObj* m_pEquipment;

	/*�������� ���� vector*/
	vector<CObj*>	m_vecInven;	
	int				m_iMaxSize;
	int				m_iSelect;		//������ Item Index

	TEXTINFO  m_tBlankInfo[INVEN_SIZE];	//������ Box ��ġ����
	RECT	  m_tBlank[INVEN_SIZE];		//�������� �� ���� RECT
	int		  m_iIsEmpty[INVEN_SIZE];	//�������� �� �ڸ��� ����ִ°�?

	/*�κ��丮 �̵��� �ʿ��� ��ǥ.*/
	float m_fOldXpos;
	float m_fOldYpos;

	float m_fMoveXpos;
	float m_fMoveYpos;

	/*���� â�� �����ִ°� Ȯ��*/
	bool  m_bIsSellLstOn;

	/*���� ��带 ������� �ؽ�Ʈ ����.*/
	TEXTINFO m_tSeedTextInfo;
	RECT	 m_tSeedTextRect;

};

