#include "stdafx.h"
#include "Inventory.h"
#include "Item.h"
#include "Equipment.h"

DWORD dwUpdateOldPos2 = GetTickCount();

CInventory::CInventory()
	:m_bIsInvenOn(false),m_pPlayer(nullptr), m_bIsSellLstOn(false)
{
	ZeroMemory(&m_tBlankInfo, sizeof(TEXTINFO)*INVEN_SIZE);
	ZeroMemory(&m_tBlank, sizeof(RECT)*INVEN_SIZE);
	ZeroMemory(&m_iIsEmpty, sizeof(int)*INVEN_SIZE);
}

CInventory::~CInventory()
{
	Release();
}

void CInventory::SetItemInfo(int _index,const INFO & _info, wstring _imagekey, ItemType::TYPE _type)
{
	m_vecInven[_index]->SetInfo(_info);
	dynamic_cast<CItem*>(m_vecInven[_index])->SetImageKey(_imagekey);
	dynamic_cast<CItem*>(m_vecInven[_index])->SetItemType(_type);
}

void CInventory::Initialize()
{
	CBmpMgr::GetInstance()->InsertBmp(L"Inven_Info", L"../Image/Ui/Inven_Info.bmp");
	
	m_eLayer = Layer::UI;

	m_tInfo.fCX = 343.f;
	m_tInfo.fCY = 398.f;

	for (int i = 0; i < INVEN_SIZE; ++i)
	{
		m_tBlankInfo[i].fCX = 24.f;
		m_tBlankInfo[i].fCY = 24.f;
	}

	m_tBlankInfo[0].fX  = 475.f, m_tBlankInfo[0].fY  = 155.f;
	m_tBlankInfo[1].fX  = 475.f, m_tBlankInfo[1].fY  = 180.f;
	m_tBlankInfo[2].fX  = 475.f, m_tBlankInfo[2].fY  = 205.f;
	m_tBlankInfo[3].fX  = 475.f, m_tBlankInfo[3].fY  = 230.f;
	m_tBlankInfo[4].fX  = 475.f, m_tBlankInfo[4].fY  = 255.f;
	m_tBlankInfo[5].fX  = 475.f, m_tBlankInfo[5].fY  = 280.f;
	m_tBlankInfo[6].fX  = 475.f, m_tBlankInfo[6].fY  = 305.f;
	m_tBlankInfo[7].fX  = 475.f, m_tBlankInfo[7].fY  = 330.f;
	m_tBlankInfo[8].fX  = 475.f, m_tBlankInfo[8].fY  = 355.f;
	m_tBlankInfo[9].fX  = 475.f, m_tBlankInfo[9].fY  = 380.f;
	m_tBlankInfo[10].fX = 475.f, m_tBlankInfo[10].fY = 405.f;
	m_tBlankInfo[11].fX = 475.f, m_tBlankInfo[11].fY = 430.f;
	//m_tBlankInfo[12].fX = 475.f, m_tBlankInfo[12].fY = 455.f;

	m_iMaxSize = INVEN_SIZE;

	/*���� ��� ����� TextRect����.*/
	m_tSeedTextInfo.fCX = 70.f, m_tSeedTextInfo.fCY = 13.f;
	m_tSeedTextInfo.fX = 730.f, m_tSeedTextInfo.fY = 485.f;

	m_vecInven.reserve(INVEN_SIZE);

	m_pPlayer = CObjMgr::GetInstance()->GetPlayer();
}

void CInventory::LateInit()
{

}

