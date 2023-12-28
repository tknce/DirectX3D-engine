#include "pch.h"
#include "CameraUI.h"



#include <Engine\CCamera.h>
#include <Engine/CTransform.h>
#include <Engine\CResMgr.h>
#include <Engine\CRes.h>

#include "CImGuiMgr.h"
#include "ListUI.h"

CameraUI::CameraUI()
	: ComponentUI("Cameracomponent", COMPONENT_TYPE::CAMERA)
	, m_bProjtype(true)
	, m_fFar(1000.f)
	, m_fScale(0)
	, m_iLayermask(0)
	, m_vLayer()
	
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		string s = std::to_string(i);
		m_vLayer.push_back(s);
	}
}

CameraUI::~CameraUI()
{
}



void CameraUI::update()
{
	if (nullptr != GetTarget())
	{
		if (!GetTarget()->ComponentCheck(GetComponentType()))
			return;

		if (GetTarget()->Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAHPICS)
		{
			m_bProjtype = true;
		}
		else
		{
			m_bProjtype = false;
		}
		
		m_fFar = GetTarget()->Camera()->GetFar();
		if(m_bProjtype)
		m_fScale = GetTarget()->Camera()->GetOrthographicScale();
		else
		{
			m_fScale = 1;
		}
	}

	ComponentUI::update();
}

void CameraUI::render_update()
{
	ComponentUI::render_update();



	ImGui::Text("Far        "); ImGui::SameLine(); ImGui::InputFloat("##Far", &m_fFar);
	ImGui::Text("Scale      "); ImGui::SameLine(); ImGui::InputFloat("##Scale", &m_fScale);
	ImGui::Text("Projecttype"); ImGui::SameLine(); ImGui::Checkbox("##Projecttype", &m_bProjtype);
	ImGui::Text("Layer      ");
	ImGui::SameLine();
	if (ImGui::Button("##LayerBtn", Vec2(15.f, 15.f)))
	{
		ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUI"));
		assert(pListUI);
		pListUI->SetItemList(m_vLayer);
		pListUI->AddDynamicDBClicked(this, (FUNC_1)&CameraUI::Setmask);

		pListUI->Open();
	}





	if (GetTarget())
	{
		GetTarget()->Camera()->SetFar(m_fFar);
		if(m_fScale != 0)
			GetTarget()->Camera()->SetOrthographicScale(m_fScale);

		if (m_bProjtype)
		{
			GetTarget()->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAHPICS);
			GetTarget()->Transform()->SetRelativeRotation(Vec3(0.f,0.f,0.f));
		}
		else
		{
			GetTarget()->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
		}
		
	}
}

void CameraUI::Setmask(DWORD_PTR _iMask)
{
	string idx = (char*)_iMask;
	int mask = std::stoi(idx);

	if(0 <= mask && MAX_LAYER > mask)
	GetTarget()->Camera()->SetLayerMask(mask);
}

