#pragma once
#include "Obj.h"
class CTile :
	public CObj
{
public:
	CTile();
	virtual ~CTile();

public:
	int GetDrawID() { return m_iDrawID; }

public:
	void SetDrawID(int iDrawID) { m_iDrawID = iDrawID; }

public:
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void LateInit();
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;


private:
	int m_iDrawID;

};