int CInventory::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;

	/*Inventory On�� ������ ������ ����.*/
	if (m_bIsInvenOn)
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
				if (dwUpdateOldPos2 + 100 <= GetTickCount())
				{
					m_fOldXpos = (float)Curpt.x;
					m_fOldYpos = (float)Curpt.y;

					dwUpdateOldPos2 = GetTickCount();
				}
				/*Ŭ�� ���� �� x ��ǥ - ���� ��ǥ */
				float fCurXpos = (float)Curpt.x;
				float fCurYpos = (float)Curpt.y;

				/*���� ���콺 ��ǥ & ���� ���콺 ��ǥ ���� ���� �̵�. */
				m_fMoveXpos = fCurXpos - m_fOldXpos;
				m_fMoveYpos = fCurYpos - m_fOldYpos;

				m_tInfo.fX += m_fMoveXpos*0.4f, m_tInfo.fY += m_fMoveYpos*0.4f;

				m_tBlankInfo[0].fX  += m_fMoveXpos*0.4f, m_tBlankInfo[0].fY  += m_fMoveYpos*0.4f;
				m_tBlankInfo[1].fX  += m_fMoveXpos*0.4f, m_tBlankInfo[1].fY  += m_fMoveYpos*0.4f;
				m_tBlankInfo[2].fX  += m_fMoveXpos*0.4f, m_tBlankInfo[2].fY  += m_fMoveYpos*0.4f;
				m_tBlankInfo[3].fX  += m_fMoveXpos*0.4f, m_tBlankInfo[3].fY  += m_fMoveYpos*0.4f;
				m_tBlankInfo[4].fX  += m_fMoveXpos*0.4f, m_tBlankInfo[4].fY  += m_fMoveYpos*0.4f;
				m_tBlankInfo[5].fX  += m_fMoveXpos*0.4f, m_tBlankInfo[5].fY  += m_fMoveYpos*0.4f;
				m_tBlankInfo[6].fX  += m_fMoveXpos*0.4f, m_tBlankInfo[6].fY  += m_fMoveYpos*0.4f;
				m_tBlankInfo[7].fX  += m_fMoveXpos*0.4f, m_tBlankInfo[7].fY  += m_fMoveYpos*0.4f;
				m_tBlankInfo[8].fX  += m_fMoveXpos*0.4f, m_tBlankInfo[8].fY  += m_fMoveYpos*0.4f;
				m_tBlankInfo[9].fX  += m_fMoveXpos*0.4f, m_tBlankInfo[9].fY  += m_fMoveYpos*0.4f;
				m_tBlankInfo[10].fX += m_fMoveXpos*0.4f, m_tBlankInfo[10].fY += m_fMoveYpos*0.4f;
				m_tBlankInfo[11].fX += m_fMoveXpos*0.4f, m_tBlankInfo[11].fY += m_fMoveYpos*0.4f;
				//m_tBlankInfo[12].fX += m_fMoveXpos*0.4f, m_tBlankInfo[12].fY += m_fMoveYpos*0.4f;

				m_tSeedTextInfo.fX += m_fMoveXpos*0.4f, m_tSeedTextInfo.fY += m_fMoveYpos*0.4f;
			}

			/*������ ���� or �Ǹ�*/
			for (int i = 0; i < INVEN_SIZE; ++i)
			{
				if (PtInRect(&m_tBlank[i], Curpt))
				{
					if (KEY_DOWN(VK_LBUTTON))
					{
						/*������ ������ ��ġ�� Index �� ��ȯ.*/
						m_iSelect = i;

						if (m_bIsSellLstOn)
						{
							CSoundMgr::GetInstance()->PlaySound(L"Sell.wav", CSoundMgr::ITEM_SELL);

							/*���� â�� ���� �ִٸ� Ŭ���� ������ �Ǹ�.*/
							SellItem(m_iSelect);
						}
						else
						{
							CSoundMgr::GetInstance()->PlaySound(L"SwordEquip.mp3", CSoundMgr::ITEM_PUTON);

							/*���� â�� ���� �ִٸ� Ŭ���� ������ ����.*/
							/*index ���� Type���� �������ش�.*/
							if(m_iIsEmpty[m_iSelect] == 1)
								dynamic_cast<CEquipment*>(m_pEquipment)->PutOnItem(m_iSelect, dynamic_cast<CItem*>(m_vecInven[m_iSelect])->GetItemType());
						}
					}
				}
			}
		}
	}

	return NO_EVENT;
}

void CInventory::LateUpdate()
{
	if (m_bIsInvenOn)
	{
		/*������ ���� & ��� �� �� ���� ó��.*/
		if (m_vecInven.size() == 0)
		{
			m_iIsEmpty[0] = 0;
			return;
		}
	}
}

void CInventory::Render(HDC hDC)
{
	if (m_bIsInvenOn)
	{
		CObj::UpdateRect();
		UpdateBlankRect();
		UpdateTextRect();

		HDC hMemDC = nullptr;
		NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(L"Inven_Info"));

		Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
		GdiTransparentBlt(hDC, m_tRect.left, m_tRect.top, (int)m_tInfo.fCX, (int)m_tInfo.fCY,
			hMemDC, 0, 0, (int)m_tInfo.fCX, (int)m_tInfo.fCY, RGB(255, 0, 255));

		///*������ ��ġ�� �簢�� ���*/
		//for (int i = 0; i < INVEN_SIZE; ++i)
		//	Rectangle(hDC, m_tBlank[i].left, m_tBlank[i].top, m_tBlank[i].right, m_tBlank[i].bottom);


		/*�κ��丮 ���*/
		int i = 0;
		for (CObj*& pObj : m_vecInven)
		{
			NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(dynamic_cast<CItem*>(pObj)->GetImageKey()));
			GdiTransparentBlt(hDC, m_tBlank[i].left, m_tBlank[i].top, 24, 24, hMemDC, 0, 0, 24, 24, RGB(255, 0, 255));

			++i;
			if (i > INVEN_SIZE)
				break;	//������ size �Ѿ��Ż��.
		}

		/*������ text box*/
		SetBkMode(hDC, TRANSPARENT);
		TCHAR szBuf[32] = L"";

		int j = 0;
		for (CObj*& pObj : m_vecInven)
		{
			swprintf_s(szBuf, L"%s", pObj->GetInfo().szName.c_str());
			TextOut(hDC, m_tBlank[j].left+40, m_tBlank[j].top + 7, szBuf, lstrlen(szBuf));

			++j;
			if (j > INVEN_SIZE)
				break;	//������ size �Ѿ��Ż��.
		}


		/*���� ��差 Text���*/
		SetBkMode(hDC, TRANSPARENT);	//TextRect ����� ����
		TCHAR szBuf2[32] = L"";
		swprintf_s(szBuf2, L"%d", m_pPlayer->GetInfo().iGold);

		DrawText(hDC, szBuf2, lstrlen(szBuf2), &m_tSeedTextRect, DT_CENTER);
	}
}

