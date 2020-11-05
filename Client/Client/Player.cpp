#include "stdafx.h"
#include "Player.h"
#include "Skill.h"
#include "SkillSoul.h"
#include "MultiAttack.h"
#include "SkillMoon.h"
#include "LevelUp.h"
#include "LevelUpEffect.h"
#include "SkillIcon.h"
#include "HpPotionEffect.h"
#include "MpPotionEffect.h"
#include "PlayerInfo.h"
#include "Inventory.h"
#include "Equipment.h"
#include "StateButton.h"
#include "EquipButton.h"
#include "InvenButton.h"
#include "Tile.h"
#include "ComboAttack.h"
#include "ComboCnt.h"

#define HP_MP_SP_LEN 84.f
#define COMBO_TIME   180

static int iHitTime = 0;
static int iComboAttackTime = 0;

CPlayer::CPlayer()
	:m_bIsCreate(false), m_pPlayerInfo(nullptr), m_pSkillIcon(nullptr),
	m_bIsInfoUi(false), m_bIsInvenUi(false), m_bIsEquipUi(false), m_bIsCombo(false), m_pComboBox(nullptr)
{
	ZeroMemory(&m_tAbility, sizeof(PLAYERINFO));
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	/*Player ��Ʈ��*/
	CBmpMgr::GetInstance()->InsertBmp(L"Player_Down_", L"../Image/Player/Player_Down_.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Player_Left_", L"../Image/Player/Player_Left_.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Player_LeftDown_", L"../Image/Player/Player_LeftDown_.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Player_LeftUp_", L"../Image/Player/Player_LeftUp_.bmp");

	CBmpMgr::GetInstance()->InsertBmp(L"Player_Right_", L"../Image/Player/Player_Right_.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Player_RightDown_", L"../Image/Player/Player_RightDown_.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Player_RightUp_", L"../Image/Player/Player_RightUp_.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Player_Up_", L"../Image/Player/Player_Up_.bmp");
	
	/*Player Info UI*/
	CBmpMgr::GetInstance()->InsertBmp(L"Player_Info", L"../Image/Ui/Player_Info.bmp");
	/*Player Slot UI*/
	CBmpMgr::GetInstance()->InsertBmp(L"Player_Slot", L"../Image/Ui/Player_Slot.bmp");
	/*Player Hp,Mp,Sp bar*/
	CBmpMgr::GetInstance()->InsertBmp(L"Player_Hp", L"../Image/Ui/Player_Hp.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Player_Mp", L"../Image/Ui/Player_Mp.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Player_Sp", L"../Image/Ui/Player_Sp.bmp");
	/*Player Run & Walk Img*/
	CBmpMgr::GetInstance()->InsertBmp(L"Player_MoveState", L"../Image/Ui/Player_MoveState.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"Quick_Skill", L"../Image/Ui/Quick_Skill.bmp");
	/*Potion*/
	CBmpMgr::GetInstance()->InsertBmp(L"Potion", L"../Image/Ui/Potion.bmp");
	/*Potion Effect*/
	CBmpMgr::GetInstance()->InsertBmp(L"HpPotion", L"../Image/Effect/HpPotion.bmp");
	CBmpMgr::GetInstance()->InsertBmp(L"MpPotion", L"../Image/Effect/MpPotion.bmp");


	/*��ų ������ PushBack*/
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CSkillIcon>::CreateObj(64.f, 81.f), ObjID::UI);
	/*��ų ���� & ��ų ������ ��ü*/
	m_pSkillIcon = CObjMgr::GetInstance()->GetSkillIconUi();
	m_eSkill = MOON;

	///*Player Info PushBack*/
	//CObjMgr::GetInstance()->AddObject(CAbstractFactory<CPlayerInfo>::CreateObj(450.f, 10.f), ObjID::UI);



	/*�ִϸ��̼� ���� IDLE ���·� �ʱ�ȭ.*/
	m_pFrameKey = L"Player_Down_";
	m_eCurStance = IDLE;
	m_ePreStance = m_eCurStance;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iScene = 5;
	m_tFrame.dwOldTime = GetTickCount();
	m_tFrame.dwFrameSpd = 200;

	m_iMoveState	= 0;
	m_bIsRunning	= true;
	m_bIsAttack		= false;
	m_bIsSkill		= false;
	m_bIsHit		= false;

	/*���� �� ���� �����ϱ� ���� ����*/
	m_bIsCreate = true;

	/*Player Info*/
	m_tInfo.fCX = 256.f;
	m_tInfo.fCY = 256.f;
	m_tInfo.szName = L"Sin_Young";
	m_tInfo.iLevel = 1;
	m_tInfo.iMaxHp = 7000;
	m_tInfo.iHp = m_tInfo.iMaxHp;
	m_tInfo.iMaxMp = 3000;
	m_tInfo.iMp = m_tInfo.iMaxMp;
	m_tInfo.iMaxSp = 5000;
	m_tInfo.iSp = m_tInfo.iMaxSp;
	m_tInfo.iAtt = 123;
	m_tInfo.iExp = 0;
	m_tInfo.iMaxExp = 200;
	m_tInfo.iGold = 1000000;

	m_tAbility.iStap	= 2;
	m_tAbility.iHack	= 5;
	m_tAbility.iInt		= 1;
	m_tAbility.iDef		= 3;
	m_tAbility.iMr		= 1;
	m_tAbility.iDex		= 4;
	m_tAbility.iAgl		= 3;
		
	m_iComboCnt = 0;

	/*�浹�ڽ� ��ǥ = Player�� ��ǥ*/
	CObj::UpdateColXYPos();
	/*�浹�ڽ� ũ��*/
	m_tColInfo.fCX = 30.f;
	m_tColInfo.fCY = 30.f;

	/*�̵��ӵ�*/
	m_fSpeed = 2.5f;

	/*��ų&���� ����*/
	fAttLen = 60.f;
	m_fAngle = 270.f;

	m_fAttackX = m_tColInfo.fX + cosf(m_fAngle * PI / 180.f) * fAttLen;
	m_fAttackY = m_tColInfo.fY + sinf(m_fAngle * PI / 180.f) * fAttLen;
}

