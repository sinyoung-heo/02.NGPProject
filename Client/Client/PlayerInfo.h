#pragma once
#include "Obj.h"
class CPlayerInfo :
	public CObj
{
public:
	CPlayerInfo();
	virtual ~CPlayerInfo();

public:
	void SetbIsInfoOn(bool _bIsOn) { m_bIsInfoOn = _bIsOn; }

public:
	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	void UpdateTextRect();

private:
	CObj*		m_pPlayer;
	PLAYERINFO  m_tAbility;
	bool  m_bIsInfoOn;

	/*���� �̵��� �ʿ��� ��ǥ.*/
	float m_fOldXpos;
	float m_fOldYpos;

	float m_fMoveXpos;
	float m_fMoveYpos;

	/*DrawText�� ���� �簢��*/
	TEXTINFO m_Text1;
	RECT	 m_TextRect1;

	TEXTINFO m_Text2;
	RECT	 m_TextRect2;

	TEXTINFO m_Text3;
	RECT	 m_TextRect3;
	
	TEXTINFO m_Text4;
	RECT	 m_TextRect4;
};


