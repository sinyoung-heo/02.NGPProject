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
#pragma region READY_PLAYER_RESOURCE
	/*Player 비트맵*/
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
#pragma endregion

	/*스킬 아이콘 PushBack*/
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CSkillIcon>::CreateObj(64.f, 81.f), ObjID::UI);
	/*스킬 상태 & 스킬 아이콘 객체*/
	m_pSkillIcon = CObjMgr::GetInstance()->GetSkillIconUi();
	m_eSkill = MOON;

	///*Player Info PushBack*/
	//CObjMgr::GetInstance()->AddObject(CAbstractFactory<CPlayerInfo>::CreateObj(450.f, 10.f), ObjID::UI);



	/*애니메이션 변수 IDLE 상태로 초기화.*/
	m_pFrameKey = L"Player_Down_";
	m_eCurStance = IDLE;
	m_ePreStance = m_eCurStance;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_tFrame.iScene = 5;
	m_tFrame.dwOldTime = GetTickCount();
	m_tFrame.dwFrameSpd = 200;

	m_iMoveState = 0;
	m_bIsRunning = true;
	m_bIsAttack = false;
	m_bIsSkill = false;
	m_bIsHit = false;

	/*최초 한 번만 생성하기 위한 변수*/
	m_bIsCreate = true;

	/*Player Info*/
	m_tInfo.fCX = 256.f;
	m_tInfo.fCY = 256.f;
	wstring wstr(g_szServerName, &g_szServerName[64]);
	//m_tInfo.szName = L"Sin_Young";
	m_tInfo.szName	= wstr;
	m_tInfo.iLevel	= 1;
	m_tInfo.iMaxHp	= 9'999;
	m_tInfo.iHp		= m_tInfo.iMaxHp;
	m_tInfo.iMaxMp	= 9'999;
	m_tInfo.iMp		= m_tInfo.iMaxMp;
	m_tInfo.iMaxSp	= 9'999;
	m_tInfo.iSp		= m_tInfo.iMaxSp;
	m_tInfo.iAtt	= 1'000;
	m_tInfo.iExp	= 0;
	m_tInfo.iMaxExp = 200;
	m_tInfo.iGold	= 1000000;

	m_tAbility.iStap	= 2;
	m_tAbility.iHack	= 5;
	m_tAbility.iInt		= 1;
	m_tAbility.iDef		= 3;
	m_tAbility.iMr		= 1;
	m_tAbility.iDex		= 4;
	m_tAbility.iAgl		= 3;

	m_iComboCnt = 0;

	/*충돌박스 좌표 = Player의 좌표*/
	CObj::UpdateColXYPos();
	/*충돌박스 크기*/
	m_tColInfo.fCX = 30.f;
	m_tColInfo.fCY = 30.f;

	/*이동속도*/
	m_fSpeed = 2.5f;

	/*스킬&공격 방향*/
	fAttLen = 60.f;
	m_fAngle = 270.f;

	m_fAttackX = m_tColInfo.fX + cosf(m_fAngle * PI / 180.f) * fAttLen;
	m_fAttackY = m_tColInfo.fY + sinf(m_fAngle * PI / 180.f) * fAttLen;

	m_eCurScene = CSceneMgr::SCENE::SC_TOWN;
	m_eCurDir = ObjDir::DOWN;
	m_ePreDir = m_eCurDir;
}

void CPlayer::LateInit()
{
	/*Player Info & Equipment & Inventory PushBack*/
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CPlayerInfo>::CreateObj(625.f, 160.f), ObjID::UI);
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CInventory>::CreateObj(625.f, 300.f), ObjID::INVENTORY);
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CEquipment>::CreateObj(130.f, 250.f), ObjID::EQUIPMENT);

	m_pPlayerInfo = CObjMgr::GetInstance()->GetUiBack();
	m_pInventory = CObjMgr::GetInstance()->GetInventory();
	m_pEquipment = CObjMgr::GetInstance()->GetEquipment();

	/*인벤토리에 장비창 전달*/
	static_cast<CInventory*>(m_pInventory)->SetEquipment(CObjMgr::GetInstance()->GetEquipment());
	/*장비창에 인벤토리 전달*/
	static_cast<CEquipment*>(m_pEquipment)->SetInventory(CObjMgr::GetInstance()->GetInventory());

	/*Ui Button 생성*/
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CStateButton>::CreateObj(790.f, 30.f), ObjID::UI_BUTTON);
	m_pStateButton = CObjMgr::GetInstance()->GetUiButton();
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CEquipButton>::CreateObj(790.f, 81.f), ObjID::UI_BUTTON);
	m_pEquipButton = CObjMgr::GetInstance()->GetUiButton();
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CInvenButton>::CreateObj(790.f, 131.f), ObjID::UI_BUTTON);
	m_pInvenButton = CObjMgr::GetInstance()->GetUiButton();

	/*ComboBox 생성*/
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CComboAttack>::CreateObj(1200.f, 1200.f), ObjID::COMBO_BOX);
	m_pComboBox = CObjMgr::GetInstance()->GetComboBox();
}