void CPlayer::LateInit()
{
	/*Player Info & Equipment & Inventory PushBack*/
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CPlayerInfo>::CreateObj(625.f, 160.f), ObjID::UI);
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CInventory>::CreateObj(625.f, 300.f), ObjID::INVENTORY);
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CEquipment>::CreateObj(130.f, 250.f), ObjID::EQUIPMENT);

	m_pPlayerInfo	 = CObjMgr::GetInstance()->GetUiBack();
	m_pInventory	 = CObjMgr::GetInstance()->GetInventory();
	m_pEquipment	 = CObjMgr::GetInstance()->GetEquipment();

	/*�κ��丮�� ���â ����*/
	dynamic_cast<CInventory*>(m_pInventory)->SetEquipment(CObjMgr::GetInstance()->GetEquipment());
	/*���â�� �κ��丮 ����*/
	dynamic_cast<CEquipment*>(m_pEquipment)->SetInventory(CObjMgr::GetInstance()->GetInventory());

	/*Ui Button ����*/
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CStateButton>::CreateObj(790.f, 30.f), ObjID::UI_BUTTON);
	m_pStateButton = CObjMgr::GetInstance()->GetUiButton();
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CEquipButton>::CreateObj(790.f, 81.f), ObjID::UI_BUTTON);
	m_pEquipButton = CObjMgr::GetInstance()->GetUiButton();
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CInvenButton>::CreateObj(790.f, 131.f), ObjID::UI_BUTTON);
	m_pInvenButton = CObjMgr::GetInstance()->GetUiButton();

	/*ComboBox ����*/
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CComboAttack>::CreateObj(1200.f, 1200.f), ObjID::COMBO_BOX);
	m_pComboBox = CObjMgr::GetInstance()->GetComboBox();
}

