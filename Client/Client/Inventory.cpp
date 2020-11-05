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

	/*보유 골드 출력할 TextRect정보.*/
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

	/*Inventory On인 상태일 때에만 실행.*/
	if (m_bIsInvenOn)
	{
		POINT Curpt = {};
		GetCursorPos(&Curpt);
		ScreenToClient(g_hWnd, &Curpt);

		//float fScrollX = CScrollMgr::GetScrollX();
		//float fScrollY = CScrollMgr::GetScrollY();

		//cout << Curpt.x /*- fScrollX */<< "  " << Curpt.y /*- fScrollY*/ << endl;

		/*Inventory 이동*/
		if (PtInRect(&m_tRect, Curpt))
		{
			if (KEY_PRESSING(VK_LBUTTON))
			{
				/* 주기적으로 old 좌푝 갱신 */
				if (dwUpdateOldPos2 + 100 <= GetTickCount())
				{
					m_fOldXpos = (float)Curpt.x;
					m_fOldYpos = (float)Curpt.y;

					dwUpdateOldPos2 = GetTickCount();
				}
				/*클릭 중일 때 x 좌표 - 현재 좌표 */
				float fCurXpos = (float)Curpt.x;
				float fCurYpos = (float)Curpt.y;

				/*현재 마우스 좌표 & 이전 마우스 좌표 차로 도형 이동. */
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

			/*아이템 장착 or 판매*/
			for (int i = 0; i < INVEN_SIZE; ++i)
			{
				if (PtInRect(&m_tBlank[i], Curpt))
				{
					if (KEY_DOWN(VK_LBUTTON))
					{
						/*선택한 아이템 위치의 Index 값 반환.*/
						m_iSelect = i;

						if (m_bIsSellLstOn)
						{
							CSoundMgr::GetInstance()->PlaySound(L"Sell.wav", CSoundMgr::ITEM_SELL);

							/*상점 창이 켜져 있다면 클릭한 아이템 판매.*/
							SellItem(m_iSelect);
						}
						else
						{
							CSoundMgr::GetInstance()->PlaySound(L"SwordEquip.mp3", CSoundMgr::ITEM_PUTON);

							/*상점 창이 꺼져 있다면 클릭한 아이템 장착.*/
							/*index 값과 Type값을 전달해준다.*/
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
		/*아이템 장착 & 사고 팔 때 예외 처리.*/
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

		///*아이템 위치에 사각형 출력*/
		//for (int i = 0; i < INVEN_SIZE; ++i)
		//	Rectangle(hDC, m_tBlank[i].left, m_tBlank[i].top, m_tBlank[i].right, m_tBlank[i].bottom);


		/*인벤토리 출력*/
		int i = 0;
		for (CObj*& pObj : m_vecInven)
		{
			NULL_CHECK(hMemDC = CBmpMgr::GetInstance()->FindImage(dynamic_cast<CItem*>(pObj)->GetImageKey()));
			GdiTransparentBlt(hDC, m_tBlank[i].left, m_tBlank[i].top, 24, 24, hMemDC, 0, 0, 24, 24, RGB(255, 0, 255));

			++i;
			if (i > INVEN_SIZE)
				break;	//아이템 size 넘어가면탈출.
		}

		/*아이템 text box*/
		SetBkMode(hDC, TRANSPARENT);
		TCHAR szBuf[32] = L"";

		int j = 0;
		for (CObj*& pObj : m_vecInven)
		{
			swprintf_s(szBuf, L"%s", pObj->GetInfo().szName.c_str());
			TextOut(hDC, m_tBlank[j].left+40, m_tBlank[j].top + 7, szBuf, lstrlen(szBuf));

			++j;
			if (j > INVEN_SIZE)
				break;	//아이템 size 넘어가면탈출.
		}


		/*보유 골드량 Text출력*/
		SetBkMode(hDC, TRANSPARENT);	//TextRect 흰바탕 제거
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
	/*반복문 -> 인벤토리에 빈공간이 있는지 찾는다. m_iIsEmpty 0이면 빈공간 1이면 사용중.
	없으면 넘겨받은 객체 SafeDelete.*/
	for (int i = 0; i < INVEN_SIZE; ++i)
	{
		if (m_iIsEmpty[i] == 0)
		{
			m_iSelect = i;	//빈공간 인덱스 찾고
			break;
		}
		else if (m_iIsEmpty[INVEN_SIZE - 1] == 1)
		{
			/* 처음부터 순회~마지막 index까지 다 사용중이면 -> 빈공간 없으면 return */
			SafeDelete(pObj);
			return;
		}
	}

	/*Player 돈 차감.*/
	m_pPlayer->SetMoney(pObj->GetInfo().iGold *-1);


	/*빈공간이 있다면 넘겨받은 객체 Push Back & 해당 인덱스 m_iIsEmpty 1로 변경.*/
	m_vecInven.push_back(pObj);
	/*빈 공간 index의 사각형 rect 위치 대입.*/
	pObj->SetPos(m_tBlankInfo[m_iSelect].fX, m_tBlankInfo[m_iSelect].fY);
	/*아이템 위치 사용 중으로 변경.*/
	m_iIsEmpty[m_iSelect] = 1;
}

void CInventory::SellItem(int _index)
{
	/*선택한 index가 빈 공간이면 종료.*/
	if (m_iIsEmpty[_index] == 0)
		return;

	/*넘겨 받은 index의 아이템 erase 해준다.*/
	m_pPlayer->SetMoney((int)(m_vecInven[_index]->GetInfo().iGold *0.5));	//절반 가격으로

																	/*아이템 삭제*/
	SafeDelete(m_vecInven[_index]);

	vector<CObj*>::iterator iter_begin = m_vecInven.begin();
	m_vecInven.erase(iter_begin + _index);

	/*판매한 공간 빈공간으로 변환*/
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

	/*아이템 장착 & 사고 팔 때 예외 처리.*/
	if (m_vecInven.size() == 0)
	{
		m_iIsEmpty[0] = 0;
		return;
	}
	/*빈 곳 처리 해준다.*/
	for (size_t i = 0; i < m_vecInven.size() - 1; ++i)
		m_iIsEmpty[i] = 1;

	for (size_t j = m_vecInven.size(); j < INVEN_SIZE; ++j)
		m_iIsEmpty[j] = 0;
}