int CPlayer::Update()
{
	CObj::LateInit();

	if (m_bIsDead)
		return DEAD_OBJ;

	// Player Key Input.
	KeyInput();


	/*__________________________________________________________________________________________________________
	[ 기본 공격 상태 유지시간 체크 ]
	____________________________________________________________________________________________________________*/
	if (m_bIsAttack)
	{
		m_eCurStance = ATTACK;

		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{
			//콤보 카운트 1증가.
			m_iComboCnt += 1;
			if (m_iComboCnt > 1)
			{
				m_bIsCombo = true;
				// 콤보박스 좌표 & 콤보카운팅 업데이트.
				static_cast<CComboAttack*>(m_pComboBox)->UpdateComboCnt(m_iComboCnt);
				static_cast<CComboAttack*>(m_pComboBox)->SetbIsCombo(m_bIsCombo);

				CObjMgr::GetInstance()->AddObject(CAbstractFactory<CComboCnt>::CreateComCnt(m_tInfo.fX - 30.f, m_tInfo.fY - 90.f, m_iComboCnt), ObjID::EFFECT);

			}

			// 공격 상태 끝나면 다시 IDLE 상태로.
			m_bIsAttack = false;
			m_eCurStance = IDLE;
		}
	}

	/*__________________________________________________________________________________________________________
	[ 스킬 공격 상태 유지시간 체크. ]
	____________________________________________________________________________________________________________*/
	if (m_bIsSkill)
	{
		m_eCurStance = SKILL;

		if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
		{
			// 스킬 상태 끝나면 다시 IDLE 상태로.
			m_bIsSkill = false;
			m_eCurStance = IDLE;
		}
	}

	// 피격 상태 유지 시간.
	if (m_bIsHit)
	{
		m_eCurStance = HIT;
		iHitTime += 5;

		if (iHitTime > 30)
		{
			// 스킬 상태 끝나면 다시 IDLE 상태로.
			m_bIsHit = false;
			m_eCurStance = IDLE;
			iHitTime = 0;
		}
	}

	// 만약 콤보 상태이면.
	if (m_bIsCombo)
	{
		//콤보 제한 시간 카운팅.
		++iComboAttackTime;

		// 프레임 스피드 빨라짐.
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
		// 콤보 유지 시간도 초기화.
		iComboAttackTime = 0;

		// 프레임 스피드 원래대로.
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

	if (iComboAttackTime > COMBO_TIME)
	{
		m_bIsCombo = false;
		iComboAttackTime = 0;

		// 콤보 카운터 다시 0으로.
		m_iComboCnt = 0;

		// 콤보박스 업데이트.
		static_cast<CComboAttack*>(m_pComboBox)->SetbIsCombo(m_bIsCombo);
		static_cast<CComboAttack*>(m_pComboBox)->UpdateComboCnt(m_iComboCnt);
	}

	m_fAttackX = m_tColInfo.fX + cosf(m_fAngle * PI / 180.f) * fAttLen;
	m_fAttackY = m_tColInfo.fY + sinf(m_fAngle * PI / 180.f) * fAttLen;

	return NO_EVENT;

}

void CPlayer::LateUpdate()
{
	CObj::UpdateColXYPos();

	/*애니메이션 함수.*/
	CObj::MoveFrame();
	SceneChange();
	DirChange();
	IsOffSet();

	/*레벨업 검사.*/
	IsLevelUp();

	/*콤보박스 좌표갱신*/
	if (m_pComboBox != nullptr)
		m_pComboBox->SetPos(m_tInfo.fX - 30.f, m_tInfo.fY - 80.f);

	/*씬마다 스크롤 락 최적화.*/
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
	CObj::UpdateRect();		//이미지박스 업데이트.
	CObj::UpdateColRect();	//충돌박스  업데이트.


	/*Player Render*/
	HDC hMemDC = nullptr;
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(m_pFrameKey.c_str()));

	float fScrollX = CScrollMgr::GetScrollX();
	float fScrollY = CScrollMgr::GetScrollY();

		/*스크롤 적용하여 렌더*/
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
	GdiTransparentBlt(hDC, 95, 16, (int)(m_tInfo.iHp * HP_MP_SP_LEN) / m_tInfo.iMaxHp, 14,
		hMemDC, 0, 0, 84, 14, RGB(255, 0, 255));
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Player_Mp"));
	GdiTransparentBlt(hDC, 95, 30, (int)(m_tInfo.iMp * HP_MP_SP_LEN) / m_tInfo.iMaxMp, 14,
		hMemDC, 0, 0, 84, 14, RGB(255, 0, 255));
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Player_Sp"));
	GdiTransparentBlt(hDC, 95, 44, (int)(m_tInfo.iSp * HP_MP_SP_LEN) / m_tInfo.iMaxSp, 14,
		hMemDC, 0, 0, 84, 14, RGB(255, 0, 255));


	/*Hp Mp Sp 수치 TextOut*/
	TCHAR szBuf[32] = L"";
	swprintf_s(szBuf, L"%d / %d", m_tInfo.iHp, m_tInfo.iMaxHp);

	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC, 100, 15, szBuf, lstrlen(szBuf));

	swprintf_s(szBuf, L"%d / %d", m_tInfo.iMp, m_tInfo.iMaxMp);
	TextOut(hDC, 100, 29, szBuf, lstrlen(szBuf));

	swprintf_s(szBuf, L"%d / %d", m_tInfo.iSp, m_tInfo.iMaxSp);
	TextOut(hDC, 100, 44, szBuf, lstrlen(szBuf));




	/*Player MoveState - 걷기 & 달리기 이미지 전환.*/
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

	/*포션 아이콘 이미지 랜더*/
	NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Potion"));
	GdiTransparentBlt(hDC, 12, 570, 24, 24, hMemDC, 0 * 24, 0, 24, 24, RGB(255, 255, 255));
	GdiTransparentBlt(hDC, 41, 570, 24, 24, hMemDC, 1 * 24, 0, 24, 24, RGB(255, 255, 255));
	GdiTransparentBlt(hDC, 71, 570, 24, 24, hMemDC, 2 * 24, 0, 24, 24, RGB(255, 255, 255));
}

