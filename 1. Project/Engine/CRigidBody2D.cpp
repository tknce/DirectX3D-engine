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

	// �ӵ��� ���ӵ��� ���Ѵ�.
	m_vVelocity += m_vAccel * DT;

	// ���� �پ�������
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
	

	// �߷� ���ӵ� ����
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


	// �ӵ� ���� ����
	Vec2 vGravity = m_vGravityAccel;

	// �߷��� ������ �Ѿ��.
	if (!m_vGravityAccel.IsZero())
	{
		vGravity.Normalize();
		vGravity = vGravity * m_vVelocity.Dot(vGravity); // �ӵ��� �߷¹������� ����
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

	//������ ���� (����� ���� ����, �ӵ��� 0�� �ƴҶ�)
	if (!m_vVelocity.IsZero())
	{
		// �ӵ��� �ݴ�������� �������� ����
		Vec2 vFriction = -m_vVelocity;
	
		vFriction = vFriction.MyNormalize() * m_fFriction * DT * m_fMass;

		// ���������� ���� �ӵ� ���ҷ��� ���� �ӵ����� �� ū ���
		if (m_vVelocity.Length() < vFriction.Length())
		{
			// �ӵ��� 0���� �����
			m_vVelocity = Vec2(0.f, 0.f);
		}
		else
		{
			// �ӵ����� ���������� ���� �ݴ�������� �ӵ��� �����Ѵ�.
			m_vVelocity += vFriction;
		}

	}

	// �ӵ��� �°� ��ü�� �̵���Ų��.
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