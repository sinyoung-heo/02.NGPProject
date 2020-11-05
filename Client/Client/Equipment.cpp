#include "stdafx.h"
#include "Equipment.h"
#include "Item.h"
#include "Inventory.h"

DWORD dwUpdateOldPos3 = GetTickCount();

CEquipment::CEquipment()
	:m_pPlayer(nullptr), m_bIsEquipOn(false), m_pInventory(nullptr)
{
	ZeroMemory(&m_tBlankInfo, sizeof(TEXTINFO)*EQUIP_SIZE);
	ZeroMemory(&m_tBlank, sizeof(RECT)*EQUIP_SIZE);
	ZeroMemory(&m_iIsEmpty, sizeof(int)*EQUIP_SIZE);
}


CEquipment::~CEquipment()
{
	Release();
}

void CEquipment::Initialize()
{
	m_eLayer = Layer::UI;

	CBmpMgr::GetInstance()->InsertBmp(L"Equip_Info", L"../Image/Ui/Equip_Info.bmp");

	m_tInfo.fCX = 195.f;
	m_tInfo.fCY = 182.f;

	m_iMaxSize = EQUIP_SIZE;

	for (int i = 0; i < EQUIP_SIZE; ++i)
	{
		m_tBlankInfo[i].fCX = 24.f;
		m_tBlankInfo[i].fCY = 24.f;
	}
	
	m_tBlankInfo[0].fX = 50.f, m_tBlankInfo[0].fY = 195.f;
	m_tBlankInfo[1].fX = 50.f, m_tBlankInfo[1].fY = 230.f;
	m_tBlankInfo[2].fX = 50.f, m_tBlankInfo[2].fY = 260.f;
	m_tBlankInfo[3].fX = 50.f, m_tBlankInfo[3].fY = 320.f;

	m_vecEquip.reserve(EQUIP_SIZE);

	m_pPlayer = CObjMgr::GetInstance()->GetPlayer();
}

void CEquipment::LateInit()
{

}

int CEquipment::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;

	if (m_bIsEquipOn)
	{

		POINT Curpt = {};
		GetCursorPos(&Curpt);
		ScreenToClient(g_hWnd, &Curpt);

		//float fScrollX = CScrollMgr::GetScrollX();
		//float fScrollY = CScrollMgr::GetScrollY();

		//cout << Curpt.x /*- fScrollX */<< "  " << Curpt.y /*- fScrollY*/ << endl;


		/*Inventory �̵�*/
		if (PtInRect(&m_tRect, Curpt))
		{
			if (KEY_PRESSING(VK_LBUTTON))
			{
				/* �ֱ������� old �¾i ���� */
				if (dwUpdateOldPos3 + 100 <= GetTickCount())
				{
					m_fOldXpos = (float)Curpt.x;
					m_fOldYpos = (float)Curpt.y;

					dwUpdateOldPos3 = GetTickCount();
				}
				/*Ŭ�� ���� �� x ��ǥ - ���� ��ǥ */
				float fCurXpos = (float)Curpt.x;
				float fCurYpos = (float)Curpt.y;

				/*���� ���콺 ��ǥ & ���� ���콺 ��ǥ ���� ���� �̵�. */
				m_fMoveXpos = fCurXpos - m_fOldXpos;
				m_fMoveYpos = fCurYpos - m_fOldYpos;

				m_tInfo.fX += m_fMoveXpos*0.4f, m_tInfo.fY += m_fMoveYpos*0.4f;

				m_tBlankInfo[0].fX += m_fMoveXpos*0.4f, m_tBlankInfo[0].fY += m_fMoveYpos*0.4f;
				m_tBlankInfo[1].fX += m_fMoveXpos*0.4f, m_tBlankInfo[1].fY += m_fMoveYpos*0.4f;
				m_tBlankInfo[2].fX += m_fMoveXpos*0.4f, m_tBlankInfo[2].fY += m_fMoveYpos*0.4f;
				m_tBlankInfo[3].fX += m_fMoveXpos*0.4f, m_tBlankInfo[3].fY += m_fMoveYpos*0.4f;
			}


			for (int i = 0; i < EQUIP_SIZE; ++i)
			{
				if (PtInRect(&m_tBlank[i], Curpt))
				{
					if (KEY_DOWN(VK_LBUTTON))
					{
						CSoundMgr::GetInstance()->PlaySound(L"SwordEquip.mp3", CSoundMgr::ITEM_PUTOFF);
						m_iSelect = i;
						/*������ ����. ��ü ���� �Ҵ��Ͽ� inventory�� push back. & Equp���� Erase*/
						if (m_iIsEmpty[m_iSelect] == 1)
							PutOffItem(m_iSelect);
					}
				}
			}
		}
	}

	return NO_EVENT;
}