void CPlayer::Release()
{

}

void CPlayer::KeyInput()
{
	if (KEY_DOWN('L'))
		m_tInfo.iExp += 1000;

	/*__________________________________________________________________________________________________________
	[ Change RUN / WALK ]
	____________________________________________________________________________________________________________*/
#pragma region RUN_AND_WALK
	//if (KEY_DOWN('R'))
	//{
	//	m_bIsRunning = !m_bIsRunning;
	//	m_iMoveState = (m_iMoveState + 1) % 2;
	//}
	//if (m_bIsRunning == false)
	//{
	//	++m_tInfo.iSp;

	//	if (m_tInfo.iSp > m_tInfo.iMaxSp)
	//		m_tInfo.iSp = m_tInfo.iMaxSp;
	//}
	//if (m_tInfo.iSp <= 0)
	//{
	//	m_tInfo.iSp = 0;
	//	m_bIsRunning = false;
	//	m_iMoveState = (m_iMoveState + 1) % 2;
	//}
#pragma endregion

	/*__________________________________________________________________________________________________________
	[ Player Move - RUN ]
	____________________________________________________________________________________________________________*/
#pragma region CLIENT DATA
	//if (KEY_PRESSING('A'))
	//{
	//	// 공격 방향 설정
	//	m_fAngle = 180.f;

	//	// 이미지박스 & 충돌박스 같이 이동.
	//	if (KEY_PRESSING('W'))
	//	{	
	//		// 공격 방향 설정
	//		m_fAngle = 240.f;

	//		if (m_bIsRunning)
	//		{	
	//			//달리기
	//			m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
	//			m_tInfo.fY -= m_fSpeed / sqrtf(2.f);
	//			CObj::UpdateColXYPos();

	//			m_pFrameKey = L"Player_LeftUp_";
	//			m_eCurStance = RUN;

	//			--m_tInfo.iSp;
	//		}
	//		else
	//		{	
	//			//걷기
	//			m_tInfo.fX -= m_fSpeed / sqrtf(2.f) / 2;
	//			m_tInfo.fY -= m_fSpeed / sqrtf(2.f) / 2;
	//			CObj::UpdateColXYPos();

	//			m_pFrameKey = L"Player_LeftUp_";
	//			m_eCurStance = WALK;
	//		}
	//	}
	//	else if (KEY_PRESSING('D'))
	//	{
	//		// 공격 방향 설정
	//		m_fAngle = 125.f;

	//		if (m_bIsRunning)
	//		{	
	//			//달리기
	//			m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
	//			m_tInfo.fY += m_fSpeed / sqrtf(2.f);
	//			CObj::UpdateColXYPos();

	//			m_pFrameKey = L"Player_LeftDown_";
	//			m_eCurStance = RUN;

	//			--m_tInfo.iSp;
	//		}
	//		else
	//		{	
	//			//걷기
	//			m_tInfo.fX -= m_fSpeed / sqrtf(2.f) / 2;
	//			m_tInfo.fY += m_fSpeed / sqrtf(2.f) / 2;
	//			CObj::UpdateColXYPos();

	//			m_pFrameKey = L"Player_LeftDown_";
	//			m_eCurStance = WALK;
	//		}
	//	}
	//	else
	//	{
	//		// 좌 이동
	//		if (m_bIsRunning)
	//		{	
	//			// 달리기
	//			m_tInfo.fX -= m_fSpeed;
	//			CObj::UpdateColXYPos();

	//			m_pFrameKey = L"Player_Left_";
	//			m_eCurStance = RUN;

	//			--m_tInfo.iSp;
	//		}
	//		else
	//		{	
	//			// 걷기
	//			m_tInfo.fX -= m_fSpeed / 2;
	//			CObj::UpdateColXYPos();

	//			m_pFrameKey = L"Player_Left_";
	//			m_eCurStance = WALK;
	//		}
	//	}
	//}
	//else if (KEY_PRESSING('D'))
	//{
	//	// 공격 방향 설정
	//	m_fAngle = 360.f;
	//	if (KEY_PRESSING('W'))
	//	{	
	//		// 공격 방향 설정
	//		m_fAngle = 300.f;

	//		if (m_bIsRunning) //달리기
	//		{	
	//			// 이미지 박스
	//			m_tInfo.fX += m_fSpeed / sqrtf(2.f);
	//			m_tInfo.fY -= m_fSpeed / sqrtf(2.f);

	//			// 충돌 박스
	//			CObj::UpdateColXYPos();

	//			m_pFrameKey = L"Player_RightUp_";
	//			m_eCurStance = RUN;

	//			--m_tInfo.iSp;
	//		}
	//		else
	//		{	
	//			//걷기
	//			// 이미지 박스
	//			m_tInfo.fX += m_fSpeed / sqrtf(2.f) / 2;
	//			m_tInfo.fY -= m_fSpeed / sqrtf(2.f) / 2;
	//			
	//			// 충돌 박스
	//			CObj::UpdateColXYPos();

	//			m_pFrameKey = L"Player_RightUp_";
	//			m_eCurStance = WALK;
	//		}
	//	}
	//	else if (KEY_PRESSING('S'))
	//	{
	//		// 공격 방향 설정
	//		m_fAngle = 55.f;

	//		if (m_bIsRunning)
	//		{	
	//			//달리기
	//			m_tInfo.fX += m_fSpeed / sqrtf(2.f);
	//			m_tInfo.fY += m_fSpeed / sqrtf(2.f);
	//			CObj::UpdateColXYPos();

	//			m_pFrameKey = L"Player_RightDown_";
	//			m_eCurStance = RUN;

	//			--m_tInfo.iSp;
	//		}
	//		else
	//		{	
	//			//걷기
	//			m_tInfo.fX += m_fSpeed / sqrtf(2.f) / 2;
	//			m_tInfo.fY += m_fSpeed / sqrtf(2.f) / 2;
	//			CObj::UpdateColXYPos();

	//			m_pFrameKey = L"Player_RightDown_";
	//			m_eCurStance = WALK;
	//		}
	//	}
	//	else
	//	{	
	//		//달리기
	//		if (m_bIsRunning)
	//		{	
	//			// 우 이동
	//			m_tInfo.fX += m_fSpeed;
	//			CObj::UpdateColXYPos();

	//			m_pFrameKey = L"Player_Right_";
	//			m_eCurStance = RUN;

	//			--m_tInfo.iSp;
	//		}
	//		else
	//		{ 
	//			//걷기
	//			m_tInfo.fX += m_fSpeed / 2;
	//			CObj::UpdateColXYPos();

	//			m_pFrameKey = L"Player_Right_";
	//			m_eCurStance = WALK;
	//		}
	//	}
	//}
	//else if (KEY_PRESSING('W'))
	//{
	//	// 공격 방향 설정
	//	m_fAngle = 270.f;
	//	if (m_bIsRunning)
	//	{	
	//		//달리기
	//		m_tInfo.fY -= m_fSpeed;
	//		CObj::UpdateColXYPos();

	//		m_pFrameKey = L"Player_Up_";
	//		m_eCurStance = RUN;

	//		--m_tInfo.iSp;
	//	}
	//	else
	//	{	
	//		//걷기
	//		m_tInfo.fY -= m_fSpeed / 2;
	//		CObj::UpdateColXYPos();

	//		m_pFrameKey = L"Player_Up_";
	//		m_eCurStance = WALK;
	//	}

	//}
	//else if (KEY_PRESSING('S'))
	//{
	//	// 공격 방향 설정
	//	m_fAngle = 90.f;
	//	if (m_bIsRunning)
	//	{	
	//		//달리기
	//		m_tInfo.fY += m_fSpeed;
	//		CObj::UpdateColXYPos();

	//		m_pFrameKey = L"Player_Down_";
	//		m_eCurStance = RUN;

	//		--m_tInfo.iSp;
	//	}
	//	else
	//	{	
	//		//걷기
	//		m_tInfo.fY += m_fSpeed / 2;
	//		CObj::UpdateColXYPos();

	//		m_pFrameKey = L"Player_Down_";
	//		m_eCurStance = WALK;
	//	}
	//}
	/*else if (KEY_UP('A'))
		m_eCurStance = IDLE;
	else if (KEY_UP('D'))
		m_eCurStance = IDLE;
	else if (KEY_UP('W'))
		m_eCurStance = IDLE;
	else if (KEY_UP('S'))
		m_eCurStance = IDLE;*/
#pragma endregion

	if (KEY_PRESSING('A') && g_bIsActive)
	{
		CPacketMgr::GetInstance()->SendPlayerMovePacket(MV_LEFT);
		m_pFrameKey		= L"Player_Left_";
		m_eCurStance	= RUN;
		m_eCurDir		= ObjDir::LEFT;

		m_fAngle = 180.0f;
	}
	else if (KEY_PRESSING('D') && g_bIsActive)
	{
		CPacketMgr::GetInstance()->SendPlayerMovePacket(MV_RIGHT);

		m_pFrameKey		= L"Player_Right_";
		m_eCurStance	= RUN;
		m_eCurDir		= ObjDir::RIGHT;

		m_fAngle = 360.0f;
	}
	else if (KEY_PRESSING('W') && g_bIsActive)
	{
		CPacketMgr::GetInstance()->SendPlayerMovePacket(MV_UP);

		m_pFrameKey		= L"Player_Up_";
		m_eCurStance	= RUN;
		m_eCurDir		= ObjDir::UP;

		m_fAngle = 270.0f;
	}
	else if (KEY_PRESSING('S') && g_bIsActive)
	{
		CPacketMgr::GetInstance()->SendPlayerMovePacket(MV_DOWN);
		m_pFrameKey		= L"Player_Down_";
		m_eCurStance	= RUN;
		m_eCurDir		= ObjDir::DOWN;

		m_fAngle = 90.0f;
	}

	else if (KEY_UP('A'))
		m_eCurStance = IDLE;
	else if (KEY_UP('D'))
		m_eCurStance = IDLE;
	else if (KEY_UP('W'))
		m_eCurStance = IDLE;
	else if (KEY_UP('S'))
		m_eCurStance = IDLE;



	/*__________________________________________________________________________________________________________
	[ Player Basic Attack ]
	____________________________________________________________________________________________________________*/
	else if (KEY_DOWN(VK_SPACE) && g_bIsActive /*|| KEY_DOWN(VK_LBUTTON)*/)
	{
		CSoundMgr::GetInstance()->PlaySound(L"pAttack.wav", CSoundMgr::PLAYER_ATTACK);
		m_bIsAttack		= true;
		m_eCurStance	= ATTACK;

		CObj::UpdateColXYPos();
		m_fAttackX = m_tColInfo.fX + cosf(m_fAngle * PI / 180.f) * fAttLen;
		m_fAttackY = m_tColInfo.fY + sinf(m_fAngle * PI / 180.f) * fAttLen;

		CObjMgr::GetInstance()->AddObject(CreateSkill<CSkill>(), ObjID::SKILL);

		// Send AttackPacket
		CPacketMgr::GetInstance()->SendPlayerAttack(m_fAttackX, m_fAttackY,SKILL_BASIC);
	}

	// 스킬 공격 등록
	else if (KEY_DOWN(VK_F1) && g_bIsActive)
	{
		m_eSkill = MOON;
		static_cast<CSkillIcon*>(m_pSkillIcon)->SetImageKey(0);
	}
	else if (KEY_DOWN(VK_F2) && g_bIsActive)
	{
		m_eSkill = SOUL;
		static_cast<CSkillIcon*>(m_pSkillIcon)->SetImageKey(1);
	}
	else if (KEY_DOWN(VK_F3) && g_bIsActive)
	{
		m_eSkill = MULTI;
		static_cast<CSkillIcon*>(m_pSkillIcon)->SetImageKey(4);
	}

	// 스킬 공격
	else if (KEY_DOWN(VK_RBUTTON) && g_bIsActive)
	{
		if (m_iComboCnt > 0)
		{
			// 콤보 카운트 0보다 큰 상태이면
			if (iComboAttackTime < COMBO_TIME)
			{
				// 콤보어택 제한시간을 넘지 않았으면.
				m_bIsCombo = true;

				// 콤보어택에 성공 했으면 제한시간 초기화.
				iComboAttackTime = 0;

				++m_iComboCnt;

				// 콤보박스 좌표 & 콤보카운팅 업데이트.
				static_cast<CComboAttack*>(m_pComboBox)->UpdateComboCnt(m_iComboCnt);
				static_cast<CComboAttack*>(m_pComboBox)->SetbIsCombo(m_bIsCombo);

				CObjMgr::GetInstance()->AddObject(CAbstractFactory<CComboCnt>::CreateComCnt(m_tInfo.fX - 30.f, m_tInfo.fY - 90.f, m_iComboCnt), ObjID::EFFECT);
			}
		}

		m_bIsSkill		= true;
		m_eCurStance	= SKILL;

		CObj::UpdateColXYPos();

		m_fAttackX = m_tColInfo.fX + cosf(m_fAngle * PI / 180.f) * fAttLen;
		m_fAttackY = m_tColInfo.fY + sinf(m_fAngle * PI / 180.f) * fAttLen;

		// MP가 100 이상일 때만 스칼 사용 가능.
		if (m_tInfo.iMp > 100)
		{
			CSoundMgr::GetInstance()->PlaySound(L"SKILL.wav", CSoundMgr::PLAYER_SKILL);

			switch (m_eSkill)
			{
			case SOUL:
				CObjMgr::GetInstance()->AddObject(CreateSkill<CSkillSoul>(), ObjID::SKILL);
				// m_tInfo.iMp -= 100;

				// Send AttackPacket
				CPacketMgr::GetInstance()->SendPlayerAttack(m_fAttackX, m_fAttackY, SKILL_SOUL);

				break;
			case MOON:
				CObjMgr::GetInstance()->AddObject(CreateSkill<CSkillMoon>(), ObjID::SKILL);
				// m_tInfo.iMp -= 100;
				
				// Send AttackPacket
				CPacketMgr::GetInstance()->SendPlayerAttack(m_fAttackX, m_fAttackY, SKILL_MOON);

				break;
			case MULTI:
				CObjMgr::GetInstance()->AddObject(CreateSkill<CMultiAttack>(), ObjID::SKILL);
				// m_tInfo.iMp -= 100;

				// Send AttackPacket
				CPacketMgr::GetInstance()->SendPlayerAttack(m_fAttackX, m_fAttackY, SKILL_MULTI);

				break;
			}
		}
		else
			CSoundMgr::GetInstance()->PlaySound(L"NoMana.wav", CSoundMgr::PLAYER_SKILL);


	}


	/*__________________________________________________________________________________________________________
	[ Use Potion. ]
	____________________________________________________________________________________________________________*/
	else if (KEY_DOWN(VK_F5) && g_bIsActive)
	{
		CSoundMgr::GetInstance()->PlaySound(L"PotionUse.wav", CSoundMgr::USE_POTION);

		m_tInfo.iHp += (int)(m_tInfo.iMaxHp * 0.1f);

		if (m_tInfo.iHp > m_tInfo.iMaxHp)
			m_tInfo.iHp = m_tInfo.iMaxHp;

		CObjMgr::GetInstance()->AddObject(CAbstractFactory<CHpPotionEffect>::CreateObj(m_tInfo.fX, m_tInfo.fY - 1.f),
			ObjID::EFFECT);
		
		CPacketMgr::GetInstance()->SendPlayerAttack(m_tInfo.fX, m_tInfo.fY, SKILL_HP);
	}
	else if (KEY_DOWN(VK_F6) && g_bIsActive)
	{
		CSoundMgr::GetInstance()->PlaySound(L"PotionUse.wav", CSoundMgr::USE_POTION);

		m_tInfo.iSp += (int)(m_tInfo.iMaxSp * 0.1f);

		if (m_tInfo.iSp > m_tInfo.iMaxSp)
			m_tInfo.iSp = m_tInfo.iMaxSp;

		CObjMgr::GetInstance()->AddObject(CAbstractFactory<CMpPotionEffect>::CreateObj(m_tInfo.fX, m_tInfo.fY - 1.f),
			ObjID::EFFECT);

		CPacketMgr::GetInstance()->SendPlayerAttack(m_tInfo.fX, m_tInfo.fY, SKILL_SP);
	}
	else if (KEY_DOWN(VK_F7) && g_bIsActive)
	{
		CSoundMgr::GetInstance()->PlaySound(L"PotionUse.wav", CSoundMgr::USE_POTION);

		m_tInfo.iMp += (int)(m_tInfo.iMaxMp * 0.1f);

		if (m_tInfo.iMp > m_tInfo.iMaxMp)
			m_tInfo.iMp = m_tInfo.iMaxMp;

		CObjMgr::GetInstance()->AddObject(CAbstractFactory<CMpPotionEffect>::CreateObj(m_tInfo.fX, m_tInfo.fY - 1.f),
			ObjID::EFFECT);

		CPacketMgr::GetInstance()->SendPlayerAttack(m_tInfo.fX, m_tInfo.fY, SKILL_MP);
	}

	/*__________________________________________________________________________________________________________
	[ Show Player's UI ]
	____________________________________________________________________________________________________________*/
	// Player Info.
	else if (KEY_DOWN('P') && g_bIsActive)
	{
		m_bIsInfoUi = !m_bIsInfoUi;
		static_cast<CPlayerInfo*>(m_pPlayerInfo)->SetbIsInfoOn(m_bIsInfoUi);

		if (m_bIsInfoUi == true)
			static_cast<CStateButton*>(m_pStateButton)->SetDrawKey(1);
		else
			static_cast<CStateButton*>(m_pStateButton)->SetDrawKey(0);
	}

	// Player Inventory.
	else if (KEY_DOWN('I') && g_bIsActive)
	{
		m_bIsInvenUi = !m_bIsInvenUi;
		static_cast<CInventory*>(m_pInventory)->SetbIsInvenOn(m_bIsInvenUi);

		if (m_bIsInvenUi == true)
			static_cast<CInvenButton*>(m_pInvenButton)->SetDrawKey(1);
		else
			static_cast<CInvenButton*>(m_pInvenButton)->SetDrawKey(0);


	}

	// Player Equipment.
	else if (KEY_DOWN('E') && g_bIsActive)
	{
		m_bIsEquipUi = !m_bIsEquipUi;
		static_cast<CEquipment*>(m_pEquipment)->SetbIsEquipOn(m_bIsEquipUi);

		if (m_bIsEquipUi == true)
			static_cast<CEquipButton*>(m_pEquipButton)->SetDrawKey(1);
		else
			static_cast<CEquipButton*>(m_pEquipButton)->SetDrawKey(0);

	}

}

