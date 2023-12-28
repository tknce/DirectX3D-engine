#include "pch.h"
#include "SkyBoxUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CSkyBox.h>

SkyBoxUI::SkyBoxUI()
	:ComponentUI("SkyBoxUI", COMPONENT_TYPE::SKYBOX)
{
}

SkyBoxUI::~SkyBoxUI()
{
}

void SkyBoxUI::update()
{
	ComponentUI::update();
	if (GetTarget() != nullptr)
	{
		m_iType = GetTarget()->SkyBox()->GetType();
	}
}

void SkyBoxUI::render_update()
{
	ComponentUI::render_update();
	if(m_iType == 0)
	ImGui::Text("SPHERE");
	else if (m_iType == 1)
	ImGui::Text("CUBE");

	ImGui::SameLine();
	ImGui::InputInt("Type", &m_iType);

	if (GetTarget() != nullptr)
	{
		GetTarget()->SkyBox()->SetType((UINT)m_iType);
	}
}