int CPlayer::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;

	if (KEY_DOWN('L'))
		m_tInfo.iExp += 1000;
	if (KEY_DOWN('Q'))
	{
		m_tInfo.iHp = 1;
		m_tInfo.iMp = 1;
		m_tInfo.iSp = 1;
	}
	
	/*�޸���, �ȱ� ��ȯ*/
	if (KEY_DOWN('R'))
	{
		m_bIsRunning = !m_bIsRunning;
		m_iMoveState = (m_iMoveState + 1) % 2;
	}
	if (m_bIsRunning == false)
	{
		++m_tInfo.iSp;

		if (m_tInfo.iSp > m_tInfo.iMaxSp)
			m_tInfo.iSp = m_tInfo.iMaxSp;
	}
	if (m_tInfo.iSp <= 0)
	{
		m_tInfo.iSp = 0;
		m_bIsRunning = false;
		m_iMoveState = (m_iMoveState + 1) % 2;
	}

	/*�÷��̾� �̵� RUN*/
	if (KEY_PRESSING('A'))
	{
		/*���� ���� ����*/
		m_fAngle = 180.f;
		/*�̹����ڽ� & �浹�ڽ� ���� �̵�.*/
		if (KEY_PRESSING('W'))
		{	/*���� ���� ����*/
			m_fAngle = 240.f;
			if (m_bIsRunning)
			{	//�޸���
				m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
				m_tInfo.fY -= m_fSpeed / sqrtf(2.f);
				CObj::UpdateColXYPos();

				m_pFrameKey = L"Player_LeftUp_";
				m_eCurStance = RUN;

				--m_tInfo.iSp;
			}
			else
			{	//�ȱ�
				m_tInfo.fX -= m_fSpeed / sqrtf(2.f) / 2;
				m_tInfo.fY -= m_fSpeed / sqrtf(2.f) / 2;
				CObj::UpdateColXYPos();

				m_pFrameKey = L"Player_LeftUp_";
				m_eCurStance = WALK;
			}
		}
		else if (KEY_PRESSING('D'))
		{
			/*���� ���� ����*/
			m_fAngle = 125.f;
			if (m_bIsRunning)
			{	//�޸���
				m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
				m_tInfo.fY += m_fSpeed / sqrtf(2.f);
				CObj::UpdateColXYPos();

				m_pFrameKey = L"Player_LeftDown_";
				m_eCurStance = RUN;

				--m_tInfo.iSp;
			}
			else
			{	//�ȱ�
				m_tInfo.fX -= m_fSpeed / sqrtf(2.f) / 2;
				m_tInfo.fY += m_fSpeed / sqrtf(2.f) / 2;
				CObj::UpdateColXYPos();

				m_pFrameKey = L"Player_LeftDown_";
				m_eCurStance = WALK;
			}
		}
		else
		{/*�� �̵�*/
			if (m_bIsRunning)
			{	//�޸���
				m_tInfo.fX -= m_fSpeed;
				CObj::UpdateColXYPos();

				m_pFrameKey = L"Player_Left_";
				m_eCurStance = RUN;

				--m_tInfo.iSp;
			}
			else
			{	//�ȱ�
				m_tInfo.fX -= m_fSpeed / 2;
				CObj::UpdateColXYPos();

				m_pFrameKey = L"Player_Left_";
				m_eCurStance = WALK;
			}
		}
	}
	else if (KEY_PRESSING('D'))
	{
		/*���� ���� ����*/
		m_fAngle = 360.f;
		if (KEY_PRESSING('W'))		/*����*/
		{	
			/*���� ���� ����*/
			m_fAngle = 300.f;

			if (m_bIsRunning)			//�޸���
			{	/*�̹��� �ڽ�*/
				m_tInfo.fX += m_fSpeed / sqrtf(2.f);
				m_tInfo.fY -= m_fSpeed / sqrtf(2.f);
				/*�浹 �ڽ�*/
				CObj::UpdateColXYPos();

				m_pFrameKey = L"Player_RightUp_";
				m_eCurStance = RUN;

				--m_tInfo.iSp;
			}
			else
			{							//�ȱ�
				/*�̹��� �ڽ�*/
				m_tInfo.fX += m_fSpeed / sqrtf(2.f) / 2;
				m_tInfo.fY -= m_fSpeed / sqrtf(2.f) / 2;
				/*�浹 �ڽ�*/
				CObj::UpdateColXYPos();

				m_pFrameKey = L"Player_RightUp_";
				m_eCurStance = WALK;
			}
		}
		else if (KEY_PRESSING('S')) /*���ϴ�*/
		{
			/*���� ���� ����*/
			m_fAngle = 55.f;

			if (m_bIsRunning)
			{	//�޸���
				m_tInfo.fX += m_fSpeed / sqrtf(2.f);
				m_tInfo.fY += m_fSpeed / sqrtf(2.f);
				CObj::UpdateColXYPos();

				m_pFrameKey = L"Player_RightDown_";
				m_eCurStance = RUN;

				--m_tInfo.iSp;
			}
			else
			{	//�ȱ�
				m_tInfo.fX += m_fSpeed / sqrtf(2.f) / 2;
				m_tInfo.fY += m_fSpeed / sqrtf(2.f) / 2;
				CObj::UpdateColXYPos();

				m_pFrameKey = L"Player_RightDown_";
				m_eCurStance = WALK;
			}
		}
		else
		{	//�޸���
			if (m_bIsRunning)
			{	/*�� �̵�*/
				m_tInfo.fX += m_fSpeed;
				CObj::UpdateColXYPos();

				m_pFrameKey = L"Player_Right_";
				m_eCurStance = RUN;

				--m_tInfo.iSp;
			}
			else
			{//�ȱ�
				m_tInfo.fX += m_fSpeed / 2;
				CObj::UpdateColXYPos();

				m_pFrameKey = L"Player_Right_";
				m_eCurStance = WALK;
			}
		}
	}
	else if (KEY_PRESSING('W'))	/*��� �̵�*/
	{
		/*���� ���� ����*/
		m_fAngle = 270.f;
		if (m_bIsRunning)
		{	//�޸���
			m_tInfo.fY -= m_fSpeed;
			CObj::UpdateColXYPos();

			m_pFrameKey = L"Player_Up_";
			m_eCurStance = RUN;

			--m_tInfo.iSp;
		}
		else
		{	//�ȱ�
			m_tInfo.fY -= m_fSpeed / 2;
			CObj::UpdateColXYPos();

			m_pFrameKey = L"Player_Up_";
			m_eCurStance = WALK;
		}

	}
	else if (KEY_PRESSING('S'))	/*�ϴ� �̵�*/
	{
		/*���� ���� ����*/
		m_fAngle = 90.f;
		if (m_bIsRunning)
		{	//�޸���
			m_tInfo.fY += m_fSpeed;
			CObj::UpdateColXYPos();

			m_pFrameKey = L"Player_Down_";
			m_eCurStance = RUN;

			--m_tInfo.iSp;
		}
		else
		{	//�ȱ�
			m_tInfo.fY += m_fSpeed / 2;
			CObj::UpdateColXYPos();

			m_pFrameKey = L"Player_Down_";
			m_eCurStance = WALK;
		}
	}
	else if(KEY_UP('A'))
		m_eCurStance = IDLE;
	else if (KEY_UP('D'))
		m_eCurStance = IDLE;
	else if (KEY_UP('W'))
		m_eCurStance = IDLE;
	else if (KEY_UP('S'))
		m_eCurStance = IDLE;


	/*�⺻ ����*/
	else if (KEY_DOWN(VK_SPACE) /*|| KEY_DOWN(VK_LBUTTON)*/)
	{
		CSoundMgr::GetInstance()->PlaySound(L"pAttack.wav", CSoundMgr::PLAYER_ATTACK);


		m_bIsAttack = true;
		m_eCurStance = ATTACK;

		CObj::UpdateColXYPos();
		m_fAttackX = m_tColInfo.fX + cosf(m_fAngle * PI / 180.f) * fAttLen;
		m_fAttackY = m_tColInfo.fY + sinf(m_fAngle * PI / 180.f) * fAttLen;

		CObjMgr::GetInstance()->AddObject(CreateSkill<CSkill>(), ObjID::SKILL);
	}

	/*��ų ����*/
	else if (KEY_DOWN(VK_F1))
	{
		m_eSkill = MOON;
		dynamic_cast<CSkillIcon*>(m_pSkillIcon)->SetImageKey(0);
	}
	/*��ų ����*/
	else if (KEY_DOWN(VK_F2))
	{
		m_eSkill = SOUL;
		dynamic_cast<CSkillIcon*>(m_pSkillIcon)->SetImageKey(1);
	}
	/*��ų ����*/
	else if (KEY_DOWN(VK_F3))
	{
		m_eSkill = MULTI;
		dynamic_cast<CSkillIcon*>(m_pSkillIcon)->SetImageKey(4);
	}

	/*��ų ����*/
	else if (KEY_DOWN(VK_RBUTTON))
	{	

		if (m_iComboCnt > 0)
		{
			/*�޺� ī��Ʈ 0���� ū �����̸�*/
			if (iComboAttackTime < COMBO_TIME)
			{
				/*�޺����� ���ѽð��� ���� �ʾ�����.*/
				m_bIsCombo = true;
				/*�޺����ÿ� ���� ������ ���ѽð� �ʱ�ȭ.*/
				iComboAttackTime = 0;

				++m_iComboCnt;

				/*�޺��ڽ� ��ǥ & �޺�ī���� ������Ʈ*/
				dynamic_cast<CComboAttack*>(m_pComboBox)->UpdateComboCnt(m_iComboCnt);
				dynamic_cast<CComboAttack*>(m_pComboBox)->SetbIsCombo(m_bIsCombo);

				CObjMgr::GetInstance()->AddObject(CAbstractFactory<CComboCnt>::CreateComCnt(m_tInfo.fX - 30.f, m_tInfo.fY - 90.f, m_iComboCnt), ObjID::EFFECT);

			}
		}

		m_bIsSkill = true;
		m_eCurStance = SKILL;

		CObj::UpdateColXYPos();

		m_fAttackX = m_tColInfo.fX + cosf(m_fAngle * PI / 180.f) * fAttLen;
		m_fAttackY = m_tColInfo.fY + sinf(m_fAngle * PI / 180.f) * fAttLen;

		if (m_tInfo.iMp > 100)
		{
			CSoundMgr::GetInstance()->PlaySound(L"SKILL.wav", CSoundMgr::PLAYER_SKILL);

			switch (m_eSkill)
			{
			case SOUL:
				CObjMgr::GetInstance()->AddObject(CreateSkill<CSkillSoul>(), ObjID::SKILL);
				m_tInfo.iMp -= 100;
				break;
			case MOON:
				CObjMgr::GetInstance()->AddObject(CreateSkill<CSkillMoon>(), ObjID::SKILL);
				m_tInfo.iMp -= 100;
				break;
			case MULTI:
				CObjMgr::GetInstance()->AddObject(CreateSkill<CMultiAttack>(), ObjID::SKILL);
				m_tInfo.iMp -= 100;
				break;
			}
		}
		else
			CSoundMgr::GetInstance()->PlaySound(L"NoMana.wav", CSoundMgr::PLAYER_SKILL);


	}
	/*���� ���*/
	else if (KEY_DOWN(VK_F5))
	{
		CSoundMgr::GetInstance()->PlaySound(L"PotionUse.wav", CSoundMgr::USE_POTION);

		m_tInfo.iHp += (int)(m_tInfo.iMaxHp*0.1f);

		if (m_tInfo.iHp > m_tInfo.iMaxHp)
			m_tInfo.iHp = m_tInfo.iMaxHp;

		CObjMgr::GetInstance()->AddObject(CAbstractFactory<CHpPotionEffect>::CreateObj(m_tInfo.fX, m_tInfo.fY -1.f),
			ObjID::EFFECT);
	}
	else if (KEY_DOWN(VK_F6))
	{
		CSoundMgr::GetInstance()->PlaySound(L"PotionUse.wav", CSoundMgr::USE_POTION);

		m_tInfo.iSp += (int)(m_tInfo.iMaxSp*0.1f);

		if (m_tInfo.iSp > m_tInfo.iMaxSp)
			m_tInfo.iSp = m_tInfo.iMaxSp;

		CObjMgr::GetInstance()->AddObject(CAbstractFactory<CMpPotionEffect>::CreateObj(m_tInfo.fX, m_tInfo.fY - 1.f),
			ObjID::EFFECT);
	}
	else if (KEY_DOWN(VK_F7))
	{
		CSoundMgr::GetInstance()->PlaySound(L"PotionUse.wav", CSoundMgr::USE_POTION);

		m_tInfo.iMp += (int)(m_tInfo.iMaxMp*0.1f);

		if (m_tInfo.iMp > m_tInfo.iMaxMp)
			m_tInfo.iMp = m_tInfo.iMaxMp;

		CObjMgr::GetInstance()->AddObject(CAbstractFactory<CMpPotionEffect>::CreateObj(m_tInfo.fX, m_tInfo.fY -1.f),
			ObjID::EFFECT);
	}
	else if (KEY_DOWN('P'))	/*Player Info*/
	{
		m_bIsInfoUi = !m_bIsInfoUi;
		dynamic_cast<CPlayerInfo*>(m_pPlayerInfo)->SetbIsInfoOn(m_bIsInfoUi);

		if (m_bIsInfoUi == true)
			dynamic_cast<CStateButton*>(m_pStateButton)->SetDrawKey(1);
		else
			dynamic_cast<CStateButton*>(m_pStateButton)->SetDrawKey(0);
	}
	else if (KEY_DOWN('I'))	/*Player Inventory*/
	{
		m_bIsInvenUi = !m_bIsInvenUi;
		dynamic_cast<CInventory*>(m_pInventory)->SetbIsInvenOn(m_bIsInvenUi);

		if (m_bIsInvenUi == true)
			dynamic_cast<CInvenButton*>(m_pInvenButton)->SetDrawKey(1);
		else
			dynamic_cast<CInvenButton*>(m_pInvenButton)->SetDrawKey(0);

			
	}
	else if (KEY_DOWN('E'))	/*Player Equipment*/
	{
		m_bIsEquipUi = !m_bIsEquipUi;
		dynamic_cast<CEquipment*>(m_pEquipment)->SetbIsEquipOn(m_bIsEquipUi);

		if (m_bIsEquipUi == true)
			dynamic_cast<CEquipButton*>(m_pEquipButton)->SetDrawKey(1);
		else
			dynamic_cast<CEquipButton*>(m_pEquipButton)->SetDrawKey(0);

	}

	/*���� ���� ���� �ð�.*/
	if (m_bIsAttack)
	{
		m_eCurStance = ATTACK;
		
		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{	
			m_iComboCnt += 1;	//�޺� ī��Ʈ 1����.
			if (m_iComboCnt > 1)
			{
				m_bIsCombo = true;
				/*�޺��ڽ� ��ǥ & �޺�ī���� ������Ʈ*/
				dynamic_cast<CComboAttack*>(m_pComboBox)->UpdateComboCnt(m_iComboCnt);
				dynamic_cast<CComboAttack*>(m_pComboBox)->SetbIsCombo(m_bIsCombo);

				CObjMgr::GetInstance()->AddObject(CAbstractFactory<CComboCnt>::CreateComCnt(m_tInfo.fX - 30.f, m_tInfo.fY - 90.f, m_iComboCnt), ObjID::EFFECT);

			}

			/*���� ���� ������ �ٽ� IDLE ���·�.*/
			m_bIsAttack = false;
			m_eCurStance = IDLE;
		}
	}

	/*��ų ���� ���� �ð�.*/
	if (m_bIsSkill)
	{
		m_eCurStance = SKILL;

		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{	
			/*��ų ���� ������ �ٽ� IDLE ���·�.*/
			m_bIsSkill = false;
			m_eCurStance = IDLE;
		}
	}
	/*�ǰ� ���� ���� �ð�*/
	if (m_bIsHit)
	{
		m_eCurStance = HIT;
		iHitTime += 5;

		if (iHitTime > 30)
		{	/*��ų ���� ������ �ٽ� IDLE ���·�.*/
			m_bIsHit = false;
			m_eCurStance = IDLE;
			iHitTime = 0;
		}
	}


	/*���� �޺� �����̸�*/
	if (m_bIsCombo)
	{
		++iComboAttackTime;	//�޺� ���� �ð� ī����.

		/*������ ���ǵ� ������.*/
		switch (m_eCurStance)
		{
		case ATTACK:
			m_tFrame.dwFrameSpd = 50;
			break;
		case SKILL:
			m_tFrame.dwFrameSpd = 50;
			break;
		}
	}
	else
	{
		/*�޺� ���� �ð��� �ʱ�ȭ.*/
		iComboAttackTime = 0;

		/*������ ���ǵ� �������*/
		switch (m_eCurStance)
		{
		case ATTACK:
			m_tFrame.dwFrameSpd = 100;
			break;
		case SKILL:
			m_tFrame.dwFrameSpd = 100;
			break;
		}
	}

	if(iComboAttackTime > COMBO_TIME)
	{
		m_bIsCombo = false;
		iComboAttackTime = 0;
		/*�޺� ī���� �ٽ� 0����.*/
		m_iComboCnt = 0;

		/*�޺��ڽ� ������Ʈ.*/
		dynamic_cast<CComboAttack*>(m_pComboBox)->SetbIsCombo(m_bIsCombo);
		dynamic_cast<CComboAttack*>(m_pComboBox)->UpdateComboCnt(m_iComboCnt);
	}

	m_fAttackX = m_tColInfo.fX + cosf(m_fAngle * PI / 180.f) * fAttLen;
	m_fAttackY = m_tColInfo.fY + sinf(m_fAngle * PI / 180.f) * fAttLen;
	
	return NO_EVENT;
}

