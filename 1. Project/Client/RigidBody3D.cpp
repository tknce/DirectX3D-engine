#include "pch.h"
#include "RigidBody3D.h"

#include "CImGuiMgr.h"


#include <Engine/CGameObject.h>
#include <Engine/CRigidBody3D.h>



RigidBody3D::RigidBody3D()
	:ComponentUI("RIGIDBODY3D",COMPONENT_TYPE::RIGIDBODY3D)
	, m_bIsGravity(false)
	, m_bIsDynamic(false)
{
}

RigidBody3D::~RigidBody3D()
{
}


void RigidBody3D::update()
{
	ComponentUI::update();
	if (GetTarget() != nullptr)
	{
		
		m_bIsDynamic = GetTarget()->RigidBody3D()->GetDinamic();
		m_bIsGravity = GetTarget()->RigidBody3D()->GetGravity();
		m_vVelocity = GetTarget()->RigidBody3D()->GetVelocity_info();
		m_vAngularVelocity = GetTarget()->RigidBody3D()->GetAngularVelocity_info();
		m_vPosition = GetTarget()->RigidBody3D()->GetPosition_info();
		m_vRot = GetTarget()->RigidBody3D()->GetRot();
	}
}

void RigidBody3D::render_update()
{
	ComponentUI::render_update();
	//ImGui::Text("Dinamic"); ImGui::SameLine(); 
	ImGui::Text("RockPos"); ImGui::SameLine();
	static bool b_rock[3] = { false, false,false };
	b_rock[0] = m_vRockPos.x;
	ImGui::Text("X"); ImGui::SameLine();
	if (ImGui::Checkbox("##RockPos_x", &b_rock[0]))
	{		
		m_vRockPos.x = b_rock[0];
		GetTarget()->RigidBody3D()->SetAxisLock(m_vRockPos, m_vRockRot);
	} 
	ImGui::SameLine();

	b_rock[1] = m_vRockPos.y;
	ImGui::Text("Y"); ImGui::SameLine();
	if (ImGui::Checkbox("##RockPos_y", &b_rock[1]))
	{
		m_vRockPos.y = b_rock[1];
		GetTarget()->RigidBody3D()->SetAxisLock(m_vRockPos, m_vRockRot);
	} 
	ImGui::SameLine();

	b_rock[2] = m_vRockPos.z;
	ImGui::Text("Z"); ImGui::SameLine();
	if (ImGui::Checkbox("##RockPos_z", &b_rock[2]))
	{
		m_vRockPos.z = b_rock[2];
		GetTarget()->RigidBody3D()->SetAxisLock(m_vRockPos, m_vRockRot);
	}

	ImGui::Text("RockRot"); ImGui::SameLine();
	
	b_rock[0] = m_vRockRot.x;
	ImGui::Text("X"); ImGui::SameLine();
	if (ImGui::Checkbox("##RockRot_x", &b_rock[0]))
	{
		m_vRockRot.x = b_rock[0];
		GetTarget()->RigidBody3D()->SetAxisLock(m_vRockPos, m_vRockRot);
	}
	ImGui::SameLine();

	b_rock[1] = m_vRockRot.y;
	ImGui::Text("Y"); ImGui::SameLine();
	if (ImGui::Checkbox("##RockRot_y", &b_rock[1]))
	{
		m_vRockRot.y = b_rock[1];
		GetTarget()->RigidBody3D()->SetAxisLock(m_vRockPos, m_vRockRot);
	}
	ImGui::SameLine();

	b_rock[2] = m_vRockRot.z;
	ImGui::Text("Z"); ImGui::SameLine();
	if (ImGui::Checkbox("##RockRot_z", &b_rock[2]))
	{
		m_vRockRot.z = b_rock[2];
		GetTarget()->RigidBody3D()->SetAxisLock(m_vRockPos, m_vRockRot);
	}

	ImGui::Text("Static"); ImGui::SameLine();
	if (ImGui::Checkbox("##static", &m_bIsDynamic))
	{
		m_bIsDynamic != m_bIsDynamic;
		GetTarget()->RigidBody3D()->StaticChange(m_bIsDynamic);
	}

	if (ImGui::Button("Player"))
	{
		GetTarget()->RigidBody3D()->SettingPlayer();
	}

	ImGui::Text("infomation");
	ImGui::Text("Velocity"); ImGui::SameLine();
	ImGui::InputFloat3("##Velocity", m_vVelocity);

	ImGui::Text("AngularVelocity"); ImGui::SameLine();
	ImGui::InputFloat3("##AngularVelocity", m_vAngularVelocity);

	ImGui::Text("Position"); ImGui::SameLine();
	ImGui::InputFloat3("##PhysxPosition", m_vPosition);

	ImGui::Text("Rotate"); ImGui::SameLine();
	if (ImGui::InputFloat3("##PhysxRotate", m_vRot))
	{
		GetTarget()->RigidBody3D()->SetRot(m_vRot);
		GetTarget()->RigidBody3D()->SetPosition(true);
	}
}