void CEquipment::LateUpdate()
{
	if (m_bIsEquipOn)
	{





	}
}

void CEquipment::Render(HDC hDC)
{
	if (m_bIsEquipOn)
	{
		CObj::UpdateRect();
		UpdateBlankRect();

		HDC hMemDC = nullptr;
		NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Equip_Info"));

		Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);

		GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			hMemDC, 0, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));


		///*������ ��ġ�� �簢�� ���*/
		//for (int i = 0; i < EQUIP_SIZE; ++i)
		//	Rectangle(hDC, m_tBlank[i].left, m_tBlank[i].top, m_tBlank[i].right, m_tBlank[i].bottom);


		/*������ �̹��� ����.*/

		SetBkMode(hDC, TRANSPARENT);
		TCHAR szBuf[32] = L"";

		for (CObj* pObj : m_vecEquip)
		{
			NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(static_cast<CItem*>(pObj)->GetImageKey()));

			/*������ type�� ���� render ��ġ ����*/
			ItemType::TYPE eType = static_cast<CItem*>(pObj)->GetItemType();

			GdiTransparentBlt(hDC, m_tBlank[eType].left, m_tBlank[eType].top, 24, 24, hMemDC, 0, 0, 24, 24, RGB(255, 0, 255));



			switch (eType)
			{
			case ItemType::HELMET:
				swprintf_s(szBuf, L"Mp + %d", pObj->GetInfo().iMaxMp);
				break;
			case ItemType::ARMOR:
				swprintf_s(szBuf, L"Hp + %d", pObj->GetInfo().iMaxHp);
				break;
			case ItemType::WEAPON:
				swprintf_s(szBuf, L"Att + %d", pObj->GetInfo().iAtt);
				break;
			case ItemType::SHOES:
				swprintf_s(szBuf, L"Sp + %d", pObj->GetInfo().iMaxSp);
				break;
			}
			TextOut(hDC, m_tBlank[eType].left + 120, m_tBlank[eType].top + 7, szBuf, lstrlen(szBuf));
		}

	}
}

void CEquipment::Release()
{
	for_each(m_vecEquip.begin(), m_vecEquip.end(), SafeDelete<CObj*>);

	m_vecEquip.clear();
}

void CEquipment::PutOnItem(int _index, ItemType::TYPE _eTpye)
{
	if (m_iIsEmpty[_eTpye] == 0)
	{
		/*�ش� type �������� ���� ���� �ƴ϶��.*/

		/*�ش� index�� ������ ����.*/
		CObj* pItem = static_cast<CInventory*>(m_pInventory)->GetItemInfo(_index);

		CObj* pNewItem = new CItem(pItem->GetInfo());
		static_cast<CItem*>(pNewItem)->SetImageKey(static_cast<CItem*>(pItem)->GetImageKey());
		static_cast<CItem*>(pNewItem)->SetItemType(static_cast<CItem*>(pItem)->GetItemType());

		m_vecEquip.push_back(pNewItem);

		/*Player �ɷ�ġ ����*/
		m_pPlayer->SetMaxHp(pNewItem->GetInfo().iMaxHp);
		m_pPlayer->SetMaxMp(pNewItem->GetInfo().iMaxMp);
		m_pPlayer->SetMaxSp(pNewItem->GetInfo().iMaxSp);
		m_pPlayer->SetAtt(pNewItem->GetInfo().iAtt);


		/*inventory���� �ش� index ������ erase*/
		static_cast<CInventory*>(m_pInventory)->EraseItem(_index);


		/*���� ������ ����.*/
		m_iIsEmpty[_eTpye] = 1;
	}
	else
	{
		/*�ش� type �������� ���� ���̶��. ���� swap.*/
		CObj* pItem = static_cast<CInventory*>(m_pInventory)->GetItemInfo(_index);

		/*�ӽ� ������ inventory���� �Ѿ�� ���� ����.*/
		INFO tTempInfo = pItem->GetInfo();
		wstring szTempImageKey = static_cast<CItem*>(pItem)->GetImageKey();
		ItemType::TYPE eTempType = static_cast<CItem*>(pItem)->GetItemType();

		/*���â�� �ִ� ���������� �κ��丮�� ����.*/
		for (size_t i = 0; i < m_vecEquip.size(); ++i)
		{
			if (static_cast<CItem*>(m_vecEquip[i])->GetItemType() == _eTpye)
			{
				m_iSelect = i;
				break;
			}
		}

		static_cast<CInventory*>(m_pInventory)->SetItemInfo(_index, m_vecEquip[m_iSelect]->GetInfo(),
			static_cast<CItem*>(m_vecEquip[m_iSelect])->GetImageKey(), static_cast<CItem*>(m_vecEquip[m_iSelect])->GetItemType());

		/*���â�� �ִ� ������ ���� Player���� ���ش�.*/
		m_pPlayer->SetMaxHp(m_vecEquip[m_iSelect]->GetInfo().iMaxHp *-1);
		m_pPlayer->SetMaxMp(m_vecEquip[m_iSelect]->GetInfo().iMaxMp *-1);
		m_pPlayer->SetMaxSp(m_vecEquip[m_iSelect]->GetInfo().iMaxSp *-1);
		m_pPlayer->SetAtt(m_vecEquip[m_iSelect]->GetInfo().iAtt *-1);

		/*�ӽ� ������ �ִ� �� ���â���� ����*/
		m_vecEquip[m_iSelect]->SetInfo(tTempInfo);
		static_cast<CItem*>(m_vecEquip[m_iSelect])->SetImageKey(szTempImageKey);
		static_cast<CItem*>(m_vecEquip[m_iSelect])->SetItemType(eTempType);

		/*���â�� �ִ� ������ ���� Player�� �ٽ� �����ش�.*/
		m_pPlayer->SetMaxHp(m_vecEquip[m_iSelect]->GetInfo().iMaxHp );
		m_pPlayer->SetMaxMp(m_vecEquip[m_iSelect]->GetInfo().iMaxMp );
		m_pPlayer->SetMaxSp(m_vecEquip[m_iSelect]->GetInfo().iMaxSp );
		m_pPlayer->SetAtt(m_vecEquip[m_iSelect]->GetInfo().iAtt );
	}
}