void CPlayer::LateUpdate()
{
	CObj::UpdateColXYPos();

	/*�ִϸ��̼� �Լ�.*/
	CObj::MoveFrame();
	SceneChange();
	IsOffSet();

	/*������ �˻�.*/
	IsLevelUp();

	/*�޺��ڽ� ��ǥ����*/
	if(m_pComboBox != nullptr)
		m_pComboBox->SetPos(m_tInfo.fX - 30.f, m_tInfo.fY - 80.f);

	/*������ ��ũ�� �� ����ȭ.*/
	switch (CSceneMgr::GetInstance()->GetCurScene())
	{
	case CSceneMgr::SC_TOWN:
		CScrollMgr::ScrollRockTown();
		break;
	case CSceneMgr::SC_STORE:
		CScrollMgr::ScrollRockStore();
		break;
	case CSceneMgr::SC_FIELD:
		CScrollMgr::ScrollRockField();
		break;
	case CSceneMgr::SC_DUNGEON:
		CScrollMgr::ScrollRockDungeon();
		break;
	case CSceneMgr::SC_BOSS:
		CScrollMgr::ScrollRockBoss();
		break;
	}
}

void CPlayer::Render(HDC hDC)
{
	CObj::UpdateRect();		//�̹����ڽ� ������Ʈ.
	CObj::UpdateColRect();	//�浹�ڽ�  ������Ʈ.


	/*Player Render*/
	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(m_pFrameKey.c_str()));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();
	
	/*�浹�ڽ� ���� - ��ũ�� ����.*/
	/*Rectangle(hDC, m_tColRect.left + fScrollX, m_tColRect.top + fScrollY,
		m_tColRect.right + fScrollX, m_tColRect.bottom + fScrollY);*/


	/*��ũ�� �����Ͽ� ����*/
	GdiTransparentBlt(hDC, 
					  m_tRect.left + (int)fScrollX, 
					  m_tRect.top + (int)fScrollY, 
					  (int)m_tInfo.fCX, 
					  (int)m_tInfo.fCY,
					  hMemDC, 
					  m_tFrame.iFrameStart * (int)m_tInfo.fCX, 
					  m_tFrame.iScene * (int)m_tInfo.fCY,
					  (int)m_tInfo.fCX, 
					  (int)m_tInfo.fCY, 
					  RGB(255, 0, 255));

	/*Player Info Render*/
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Player_Info"));
	GdiTransparentBlt(hDC, 0, 0, 178, 100, hMemDC, 0, 0, 178, 100, RGB(255, 0, 255));

	/*Player Hp, Mp, Sp*/
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Player_Hp"));
	GdiTransparentBlt(hDC, 95, 16, (int)(m_tInfo.iHp*HP_MP_SP_LEN) / m_tInfo.iMaxHp, 14,
		hMemDC, 0, 0, 84, 14, RGB(255, 0, 255));
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Player_Mp"));
	GdiTransparentBlt(hDC, 95, 30, (int)(m_tInfo.iMp*HP_MP_SP_LEN) / m_tInfo.iMaxMp, 14,
		hMemDC, 0, 0, 84, 14, RGB(255, 0, 255));
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Player_Sp"));
	GdiTransparentBlt(hDC, 95, 44, (int)(m_tInfo.iSp*HP_MP_SP_LEN) / m_tInfo.iMaxSp, 14,
		hMemDC, 0, 0, 84, 14, RGB(255, 0, 255));


	/*Hp Mp Sp ��ġ TextOut*/
	TCHAR szBuf[32] = L"";
	swprintf_s(szBuf, L"%d / %d", m_tInfo.iHp, m_tInfo.iMaxHp);
	
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC, 100,15, szBuf, lstrlen(szBuf));

	swprintf_s(szBuf, L"%d / %d", m_tInfo.iMp, m_tInfo.iMaxMp);
	TextOut(hDC, 100, 29, szBuf, lstrlen(szBuf));

	swprintf_s(szBuf, L"%d / %d", m_tInfo.iSp, m_tInfo.iMaxSp);
	TextOut(hDC, 100, 44, szBuf, lstrlen(szBuf));




	/*Player MoveState - �ȱ� & �޸��� �̹��� ��ȯ.*/
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Player_MoveState"));
	GdiTransparentBlt(hDC, 27, 59, 12, 11, hMemDC, m_iMoveState * 12, 0, 12, 11, RGB(255, 0, 255));

	/*Player Slot Render*/
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Player_Slot"));
	GdiTransparentBlt(hDC, 0, WINCY - 101, 135, 101, hMemDC, 0, 0, 135, 101, RGB(255, 255, 255));

	/*Moon Skill Quick Icon Render*/
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Quick_Skill"));
	BitBlt(hDC, 12, 528, 24, 30, hMemDC, 0 * 24, 0, SRCCOPY);
	/*Soul Attack Quick Icon*/
	BitBlt(hDC, 41, 528, 24, 30, hMemDC, 1 * 24, 0, SRCCOPY);
	/*Multi Attack Quick Icon*/
	BitBlt(hDC, 71, 528, 24, 30, hMemDC, 4 * 24, 0, SRCCOPY);
	
	/*���� ������ �̹��� ����*/
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Potion"));
	GdiTransparentBlt(hDC, 12, 570, 24, 24, hMemDC, 0 * 24, 0, 24, 24, RGB(255, 255, 255));
	GdiTransparentBlt(hDC, 41, 570, 24, 24, hMemDC, 1 * 24, 0, 24, 24, RGB(255, 255, 255));
	GdiTransparentBlt(hDC, 71, 570, 24, 24, hMemDC, 2 * 24, 0, 24, 24, RGB(255, 255, 255));
}

