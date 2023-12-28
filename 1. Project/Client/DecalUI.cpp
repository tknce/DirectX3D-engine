#include "pch.h"
#include "DecalUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CDecal.h>

DecalUI::DecalUI()
	:ComponentUI("DecalUI", COMPONENT_TYPE::DECAL)
{
}

DecalUI::~DecalUI()
{
}

void DecalUI::update()
{
	ComponentUI::update();

	if (GetTarget() != nullptr)
	{
		m_bRightType = GetTarget()->Decal()->GetDefaultLit();
	}

}

void DecalUI::render_update()
{
	ComponentUI::render_update();

	ImGui::Text("Right");
	ImGui::SameLine();
	ImGui::Checkbox("##type", &m_bRightType);

	if (GetTarget() != nullptr)
	{
		GetTarget()->Decal()->SetDefaultLit(m_bRightType);
	}
}

