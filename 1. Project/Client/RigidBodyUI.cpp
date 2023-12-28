#include "pch.h"
#include "RigidBodyUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CRigidBody2D.h>

RigidBodyUI::RigidBodyUI()
	: ComponentUI("RigidBody2D", COMPONENT_TYPE::RIGIDBODY2D)
	,m_vForce()
	,m_vAccel()
	,m_vVelocity()
	,m_fMass()
	,m_fFriction()
	,m_fMaxSpeed()
	,m_fMaxGravitySpeed()
	,m_vGravityAccel()
	,m_bUseGravity()
{
}

RigidBodyUI::~RigidBodyUI()
{
}

void RigidBodyUI::update()
{
	if (nullptr != GetTarget())
	{
		//GetTarget()->RigidBody2D()->
		if (GetTarget()->ComponentCheck(COMPONENT_TYPE::RIGIDBODY2D))
		{
			m_vForce = GetTarget()->RigidBody2D()->GetForce();
			m_vAccel = GetTarget()->RigidBody2D()->GetAccel();
			m_vVelocity = GetTarget()->RigidBody2D()->GetVelocity();
			m_fMass = GetTarget()->RigidBody2D()->GetMass();
			m_fFriction = GetTarget()->RigidBody2D()->GetFriction();
			m_fMaxSpeed = GetTarget()->RigidBody2D()->GetMaxSpeed();
			m_fMaxGravitySpeed = GetTarget()->RigidBody2D()->GetMaxGravitySpeed();
			m_vGravityAccel = GetTarget()->RigidBody2D()->GetGravityAccel();
			m_bUseGravity = GetTarget()->RigidBody2D()->GetUseGravity();
		}
	}

	ComponentUI::update();
}

void RigidBodyUI::render_update()
{
	ComponentUI::render_update();
	if (GetTarget()->ComponentCheck(COMPONENT_TYPE::RIGIDBODY2D))
	{
		ImGui::Text("Force			"); ImGui::SameLine(); ImGui::InputFloat2("##Force", m_vForce);
		ImGui::Text("Accel			"); ImGui::SameLine(); ImGui::InputFloat2("##Accel", m_vAccel);
		ImGui::Text("Velocity		"); ImGui::SameLine(); ImGui::InputFloat2("##Velocity", m_vVelocity);
		ImGui::Text("Mass			"); ImGui::SameLine(); ImGui::InputFloat("##Mass", &m_fMass);
		ImGui::Text("Friction		"); ImGui::SameLine(); ImGui::InputFloat("##Friction", &m_fFriction);
		ImGui::Text("MaxSpeed		"); ImGui::SameLine(); ImGui::InputFloat("##MaxSpeed", &m_fMaxSpeed);
		ImGui::Text("MaxGravitySpeed	"); ImGui::SameLine(); ImGui::InputFloat("##MaxGravitySpeed", &m_fMaxGravitySpeed);
		ImGui::Text("GravityAccel	"); ImGui::SameLine(); ImGui::InputFloat2("##GravityAccel", m_vGravityAccel);
		ImGui::Text("UseGravity		"); ImGui::SameLine(); ImGui::Checkbox("##UseGravity", &m_bUseGravity);

		if (nullptr != GetTarget())
		{
			GetTarget()->RigidBody2D()->SetVelocity(m_vVelocity);
			GetTarget()->RigidBody2D()->SetMass(m_fMass);
			GetTarget()->RigidBody2D()->SetFriction(m_fFriction);
			GetTarget()->RigidBody2D()->SetMaxSpeed(m_fMaxSpeed);
			GetTarget()->RigidBody2D()->SetMaxGravitySpeed(m_fMaxGravitySpeed);
			GetTarget()->RigidBody2D()->SetGravityAccel(m_vGravityAccel);
			//GetTarget()->RigidBody2D()->SetGround(m_bUseGravity);
		}
	}
}