void CPlayer::Release()
{

}

void CPlayer::SceneChange()
{
	if (m_eCurStance != m_ePreStance)
	{
		switch (m_eCurStance)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 4;
			m_tFrame.iScene = 5;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 100;
			break;
		case WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iScene = 6;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 100;
			break;
		case RUN:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iScene = 3;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 100;
			break;
		case ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iScene = 0;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 100;
			break;
		case SKILL:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iScene = 2;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 100;
			break;
		case HIT:
			m_tFrame.iFrameStart = 1;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iScene = 1;
			m_tFrame.dwOldTime = GetTickCount();
			m_tFrame.dwFrameSpd = 200;
			break;
		}

		m_ePreStance = m_eCurStance;
	}
}

void CPlayer::IsOffSet()
{
	/*Player ���� ���� ����� �� ��ũ�� ����.*/
	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

	if (WINCX / 2 + 100 < int(m_tInfo.fX + fScrollX))
		CScrollMgr::SetScrollX(-m_fSpeed);
	if (WINCX / 2 - 100 > int(m_tInfo.fX + fScrollX))
		CScrollMgr::SetScrollX(m_fSpeed);


	if (WINCY / 2 + 50 < int(m_tInfo.fY + fScrollY))
		CScrollMgr::SetScrollY(-m_fSpeed);
	if (WINCY / 2 - 50 > int(m_tInfo.fY + fScrollY))
		CScrollMgr::SetScrollY(m_fSpeed);
}

