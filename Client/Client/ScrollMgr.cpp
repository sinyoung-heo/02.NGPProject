#include "stdafx.h"
#include "ScrollMgr.h"

float CScrollMgr::m_fScrollX = 0.f;
float CScrollMgr::m_fScrollY = 0.f;

CScrollMgr::CScrollMgr()
{
}

CScrollMgr::~CScrollMgr()
{
}

void CScrollMgr::ScrollRockTown()
{
	if (m_fScrollX > 0.f)
		m_fScrollX = 0.f;

	if (m_fScrollY > 0.f)
		m_fScrollY = 0.f;


	if (m_fScrollX < WINCX - TOWNCX)
		m_fScrollX = float(WINCX - TOWNCX);

	if (m_fScrollY < WINCY - TOWNCY)
		m_fScrollY = float(WINCY - TOWNCY);
}

void CScrollMgr::ScrollRockStore()
{
	if (m_fScrollX > 0.f)
		m_fScrollX = 0.f;

	if (m_fScrollY > 0.f)
		m_fScrollY = 0.f;


	if (m_fScrollX < WINCX - STORECX)
		m_fScrollX = float(WINCX - STORECX);

	if (m_fScrollY < WINCY - STORECY)
		m_fScrollY = float(WINCY - STORECY);
}

void CScrollMgr::ScrollRockField()
{
	if (m_fScrollX > 0.f)
		m_fScrollX = 0.f;

	if (m_fScrollY > 0.f)
		m_fScrollY = 0.f;


	if (m_fScrollX < WINCX - FIELDCX)
		m_fScrollX = float(WINCX - FIELDCX);

	if (m_fScrollY < WINCY - FIELDCY)
		m_fScrollY = float(WINCY - FIELDCY);
}

void CScrollMgr::ScrollRockDungeon()
{
	if (m_fScrollX > 0.f)
		m_fScrollX = 0.f;

	if (m_fScrollY > 0.f)
		m_fScrollY = 0.f;


	if (m_fScrollX < WINCX - DUNGEONCX)
		m_fScrollX = float(WINCX - DUNGEONCX);

	if (m_fScrollY < WINCY - DUNGEONCY)
		m_fScrollY = float(WINCY - DUNGEONCY);
}

void CScrollMgr::ScrollRockBoss()
{
	if (m_fScrollX > 0.f)
		m_fScrollX = 0.f;

	if (m_fScrollY > 0.f)
		m_fScrollY = 0.f;


	if (m_fScrollX < WINCX - BOSSCX)
		m_fScrollX = float(WINCX - BOSSCX);

	if (m_fScrollY < WINCY - BOSSCY)
		m_fScrollY = float(WINCY - BOSSCY);
}
