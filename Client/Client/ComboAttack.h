#pragma once
#include "Obj.h"
class CComboAttack :
	public CObj
{
public:
	CComboAttack();
	virtual ~CComboAttack();

public:
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void UpdateComboCnt(int _cnt) { m_iComboCnt = _cnt; }
	void SetbIsCombo(bool _IsCombo) { m_bIsCombo = _IsCombo; }
	
private:
	void UpdateRectCnt();

private:
	int  m_iCntArr[2];
	int  m_iArrSize;

	int  m_iInput;
	int  m_iComboCnt;

	bool m_bIsCombo;

	TEXTINFO m_tInfoRectCnt;
	RECT	 m_tRectCnt;

};