void CPlayer::IsLevelUp()
{
	if (m_tInfo.iExp >= m_tInfo.iMaxExp)
	{
		while (m_tInfo.iExp >= m_tInfo.iMaxExp)
		{
			CSoundMgr::GetInstance()->PlaySound(L"lvup.wav", CSoundMgr::LEVEL_UP);


			CObjMgr::GetInstance()->AddObject(CAbstractFactory<CLevelUp>::CreateObj(m_tInfo.fX, m_tInfo.fY - 120.f),
				ObjID::EFFECT);

			CObjMgr::GetInstance()->AddObject(CAbstractFactory<CLevelUpEffect>::CreateObj(m_tInfo.fX, m_tInfo.fY),
				ObjID::EFFECT);

			m_tInfo.iExp -= m_tInfo.iMaxExp;

			++m_tInfo.iLevel;

			m_tInfo.iMaxHp = (int)(m_tInfo.iMaxHp+ m_tInfo.iMaxHp*0.1f);
			m_tInfo.iHp = m_tInfo.iMaxHp;

			m_tInfo.iMaxMp = (int)(m_tInfo.iMaxMp  + m_tInfo.iMaxMp*0.05f);
			m_tInfo.iMp = m_tInfo.iMaxMp;

			m_tInfo.iMaxSp =(int)(m_tInfo.iMaxSp  + m_tInfo.iMaxSp* 0.05f);
			m_tInfo.iSp = m_tInfo.iMaxSp;

			m_tInfo.iAtt = (int)(m_tInfo.iAtt + m_tInfo.iAtt*0.2f);


			m_tAbility.iStap	+= 2;
			m_tAbility.iHack	+= 5;
			m_tAbility.iInt		+= 1;
			m_tAbility.iDef		+= 3;
			m_tAbility.iMr		+= 1;
			m_tAbility.iDex		+= 2;
			m_tAbility.iAgl		+= 2;


			m_tInfo.iMaxExp = (int)(m_tInfo.iMaxExp + m_tInfo.iMaxExp*0.2f);
		}
	}
}