void CPlayer::SceneChange()
{
	if (m_eCurStance != m_ePreStance)
	{
		switch (m_eCurStance)
		{
		case IDLE:
			m_tFrame.iFrameStart	= 0;
			m_tFrame.iFrameEnd		= 4;
			m_tFrame.iScene			= 5;
			m_tFrame.dwOldTime		= GetTickCount();
			m_tFrame.dwFrameSpd		= 100;
			break;
		case WALK:
			m_tFrame.iFrameStart	= 0;
			m_tFrame.iFrameEnd		= 8;
			m_tFrame.iScene			= 6;
			m_tFrame.dwOldTime		= GetTickCount();
			m_tFrame.dwFrameSpd		= 100;
			break;
		case RUN:
			m_tFrame.iFrameStart	= 0;
			m_tFrame.iFrameEnd		= 9;
			m_tFrame.iScene			= 3;
			m_tFrame.dwOldTime		= GetTickCount();
			m_tFrame.dwFrameSpd		= 100;
			break;
		case ATTACK:
			m_tFrame.iFrameStart	= 0;
			m_tFrame.iFrameEnd		= 5;
			m_tFrame.iScene			= 0;
			m_tFrame.dwOldTime		= GetTickCount();
			m_tFrame.dwFrameSpd		= 100;
			break;
		case SKILL:
			m_tFrame.iFrameStart	= 0;
			m_tFrame.iFrameEnd		= 8;
			m_tFrame.iScene			= 2;
			m_tFrame.dwOldTime		= GetTickCount();
			m_tFrame.dwFrameSpd		= 100;
			break;
		case HIT:
			m_tFrame.iFrameStart	= 1;
			m_tFrame.iFrameEnd		= 1;
			m_tFrame.iScene			= 1;
			m_tFrame.dwOldTime		= GetTickCount();
			m_tFrame.dwFrameSpd		= 200;
			break;
		}

		m_ePreStance = m_eCurStance;

		// Send Packet.
		CPacketMgr::GetInstance()->SendPlayerStance(static_cast<int>(m_eCurStance), 
													static_cast<int>(m_eCurDir));
	}
}

