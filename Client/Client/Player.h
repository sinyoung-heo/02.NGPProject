#pragma once
#include "Obj.h"
class CPlayer :
	public CObj
{
public:
	enum STANCE			{ IDLE, WALK, RUN, ATTACK, SKILL, HIT, END };
	enum SKILL_STATE	{ SOUL, MOON, MULTI, BASIC, SKILL_END };

public:
	CPlayer();
	virtual ~CPlayer();

	// CObj��(��) ���� ��ӵ�
	virtual void	Initialize() override;
	virtual void	LateInit();
	virtual int		Update() override;
	virtual void	LateUpdate() override;
	virtual void	Render(HDC hDC) override;
	virtual void	Release() override;

public:
	void SetStanceHIT() { m_eCurStance = HIT, m_bIsHit = true; }

public:
	bool GetIsCreate() { return m_bIsCreate; }
	const PLAYERINFO& GetAbility() const { return m_tAbility; }
	const int&  GetComboCnt() const { return m_iComboCnt; }
	const bool& GetbIsCombo() const { return m_bIsCombo;  }

private:
	// 11.06 KeyInput �߰�.
	void KeyInput();
	void SceneChange();
	void IsOffSet();
	void IsLevelUp();

private:
	template<typename T>
	CObj* CreateSkill()
	{
		return CAbstractFactory<T>::CreateObj(m_fAttackX, m_fAttackY, m_fAngle);
	}

private:
	/*�ִϸ��̼� �������*/
	wstring m_pFrameKey;
	STANCE m_eCurStance;
	STANCE m_ePreStance;

	/*�ȱ� & �޸��� ����*/
	int		m_iMoveState;

	bool	m_bIsCreate;

	bool	m_bIsRunning;
	bool	m_bIsAttack;
	bool	m_bIsSkill;
	bool	m_bIsHit;

	float	fAttLen;

	/*��ų ������ & ��ų����*/
	CObj*		m_pSkillIcon;
	SKILL_STATE m_eSkill;

	/*Player Info UI*/
	CObj*		m_pPlayerInfo;
	bool		m_bIsInfoUi;
	PLAYERINFO	m_tAbility;

	/*Player Inventory UI*/
	CObj*		m_pInventory;
	bool		m_bIsInvenUi;

	/*Player Equipment UI*/
	CObj*		m_pEquipment;
	bool		m_bIsEquipUi;

	/*Ui Button*/
	CObj*		m_pStateButton;
	CObj*		m_pEquipButton;
	CObj*		m_pInvenButton;

	/*Combo Attack*/
	CObj*	m_pComboBox;		//�޺� �ڽ�.
	int		m_iComboCnt;		//�޺� ī��Ʈ
	bool	m_bIsCombo;			//�޺� �����ΰ�
};

