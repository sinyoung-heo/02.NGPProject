#pragma once
class CScrollMgr
{
public:
	CScrollMgr();
	~CScrollMgr();


public:
	static float GetScrollX() { return m_fScrollX; }
	static float GetScrollY() { return m_fScrollY; }

public:
	static void SetScrollX(float _x) { m_fScrollX += _x; }
	static void SetScrollY(float _y) { m_fScrollY += _y; }

public:
	static void ScrollRockTown();
	static void ScrollRockStore();
	static void ScrollRockField();
	static void ScrollRockDungeon();
	static void ScrollRockBoss();

private:
	static float m_fScrollX;
	static float m_fScrollY;
};