void CEquipment::PutOffItem(int _index)
{
	/*�ش� index�� ������ type �� ã�´�.*/
	for (size_t i = 0; i < m_vecEquip.size(); ++i)
	{
		if (static_cast<CItem*>(m_vecEquip[i])->GetItemType() == _index)
		{
			m_iSelect = i;
			break;
		}
	}


	CObj* pPutOffItem = new CItem(m_vecEquip[m_iSelect]->GetInfo());
	static_cast<CItem*>(pPutOffItem)->SetImageKey(static_cast<CItem*>(m_vecEquip[m_iSelect])->GetImageKey());
	static_cast<CItem*>(pPutOffItem)->SetItemType(static_cast<CItem*>(m_vecEquip[m_iSelect])->GetItemType());

	/*�κ��丮�� ������ �߰�.*/
	static_cast<CInventory*>(m_pInventory)->SetIsEmpty(static_cast<CInventory*>(m_pInventory)->GetInvenItemVec().size(), 1);
	static_cast<CInventory*>(m_pInventory)->GetInvenItemVec().push_back(pPutOffItem);

	/*Player �ɷ�ġ ����.*/
	m_pPlayer->SetMaxHp(m_vecEquip[m_iSelect]->GetInfo().iMaxHp * -1);
	m_pPlayer->SetMaxMp(m_vecEquip[m_iSelect]->GetInfo().iMaxMp * -1);
	m_pPlayer->SetMaxSp(m_vecEquip[m_iSelect]->GetInfo().iMaxSp * -1);
	m_pPlayer->SetAtt(m_vecEquip[m_iSelect]->GetInfo().iAtt * -1);

	/*���â���� �ش� ������ ����.*/
	m_iIsEmpty[static_cast<CItem*>(m_vecEquip[m_iSelect])->GetItemType()] = 0;
	SafeDelete(m_vecEquip[m_iSelect]);

	vector<CObj*>::iterator iter = m_vecEquip.begin();
	m_vecEquip.erase(iter + m_iSelect);
};

void CEquipment::UpdateBlankRect()
{
	for (int i = 0; i < EQUIP_SIZE; ++i)
	{
		m_tBlank[i].left	= (LONG)(m_tBlankInfo[i].fX - m_tBlankInfo[i].fCX * 0.5f);
		m_tBlank[i].top		= (LONG)(m_tBlankInfo[i].fY - m_tBlankInfo[i].fCY * 0.5f);
		m_tBlank[i].right	= (LONG)(m_tBlankInfo[i].fX + m_tBlankInfo[i].fCX * 0.5f);
		m_tBlank[i].bottom	= (LONG)(m_tBlankInfo[i].fY + m_tBlankInfo[i].fCY * 0.5f);
	}
}

