#include "pch.h"
#include "LandScapeUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CLandScape.h>


LandScapeUI::LandScapeUI()
	:ComponentUI("LandScapeUI", COMPONENT_TYPE::LANDSCAPE)
{
}

LandScapeUI::~LandScapeUI()
{
}

void LandScapeUI::update()
{
	ComponentUI::update();
	if (GetTarget() != nullptr)
	{
		
		m_vFaceCount = GetTarget()->LandScape()->GetFaceCount();
		m_vWeightResolution = GetTarget()->LandScape()->GetWeightResolution();
		m_vBrushScale = GetTarget()->LandScape()->GetBrushScale();

		m_iWeightIdx = GetTarget()->LandScape()->GetWeightIdx();
		m_iMod = GetTarget()->LandScape()->GetMod();

		// 0 hight
		// 1 brush
		// 2 arrtex
		m_vTex = GetTarget()->LandScape()->GetHightBrushArrTex();

	}
}

void LandScapeUI::render_update()
{
	ComponentUI::render_update();
	ImGui::Text("Mod"); ImGui::SameLine();
	if (ImGui::InputInt("##Mod", &m_iMod))
	{
		if (m_iMod < 0)
			m_iMod = 0;
		GetTarget()->LandScape()->SetMod(m_iMod);
	}
	ImGui::SameLine();
	if (m_iMod == 0)
		ImGui::Text("HEIGHT");
	else if (m_iMod == 1)
		ImGui::Text("SPLAT");
	else
		ImGui::Text("NONT");

	ImGui::Text("FaceCount"); ImGui::SameLine();
	if (ImGui::InputFloat2("##FaceCount", m_vFaceCount))
	{
		GetTarget()->LandScape()->SetFaceCount(m_vFaceCount.x, m_vFaceCount.y);
	}
	ImGui::Text("WeightResolution"); ImGui::SameLine();
	if (ImGui::InputFloat2("##WeightResolution", m_vWeightResolution))
	{
		GetTarget()->LandScape()->SetWeightResolution(m_vWeightResolution);
	}
	ImGui::Text("WeightIdx"); ImGui::SameLine();
	if (ImGui::InputInt("##WeightIdx", &m_iWeightIdx))
	{
		if (m_iWeightIdx < 0)
			m_iWeightIdx = 0;
		GetTarget()->LandScape()->SetWeightIdx(m_iWeightIdx);
	}

	ImGui::Text("BrushScale"); ImGui::SameLine();
	if (ImGui::InputFloat2("##BrushScale", m_vBrushScale))
	{
		GetTarget()->LandScape()->SetBrushScale(m_vBrushScale);
	}
	if (m_vTex[0].Get() != nullptr)
		ImGui::Image(m_vTex[0].Get()->GetSRV().Get(), Vec2(150.f, 150.f));

	ImGui::SameLine();
	if (m_vTex[1].Get() != nullptr)
		ImGui::Image(m_vTex[1].Get()->GetSRV().Get(), Vec2(150.f, 150.f));

	ImGui::SameLine();
	if (m_vTex[2].Get() != nullptr)
	{
		//Ptr<CTexture> Tex = m_vTex[2].Get()->CreateArrTexShaderResourceView(m_vTex[2],2);
		
		//ImGui::Image(m_vTex[2].Get()->GetSRV().Get(), Vec2(150.f, 150.f));
	}
}


