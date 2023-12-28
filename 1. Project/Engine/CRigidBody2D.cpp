#include "pch.h"
#include "CRigidBody2D.h"

#include "CTimeMgr.h"

#include "CTransform.h"



CRigidBody2D::CRigidBody2D()
	: CComponent(COMPONENT_TYPE::RIGIDBODY2D)
	, m_vForce{ 0,0 }
	, m_vAccel{0,0}
	, m_vVelocity{ 0,0 }
	, m_fMass()
	, m_fFriction()
	, m_fMaxSpeed()
	, m_fMaxGravitySpeed()
	, m_vGravityAccel{ 0,1500 }
	, m_bUseGravity()
	, m_bGround(false)
	, m_bWall(false)
	, m_bstair(false)
{
}

CRigidBody2D::~CRigidBody2D()
{
}

void CRigidBody2D::finaltick()
{
	// F = M x A
	if (m_vForce == Vec2(0, 0))
	{
		m_vAccel = Vec2(0,0);
	}
	else
	{
		m_vAccel = m_vForce / m_fMass;
	}

	// 속도에 가속도를 더한다.
	m_vVelocity += m_vAccel * DT;

	// 벽에 붙어있을시
	if (m_bWall)
	{
		m_vVelocity = Vec2(0.f, m_vVelocity.y);
		m_vVelocity.x += m_vGravityAccel.x * DT;
		m_vVelocity.y -= (m_vGravityAccel.y - 1490)  * DT;
	}

	if (m_bWall_nomal)
	{
		m_vVelocity.x = 0;
	}
	

	// 중력 가속도 적용
	if (!m_bGround )
	{
		m_vVelocity.x += m_vGravityAccel.x * DT;
		m_vVelocity.y -= m_vGravityAccel.y * DT;
	}
	else
	{
		Vec2 vGravity = m_vGravityAccel;
		vGravity.Normalize();
		m_vVelocity -= vGravity * m_vVelocity.Dot(vGravity);
	}
	if (m_bstair)
	{
		Vec2 vGravity = m_vGravityAccel;
		vGravity.Normalize();
		m_vVelocity -= vGravity * m_vVelocity.Dot(vGravity);
	}


	// 속도 제한 조건
	Vec2 vGravity = m_vGravityAccel;

	// 중력이 없으면 넘어간다.
	if (!m_vGravityAccel.IsZero())
	{
		vGravity.Normalize();
		vGravity = vGravity * m_vVelocity.Dot(vGravity); // 속도를 중력방향으로 분해
	}


	Vec2 vSideVelocity = m_vVelocity - vGravity;

	if (m_fMaxGravitySpeed < vGravity.Length())
	{
		vSideVelocity.Normalize();
		vSideVelocity *= m_fMaxGravitySpeed;
	}


	if (m_fMaxSpeed < vSideVelocity.Length())
	{
		vSideVelocity.Normalize();
		vSideVelocity *= m_fMaxSpeed;
	}

	m_vVelocity.x = vGravity.x + vSideVelocity.x;
	m_vVelocity.y = vGravity.y + vSideVelocity.y;

	//마찰력 조건 (적용된 힘이 없고, 속도가 0이 아닐때)
	if (!m_vVelocity.IsZero())
	{
		// 속도의 반대방향으로 마찰력을 적용
		Vec2 vFriction = -m_vVelocity;
	
		vFriction = vFriction.MyNormalize() * m_fFriction * DT * m_fMass;

		// 마찰력으로 인한 속도 감소량이 현재 속도보다 더 큰 경우
		if (m_vVelocity.Length() < vFriction.Length())
		{
			// 속도를 0으로 만든다
			m_vVelocity = Vec2(0.f, 0.f);
		}
		else
		{
			// 속도에서 마찰력으로 인한 반대방향으로 속도를 차감한다.
			m_vVelocity += vFriction;
		}

	}

	// 속도에 맞게 물체를 이동시킨다.
	Vec2 vPos = GetOwner()->Transform()->GetRelativePos() + m_vVelocity * DT;
	GetOwner()->Transform()->SetRelativePos(vPos.x,vPos.y,0.f);

	m_vForce = Vec2(0.f, 0.f);

	Vec2 Accel = Vec2(100, 800);
	Vec2 gravity = Vec2(0, 1500);

	static Vec2 velocity = Vec2(0, -1);

	Vec2 Gravity = gravity;
	Gravity.Normalize();
	Gravity = Gravity * velocity.Dot(Gravity);

	velocity += Accel;
	velocity.x += Gravity.x;
	velocity.y -= Gravity.y;

	int a = 0;

}
void CRigidBody2D::SaveToFile(FILE* _File)
{
	COMPONENT_TYPE type = GetType();
	fwrite(&type, sizeof(UINT), 1, _File);

	fwrite(&m_fMass, sizeof(float), 1, _File);
	fwrite(&m_fFriction, sizeof(float), 1, _File);
	fwrite(&m_fMaxSpeed, sizeof(float), 1, _File);
	fwrite(&m_fMaxGravitySpeed, sizeof(float), 1, _File);
}

void CRigidBody2D::LoadFromFile(FILE* _File)
{
	fread(&m_fMass, sizeof(float), 1, _File);
	fread(&m_fFriction, sizeof(float), 1, _File);
	fread(&m_fMaxSpeed, sizeof(float), 1, _File);
	fread(&m_fMaxGravitySpeed, sizeof(float), 1, _File);
}