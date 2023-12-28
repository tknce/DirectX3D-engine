#include "pch.h"
#include "Collider2DUI.h"

#include <Engine\CGameObject.h>
#include <Engine/CCollider2D.h>
#include <Engine/CTransform.h>
#include <Engine/CMeshRender.h>
#include <Engine/CTexture.h>
#include <Engine/CTileMap.h>
#include <Engine/CKeyMgr.h>

Collider2DUI::Collider2DUI()
	: ComponentUI("Collider", COMPONENT_TYPE::COLLIDER2D)
	, m_vOffset()
	, m_vScale()
	, m_vRot()
	, m_Type()
{
}

Collider2DUI::~Collider2DUI()
{
}


void Collider2DUI::update()
{
	if (nullptr != GetTarget())
	{
		Vec2 offset;
		offset = GetTarget()->Collider2D()->GetOffsetPos();
		m_vOffset = Vec3(offset.x, offset.y, 0.f);
		offset = GetTarget()->Collider2D()->GetScale();
		m_vScale = Vec3(offset.x, offset.y, 0.f);
		m_vRot = GetTarget()->Collider2D()->GetRotation();
		m_Type = GetTarget()->Collider2D()->GetCollider2DType();
		m_bIgnorObjectScale = GetTarget()->Collider2D()->GetIgnoreObjectScale();
		m_bEditOffset = GetTarget()->Collider2D()->GetEditmode();
	}
	ComponentUI::update();
}

void Collider2DUI::render_update()
{


	ComponentUI::render_update();

	ImGui::Text("OffSet  "); ImGui::SameLine(); ImGui::InputFloat2("##Offset", m_vOffset);
	ImGui::Text("Scale   "); ImGui::SameLine(); ImGui::InputFloat2("##Scale", m_vScale);

	m_vRot.ToDegree();
	ImGui::Text("Rotation"); ImGui::SameLine(); ImGui::InputFloat3("##Rotation", m_vRot);

	ImGui::Text("Ignore Parent Scale"); ImGui::SameLine(); ImGui::Checkbox("##IgnorParentScale", &m_bIgnorObjectScale);
	ImGui::Text("EditOffset"); ImGui::SameLine(); ImGui::Checkbox("##EditOffset", &m_bEditOffset);
	
	if (GetTarget())
	{
		GetTarget()->Collider2D()->SetOffsetPos(Vec2(m_vOffset.x, m_vOffset.y));
		GetTarget()->Collider2D()->SetScale(Vec2(m_vScale.x, m_vScale.y));

		m_vRot.ToRadian();
		GetTarget()->Collider2D()->SetRotation(m_vRot);

		GetTarget()->Collider2D()->SetIgnoreObjectScale(m_bIgnorObjectScale);
		if (m_bIgnorObjectScale)
		{
			GetTarget()->Transform()->GetRelativeScale();
		}
		GetTarget()->Collider2D()->SetEditmode(m_bEditOffset);
	}
	
	MouseDragMode_TileMap();
	// MODE
	if (m_bEditOffset)
	{
		MouseDragMode_Render();
		MouseDragMode_TileMap();

	}
	




}

void Collider2DUI::MouseDragMode_Render()
{
	m_vPreTransformPos = GetTarget()->Transform()->GetRelativePos();
}

void Collider2DUI::MouseDragMode_TileMap()
{
	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::LBTN))
	{
		Vec3 Mouse = Vec3(CKeyMgr::GetInst()->GetCameraMousePos().x, CKeyMgr::GetInst()->GetCameraMousePos().y,0.f);
		GetTarget()->Transform()->SetRelativePos(Mouse);
	
		

	}
	


}