void CInventory::Release()
{
	for_each(m_vecInven.begin(), m_vecInven.end(), SafeDelete<CObj*>);

	m_vecInven.clear();
}

void CInventory::UpdateBlankRect()
{
	for (int i = 0; i < INVEN_SIZE; ++i)
	{
		m_tBlank[i].left	= (LONG)(m_tBlankInfo[i].fX - m_tBlankInfo[i].fCX * 0.5f);
		m_tBlank[i].top		= (LONG)(m_tBlankInfo[i].fY - m_tBlankInfo[i].fCY * 0.5f);
		m_tBlank[i].right	= (LONG)(m_tBlankInfo[i].fX + m_tBlankInfo[i].fCX * 0.5f);
		m_tBlank[i].bottom	= (LONG)(m_tBlankInfo[i].fY + m_tBlankInfo[i].fCY * 0.5f);
	}
}

void CInventory::UpdateTextRect()
{
	m_tSeedTextRect.left   = (LONG)(m_tSeedTextInfo.fX - m_tSeedTextInfo.fCX * 0.5f);
	m_tSeedTextRect.top    = (LONG)(m_tSeedTextInfo.fY - m_tSeedTextInfo.fCY * 0.5f);
	m_tSeedTextRect.right  = (LONG)(m_tSeedTextInfo.fX + m_tSeedTextInfo.fCX * 0.5f);
	m_tSeedTextRect.bottom = (LONG)(m_tSeedTextInfo.fY + m_tSeedTextInfo.fCY * 0.5f);
}

void CInventory::BuyItem(CObj * pObj)
{
	/*�ݺ��� -> �κ��丮�� ������� �ִ��� ã�´�. m_iIsEmpty 0�̸� ����� 1�̸� �����.
	������ �Ѱܹ��� ��ü SafeDelete.*/
	for (int i = 0; i < INVEN_SIZE; ++i)
	{
		if (m_iIsEmpty[i] == 0)
		{
			m_iSelect = i;	//����� �ε��� ã��
			break;
		}
		else if (m_iIsEmpty[INVEN_SIZE - 1] == 1)
		{
			/* ó������ ��ȸ~������ index���� �� ������̸� -> ����� ������ return */
			SafeDelete(pObj);
			return;
		}
	}

	/*Player �� ����.*/
	m_pPlayer->SetMoney(pObj->GetInfo().iGold *-1);


	/*������� �ִٸ� �Ѱܹ��� ��ü Push Back & �ش� �ε��� m_iIsEmpty 1�� ����.*/
	m_vecInven.push_back(pObj);
	/*�� ���� index�� �簢�� rect ��ġ ����.*/
	pObj->SetPos(m_tBlankInfo[m_iSelect].fX, m_tBlankInfo[m_iSelect].fY);
	/*������ ��ġ ��� ������ ����.*/
	m_iIsEmpty[m_iSelect] = 1;
}

void CInventory::SellItem(int _index)
{
	/*������ index�� �� �����̸� ����.*/
	if (m_iIsEmpty[_index] == 0)
		return;

	/*�Ѱ� ���� index�� ������ erase ���ش�.*/
	m_pPlayer->SetMoney((int)(m_vecInven[_index]->GetInfo().iGold *0.5));	//���� ��������

																	/*������ ����*/
	SafeDelete(m_vecInven[_index]);

	vector<CObj*>::iterator iter_begin = m_vecInven.begin();
	m_vecInven.erase(iter_begin + _index);

	/*�Ǹ��� ���� ��������� ��ȯ*/
	//m_iIsEmpty[_index] = 0;

	if (m_vecInven.size() == 0)
	{
		m_iIsEmpty[0] = 0;
		return;
	}
	for (size_t i = 0; i < m_vecInven.size() - 1; ++i)
		m_iIsEmpty[i] = 1;

	for (size_t j = m_vecInven.size(); j < INVEN_SIZE; ++j)
		m_iIsEmpty[j] = 0;
}

void CInventory::EraseItem(int _index)
{
	vector<CObj*>::iterator iter = m_vecInven.begin();

	SafeDelete(m_vecInven[_index]);

	m_vecInven.erase(iter + _index);

	/*������ ���� & ��� �� �� ���� ó��.*/
	if (m_vecInven.size() == 0)
	{
		m_iIsEmpty[0] = 0;
		return;
	}
	/*�� �� ó�� ���ش�.*/
	for (size_t i = 0; i < m_vecInven.size() - 1; ++i)
		m_iIsEmpty[i] = 1;

	for (size_t j = m_vecInven.size(); j < INVEN_SIZE; ++j)
		m_iIsEmpty[j] = 0;
}
