#include "pch.h"
#include "CKeyMgr.h"

#include "CEngine.h"
#include "CDevice.h"
#include "CCamera.h"
#include "CRenderMgr.h"
#include "CTransform.h"

int vkarr[(UINT)KEY::END]
=
{
	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',
	'A',
	'S',
	'D',
	'F',
	'G',
	'H',
	'J',
	'K',
	'L',
	'Z',
	'X',
	'C',
	'V',
	'B',
	'N',
	'M',

	VK_RETURN,
	VK_ESCAPE,
	VK_LSHIFT,
	VK_LMENU,
	VK_LCONTROL,
	VK_SPACE,

	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,

	VK_LBUTTON,
	VK_RBUTTON,

	VK_NUMPAD0,
	VK_NUMPAD1,
	VK_NUMPAD2,
	VK_NUMPAD3,
	VK_NUMPAD4,
	VK_NUMPAD5,
	VK_NUMPAD6,
	VK_NUMPAD7,
	VK_NUMPAD8,
	VK_NUMPAD9,

	'0',
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
};


CKeyMgr::CKeyMgr()
{

}

CKeyMgr::~CKeyMgr()
{

}




void CKeyMgr::init()
{
	for (UINT i = 0; i < (UINT)KEY::END; ++i)
	{
		m_vecKey.push_back(tKeyInfo{ (KEY)i, KEY_STATE::NONE, false });
	}
	int a = 'Q';
}

void CKeyMgr::tick()
{
	if (GetFocus())
	{
		for (UINT i = 0; i < (UINT)KEY::END; ++i)
		{
			// �ش�Ű�� ���� �����ִ�.
			if (GetAsyncKeyState(vkarr[i]) & 0x8000)
			{
				// ���� �����ӿ��� ���� �־���.
				if (m_vecKey[i].bPrevPress)
				{
					m_vecKey[i].eState = KEY_STATE::PRESS;
				}
				else
				{
					m_vecKey[i].eState = KEY_STATE::TAP;
				}

				m_vecKey[i].bPrevPress = true;
			}

			// �ش�Ű�� ���� �ȴ����ִ�.
			else
			{
				// ���� �����ӿ��� ���� �־���.
				if (m_vecKey[i].bPrevPress)
				{
					m_vecKey[i].eState = KEY_STATE::RELEASE;
				}

				// ���� �����ӿ��� �ȴ��� �־���.
				else
				{
					m_vecKey[i].eState = KEY_STATE::NONE;
				}

				m_vecKey[i].bPrevPress = false;
			}
		}

		m_vPrevMousePos = m_vMousePos;

		POINT ptMouse = {};
		GetCursorPos(&ptMouse);
		ScreenToClient(CEngine::GetInst()->GetMainHwnd(), &ptMouse);
		m_vMousePos.x = (float)ptMouse.x;
		m_vMousePos.y = (float)ptMouse.y;
		Vec2 Resolution = CDevice::GetInst()->GetRenderResolution();
		m_vCameraMouse.x = m_vMousePos.x - (Resolution.x / 2);
		m_vCameraMouse.y = m_vMousePos.y - (Resolution.y / 2);
		m_vCameraMouse.y = valueReverse(m_vCameraMouse.y);
		
		CCamera* Camera = CRenderMgr::GetInst()->GetMainCam();
		if (Camera != nullptr)
		{
			Vec3 CameraPos = Camera->GetOwner()->Transform()->GetRelativePos();
			m_vCameraMouse.x += CameraPos.x;
			m_vCameraMouse.y += CameraPos.y;
			float CamScale = Camera->GetOrthographicScale();
			m_vCameraMouse *= CamScale;
		}
		


		// ���� ���콺 ��ġ�� ���� ���콺 ��ġ�� ���
		m_vMouseDir = Vec2(m_vMousePos.x - m_vPrevMousePos.x, m_vMousePos.y - m_vPrevMousePos.y);
		m_vMouseDir.y *= -1.f;

		/*if (!(m_vMouseDir.x == 0.f && m_vMouseDir.y == 0.f))
		{
			m_vMouseDir.Normalize();
		}		*/
	}

	else
	{
		for (UINT i = 0; i < (UINT)KEY::END; ++i)
		{
			if (KEY_STATE::TAP == m_vecKey[i].eState || KEY_STATE::PRESS == m_vecKey[i].eState)
				m_vecKey[i].eState = KEY_STATE::RELEASE;
			else if (KEY_STATE::RELEASE == m_vecKey[i].eState)
				m_vecKey[i].eState = KEY_STATE::NONE;

			m_vecKey[i].bPrevPress = false;
		}
	}
}
