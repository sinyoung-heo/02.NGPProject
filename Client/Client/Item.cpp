#include "stdafx.h"
#include "Item.h"


CItem::CItem()
	:m_eItemType(ItemType::TYPE_END)
{
}

CItem::CItem(const INFO & rInfo)
	:CObj(rInfo)
{
}

CItem::~CItem()
{
	Release();
}

void CItem::Initialize()
{
	m_tInfo.fCX = 24.f;
	m_tInfo.fCY = 24.f;
}

void CItem::LateInit()
{

}

int CItem::Update()
{
	return 0;
}

void CItem::LateUpdate()
{

}

void CItem::Render(HDC hDC)
{

}

void CItem::Release()
{

}