void CPlayer::DirChange()
{
	if (m_eCurDir != m_ePreDir)
	{
		m_ePreDir = m_eCurDir;

		// Send Packet.
		CPacketMgr::GetInstance()->SendPlayerStance(static_cast<int>(m_eCurStance),
													static_cast<int>(m_eCurDir));
	}
}

void CPlayer::IsOffSet()
{
	/*Player 일정 기준 벗어날을 때 스크롤 적용.*/
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

			m_tInfo.iMaxHp = (int)(m_tInfo.iMaxHp + m_tInfo.iMaxHp * 0.1f);
			m_tInfo.iHp = m_tInfo.iMaxHp;

			m_tInfo.iMaxMp = (int)(m_tInfo.iMaxMp + m_tInfo.iMaxMp * 0.05f);
			m_tInfo.iMp = m_tInfo.iMaxMp;

			m_tInfo.iMaxSp = (int)(m_tInfo.iMaxSp + m_tInfo.iMaxSp * 0.05f);
			m_tInfo.iSp = m_tInfo.iMaxSp;

			m_tInfo.iAtt = (int)(m_tInfo.iAtt + m_tInfo.iAtt * 0.2f);


			m_tAbility.iStap += 2;
			m_tAbility.iHack += 5;
			m_tAbility.iInt += 1;
			m_tAbility.iDef += 3;
			m_tAbility.iMr += 1;
			m_tAbility.iDex += 2;
			m_tAbility.iAgl += 2;


			m_tInfo.iMaxExp = (int)(m_tInfo.iMaxExp + m_tInfo.iMaxExp * 0.2f);
		}
	}
}
