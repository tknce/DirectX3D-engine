#include "pch.h"
#include "ParticleSystemUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CParticleUpdateShader.h>
#include <Engine/CComputeShader.h>

#include "CImGuiMgr.h"
#include "ListUI.h"

#define Target GetTarget()->ParticleSystem()

ParticleSystemUI::ParticleSystemUI()
	: ComponentUI("ParticleSystemUI", COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxCount()
	, m_iAliveCount()
	, m_vStartScale()
	, m_vEndScale()
	, m_vStartColor()
	, m_vEndColor()
	, m_vMinMaxSpeed()
	, m_vMinMaxLifeTime()
	, m_fSpawnRange()
	, m_Frequency()

{
}

ParticleSystemUI::~ParticleSystemUI()
{
}

void ParticleSystemUI::update()
{
	if (nullptr != GetTarget())
	{
		m_iAliveCount	=		Target->	GetAliveCount();
		m_iMaxCount		=		Target->		GetMaxCount();
		m_vStartScale	=		Target->	GetStartScale();
		m_vEndScale		=		Target->		GetEndScale();
		m_vStartColor	=		Target->	GetStartColor();
		m_vEndColor		=		Target->		GetEndColor();
		m_vMinMaxSpeed	=	Target->	GetMinMaxSpeed();
		m_vMinMaxLifeTime = Target->GetMinMaxLifeTime();
		m_fSpawnRange	=		Target->GetSpawnRange();
		m_Frequency		=		Target->		GetFrequency();
		m_WorldSpawn = Target->GetWorldSpawn();

		m_pComputueshader = Target->GetComputeShader();
		/*if(nullptr != Target->GetCurMaterial()->GetTexture())
		m_pTex = Target->GetCurMaterial()->GetTexture();*/
		m_pMaterrial = Target->GetCurMaterial(0);
	}
	ComponentUI::update();
}

void ParticleSystemUI::render_update()
{
	ComponentUI::render_update();

	ImGui::Text("m_iMaxCount  "); ImGui::SameLine(); ImGui::InputInt("##MaxCount", &m_iMaxCount);
	ImGui::Text("m_iAliveCount"); ImGui::SameLine(); ImGui::InputInt("##AliveCount", &m_iAliveCount);

	ImGui::Text("m_vStartScale"); ImGui::SameLine(); ImGui::InputFloat3("##StartScale", m_vStartScale);
	ImGui::Text("m_vEndScale  "); ImGui::SameLine(); ImGui::InputFloat3("##EndScale", m_vEndScale);

	// StartColor
	ImGui::Text("StartColor   "); ImGui::SameLine();
	ImGui::ColorEdit4("MyColor##3", (float*)&m_vStartColor, (false ? ImGuiColorEditFlags_AlphaPreview : 0));
	static bool saved_palette_init = true;
	static ImVec4 saved_palette[32] = {};
	if (saved_palette_init)
	{
		for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
		{
			ImGui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f,
				saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
			saved_palette[n].w = 1.0f; // Alpha
		}
		saved_palette_init = false;
	}

	// EndColor
	ImGui::Text("EndColor     "); ImGui::SameLine();
	ImGui::ColorEdit4("MyColor##4", (float*)&m_vEndColor, (false ? ImGuiColorEditFlags_AlphaPreview : 0));

	if (saved_palette_init)
	{
		for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
		{
			ImGui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f,
				saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
			saved_palette[n].w = 1.0f; // Alpha
		}
		saved_palette_init = false;
	}

	ImGui::Text("Speed");
	ImGui::Text("Min"); ImGui::SameLine(); ImGui::InputFloat("##Minspeed", &m_vMinMaxSpeed.x);  ImGui::SameLine();
	ImGui::Text("Max"); ImGui::SameLine(); ImGui::InputFloat("##Maxspeed", &m_vMinMaxSpeed.y);

	ImGui::Text("LifeTime");
	ImGui::Text("Min"); ImGui::SameLine(); ImGui::InputFloat("##Mintime", &m_vMinMaxLifeTime.x);  ImGui::SameLine();
	ImGui::Text("Max"); ImGui::SameLine(); ImGui::InputFloat("##Maxtime", &m_vMinMaxLifeTime.y);

	ImGui::Text("SpawnRange   "); ImGui::SameLine(); ImGui::InputFloat("##Range", &m_fSpawnRange);
	ImGui::Text("Frequency    "); ImGui::SameLine(); ImGui::InputFloat("##Frequency", &m_Frequency);
	ImGui::Text("particle Ignore Parent Scale"); ImGui::SameLine(); ImGui::Checkbox("##IgnorParentScaleparticle", &m_WorldSpawn);

	string Computename;
	if (nullptr != m_pTex)
	{
		Computename = string(m_pTex->GetKey().begin(), m_pTex->GetKey().end());
	}

	ImGui::Text("Texture ");
	ImGui::SameLine();

	ImGui::InputText("##ParticleTex", (char*)Computename.data(), Computename.length(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();

	if (ImGui::Button("##ParticleTexBtn", Vec2(15.f, 15.f)))
	{
		ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUI"));
		assert(pListUI);

		// 메쉬 목록을 받아와서, ListUI 에 전달
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::TEXTURE);
		static vector<wstring> vecRes;
		vecRes.clear();

		map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
		for (; iter != mapRes.end(); ++iter)
		{
			vecRes.push_back(iter->first);
		}
		pListUI->SetItemList(vecRes);
		pListUI->AddDynamicDBClicked(this, (FUNC_1)&ParticleSystemUI::SetTexture);

		pListUI->Open();
	}


	ImGui::Text("Set ComputeShader"); ImGui::SameLine();
	if (ImGui::Button("##Computeshader", Vec2(15.f, 15.f)))
	{
		ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUI"));
		assert(pListUI);

		// 메쉬 목록을 받아와서, ListUI 에 전달
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::COMPUTE_SHADER);
		static vector<wstring> vecRes;
		vecRes.clear();

		map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
		for (; iter != mapRes.end(); ++iter)
		{
			vecRes.push_back(iter->first);
		}
		pListUI->SetItemList(vecRes);
		pListUI->AddDynamicDBClicked(this, (FUNC_1)&ParticleSystemUI::SetComputeshader);

		pListUI->Open();
	}

	if (nullptr != m_pComputueshader)
	{
		Computename = string(m_pComputueshader->GetKey().begin(), m_pComputueshader->GetKey().end());
	}
	ImGui::Text("Computeshader ");
	ImGui::SameLine();
	ImGui::InputText("##Computename", (char*)Computename.data(), Computename.length(), ImGuiInputTextFlags_ReadOnly);




	Computename;
	if (nullptr != m_pMaterrial)
	{
		Computename = string(m_pMaterrial->GetKey().begin(), m_pMaterrial->GetKey().end());
	}

	ImGui::Text("Material ");
	ImGui::SameLine();

	ImGui::InputText("##MaterialParticle", (char*)Computename.data(), Computename.length(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();

	if (ImGui::Button("##MaterialParticleBtn", Vec2(15.f, 15.f)))
	{
		ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUI"));
		assert(pListUI);

		// 메쉬 목록을 받아와서, ListUI 에 전달
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::MATERIAL);
		static vector<wstring> vecRes;
		vecRes.clear();

		map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
		for (; iter != mapRes.end(); ++iter)
		{
			vecRes.push_back(iter->first);
		}
		pListUI->SetItemList(vecRes);
		pListUI->AddDynamicDBClicked(this, (FUNC_1)&ParticleSystemUI::SetMaterial);

		pListUI->Open();
	}









	if (nullptr != GetTarget())
	{
		Target->SetAliveCount(m_iAliveCount);
		Target->SetMaxCount(m_iMaxCount);
		Target->SetStartEndScale(m_vStartScale, m_vEndScale);
		Target->SetStartEndColor(m_vStartColor, m_vEndColor);
		Target->SetMinMaxSpeed(m_vMinMaxSpeed);
		Target->SetMinMaxLifeTime(m_vMinMaxLifeTime);
		Target->SetSpawnRange(m_fSpawnRange);
		Target->SetFrequency(m_Frequency);
		Target->SetWorldSpawn(m_WorldSpawn);
	}
}

void ParticleSystemUI::SetComputeshader(DWORD_PTR _strMaterialKey)
{
	string strKey = (char*)_strMaterialKey;
	wstring wstrKey = wstring(strKey.begin(), strKey.end());

	GetTarget()->ParticleSystem()->SetComputeShader(wstrKey);
}

void ParticleSystemUI::SetTexture(DWORD_PTR _strMaterialKey)
{
	string strKey = (char*)_strMaterialKey;
	wstring wstrKey = wstring(strKey.begin(), strKey.end());

	Target->GetCurMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(wstrKey));
}

void ParticleSystemUI::SetMaterial(DWORD_PTR _strMaterialKey)
{
	string strKey = (char*)_strMaterialKey;
	wstring wstrKey = wstring(strKey.begin(), strKey.end());

	Target->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(wstrKey),0);
}
