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
	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	void UpdateBlankRect();

private:
	CObj*		m_pPlayer;			//Player ����.
	CObj*		m_pInventory;		//Inventory ����.

	bool		m_bIsSellLst;		//�Ǹ�â �Ѡ����� On/Off

	vector<CObj*>	m_vecItem;		//�Ǹ����� Item ���
	int				m_iSelect;		//������ Item Index

	TEXTINFO  m_tBlankInfo[ITEM_SIZE];	//������ Box ��ġ����
	RECT	  m_tBlank[ITEM_SIZE];		//�������� �� ���� RECT
	int		  m_iIsEmpty[ITEM_SIZE];	//�������� �� �ڸ��� ����ִ°�?

};

