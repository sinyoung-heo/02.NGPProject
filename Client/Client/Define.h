#pragma once

#define WINCX 800
#define WINCY 600

#define DEAD_OBJ 1
#define NO_EVENT 0

#define PI 3.141592f
#define GRAVITY 9.8f

#define NULL_CHECK(PTR)	if(nullptr == (PTR)) {return;}

#define TILECX 30
#define TILECY 30

#define TILEX 130
#define TILEY  68



#define TOWNCX 1600
#define TOWNCY 1200

#define STORECX 1277
#define STORECY 991

#define FIELDCX 3180
#define FIELDCY 1687

#define DUNGEONCX 2500
#define DUNGEONCY 1304

#define BOSSCX 3900
#define BOSSCY 2050



#define DECLARE_SINGLETON(ClassName)		\
public:										\
	static ClassName* GetInstance()			\
	{										\
		if(nullptr == m_pInstance)			\
			m_pInstance = new ClassName;	\
											\
		return m_pInstance;					\
	}										\
											\
	void DestroyInstance()					\
	{										\
		if(m_pInstance)						\
		{									\
			delete m_pInstance;				\
			m_pInstance = nullptr;			\
		}									\
	}										\
											\
private:									\
	static ClassName*	m_pInstance;		

#define IMPLEMENT_SINGLETON(ClassName)		\
ClassName*	ClassName::m_pInstance = nullptr;