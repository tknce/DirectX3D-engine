#pragma once


// Ű �Է� ���� ����
// 1. Ű �Է� ����� ���ϰ�
// 2. Ű �̺�Ʈ üũ ����(���� ����, ���� ����)
// 3. ������ ����ȭ
// 4. ���μ��� ����ȭ( ���μ����� ��Ŀ���� �ƴ� �� �Է� ����)

enum class KEY
{
	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,

	A,
	S,
	D,
	F,
	G,
	H,
	J,
	K,
	L,

	Z,
	X,
	C,
	V,
	B,
	N,
	M,

	ENTER,
	ESC,
	LSHIFT,
	LALT,
	LCTRL,
	SPACE,

	LEFT,
	RIGHT,
	UP,
	DOWN,

	LBTN,
	RBTN,

	NUM_0,
	NUM_1,
	NUM_2,
	NUM_3,
	NUM_4,
	NUM_5,
	NUM_6,
	NUM_7,
	NUM_8,
	NUM_9,

	N_0,
	N_1,
	N_2,
	N_3,
	N_4,
	N_5,
	N_6,
	N_7,
	N_8,
	N_9,


	END,
};



enum class KEY_STATE
{
	TAP,
	PRESS,
	RELEASE,
	NONE,
};


struct tKeyInfo
{
	KEY  eType;
	KEY_STATE eState;
	bool	  bPrevPress;
};


class CKeyMgr
	: public CSingleton<CKeyMgr>
{	
private:
	vector<tKeyInfo>	m_vecKey;
	Vec2				m_vMousePos;
	Vec2				m_vPrevMousePos;
	Vec2				m_vMouseDir;
	Vec2				m_vCameraMouse;


public:
	void init();
	void tick();

public:
	KEY_STATE GetKeyState(KEY _key)
	{
		return m_vecKey[(UINT)_key].eState;
	}

	Vec2 GetMousePos() { return m_vMousePos; }
	Vec2 GetMouseDir() { return m_vMouseDir; }
	Vec2 GetCameraMousePos() { return m_vCameraMouse; }

public:
	CKeyMgr();
	~CKeyMgr();
};

