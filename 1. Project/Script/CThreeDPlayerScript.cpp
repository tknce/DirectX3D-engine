#include "pch.h"
#include "CThreeDPlayerScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine\CTimeMgr.h>
CThreeDPlayerScript::CThreeDPlayerScript()
	: CScript(THREEDPLAYERSCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Player MoveSpeed", &m_fspeed);
}

CThreeDPlayerScript::~CThreeDPlayerScript()
{
}


void CThreeDPlayerScript::begin()
{
	m_pAnimationObj = GetOwner()->GetChildwstring(L"map");
	m_fspeed = 400.f;
}

void CThreeDPlayerScript::tick()
{
	Move();
}

void CThreeDPlayerScript::Move()
{
	// 최상위 오브젝트임으로 하위 animation담당 오브젝트를 찾는다
	// 애니메이션이 엎어져있어서 -90도 x축 방향을 올렸으므로 up방향이 앞임
	// 방향을 찾고 Get Set을 해준다.
	Vec3 vFront = m_pAnimationObj->Transform()->GetRelativeDir(DIR::UP);
	Vec3 vRight = m_pAnimationObj->Transform()->GetRelativeDir(DIR::RIGHT);
	Vec3 vUp	= m_pAnimationObj->Transform()->GetRelativeDir(DIR::FRONT);
	vRight = Vec3(vRight.x, vRight.y, -vRight.z);
	vFront = Vec3(vFront.x, vFront.y, -vFront.z);
	vUp = Vec3(vUp.x, vUp.y, -vUp.z);
	m_vAxis = GetOwner()->Transform()->GetRelativePos();
	// 이동
	//if (KEY_PRESSED(KEY::W))
	//	m_vAxis += DT * -vFront	* m_fspeed;
	//if (KEY_PRESSED(KEY::S))	
	//	m_vAxis += DT * vFront * m_fspeed;
	//if (KEY_PRESSED(KEY::A))	
	//	m_vAxis += DT * -vRight * m_fspeed;
	//if (KEY_PRESSED(KEY::D))	
	//	m_vAxis += DT * vRight	* m_fspeed;

	//GetOwner()->RigidBody3D()->AddControllVelocity(Vec3(DT * -vFront * m_fspeed));
	if (KEY_PRESSED(KEY::W))
		GetOwner()->RigidBody3D()->AddControllVelocity(DT * -vFront * m_fspeed);
	if (KEY_PRESSED(KEY::S))
		GetOwner()->RigidBody3D()->AddControllVelocity(DT * vFront * m_fspeed);
	if (KEY_PRESSED(KEY::A))
		GetOwner()->RigidBody3D()->AddControllVelocity(DT * -vRight * m_fspeed);
	if (KEY_PRESSED(KEY::D))
		GetOwner()->RigidBody3D()->AddControllVelocity(DT * vRight * m_fspeed);
	// 점프
	if (KEY_TAP(KEY::SPACE))
	{
		float a = Lerp(1.f, m_fspeed, DT);
		
		GetOwner()->RigidBody3D()->AddControllVelocity(Vec3(0.f, 2000.f, 0.f));
		m_vStart = m_vAxis;
		m_vEnd = Vec3(m_vAxis.x, m_vAxis.y + m_fspeed, m_vAxis.z);
		//m_isJump = true;
	}
		
	if (m_isJump)
		Jump();
	

	GetOwner()->Transform()->SetRelativePos(m_vAxis);


}

void CThreeDPlayerScript::Jump()
{
	float dis = Vec3::Distance(m_vAxis, m_vEnd);
	if (Vec3::Distance(m_vAxis, m_vEnd) >= 30.f)
	{
		m_vAxis = m_vAxis.Lerp(m_vAxis, m_vEnd, DT);
		GetOwner()->RigidBody3D()->AddControllVelocity(Vec3(0.f, dis, 0.f));
	}
		
	else
		m_isJump = false;
}
