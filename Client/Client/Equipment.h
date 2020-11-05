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
	// CObj��(��) ���� ��ӵ�
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
	/*�������� ���� vector*/
	vector<CObj*>	m_vecEquip;
	int				m_iMaxSize;
	int				m_iSelect;		//������ Item Index

	TEXTINFO		m_tBlankInfo[EQUIP_SIZE];	//������ Box ��ġ����
	RECT			m_tBlank[EQUIP_SIZE];		//�������� �� ���� RECT
	int				m_iIsEmpty[EQUIP_SIZE];	//�������� �� �ڸ��� ����ִ°�?

	/*Player ����*/
	CObj* m_pPlayer;
	bool  m_bIsEquipOn;

	/*Inventory ����*/
	CObj* m_pInventory;


	/*�κ��丮 �̵��� �ʿ��� ��ǥ.*/
	float m_fOldXpos;
	float m_fOldYpos;

	float m_fMoveXpos;
	float m_fMoveYpos;

};

