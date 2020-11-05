#pragma once
class CObj
{
public:
	CObj();
	CObj(const INFO & rInfo);
	virtual ~CObj();

public:
	const INFO& GetInfo() const { return m_tInfo; }
	const RECT& GetRect() const { return m_tRect; }
	const COLLINFO&  GetColInfo() const { return m_tColInfo; }
	const RECT&		 GetColRect() const { return m_tColRect; }
	float GetAttackXPos() { return m_fAttackX; }
	float GetAttackYPos() { return m_fAttackY; }
	Layer::ID GetLayer() { return m_eLayer; }

public:
	void SetInfo(const INFO& rInfo) 
	{ 
		//if(rInfo.szName.data() != nullptr)
			m_tInfo = rInfo;
	}
	void SetDmg(int _dmg) { m_tInfo.iAtt = _dmg; }
	void SetSize(float _cx, float _cy) { m_tInfo.fCX = _cx, m_tInfo.fCY = _cy; }
	void SetPos(float _x, float _y) { m_tInfo.fX = _x, m_tInfo.fY = _y; }
	void SetColPos(float _x, float _y) { m_tColInfo.fX = _x, m_tColInfo.fY = _y; }
	void SetAngle(float _angle) { m_fAngle = _angle; }

	void SetHp(int _dmg) { m_tInfo.iHp -= _dmg; }
	void SetExp(int _exp) { m_tInfo.iExp += _exp; }

	void SetMoney(int _money) { m_tInfo.iGold += _money; }
	void SetMaxHp(int _maxhp) { m_tInfo.iMaxHp += _maxhp; }
	void SetMaxMp(int _maxmp) { m_tInfo.iMaxMp += _maxmp; }
	void SetMaxSp(int _maxsp) { m_tInfo.iMaxSp += _maxsp; }
	void SetAtt(int _att)  { m_tInfo.iAtt += _att; }

	/*�浹 �о�� ��ǥ����*/
	void AddSetXpos(float _x) { m_tInfo.fX += _x; }
	void AddSetYpos(float _y) { m_tInfo.fY += _y; }
	void AddSetColInfoX(float _x) { m_tColInfo.fX += _x; }
	void AddSetColInfoY(float _y) { m_tColInfo.fY += _y; }

public:
	virtual void Initialize() = 0;
	virtual void LateInit();
	virtual int  Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Render(HDC hDC) = 0;
	virtual void Release() = 0;

public:
	void UpdateRect();
	void UpdateColRect();
	void UpdateColXYPos();

	void bIsDead() { m_bIsDead = true; }

	void MoveFrame();

protected:
	INFO m_tInfo;
	RECT m_tRect;	//�̹��� �ڽ�

	/*�浹 �ڽ�*/
	COLLINFO m_tColInfo;
	RECT	 m_tColRect;

	/*�̹��� ������ ����*/
	FRAME m_tFrame;

	/*�̵� �ӵ�*/
	float m_fSpeed;

	/*���� ����*/
	float m_fAngle;
	/*���� Object ���� ��ġ.*/
	float m_fAttackX;
	float m_fAttackY;

	bool m_bLateInit;
	bool m_bIsDead;

	
	Layer::ID m_eLayer;

};
