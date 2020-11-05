#pragma once
#include "Obj.h"
class CItem :
	public CObj
{
public:
	CItem();
	CItem(const INFO& rInfo);
	virtual ~CItem();

	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void SetImageKey(wstring pImageKey)  { m_pImageKey = pImageKey; }
	void SetItemType(ItemType::TYPE eItemType)	{ m_eItemType = eItemType; }

public:
	wstring GetImageKey() { return m_pImageKey; }
	ItemType::TYPE& GetItemType()	  { return m_eItemType; }

protected:
	wstring m_pImageKey;		//������ �̹��� Key��.
	ItemType::TYPE m_eItemType;		//������ Type.

};

