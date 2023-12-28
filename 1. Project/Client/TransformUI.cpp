#include "pch.h"
#include "TransformUI.h"
#include "ListUI.h"
#include "CImGuiMgr.h"


#include <Engine\CTransform.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>


TransformUI::TransformUI()
	: ComponentUI("Transform", COMPONENT_TYPE::TRANSFORM)	
	, m_bIgnorScale(false)
{
}

TransformUI::~TransformUI()
{
}


void TransformUI::update()
{
	if (nullptr != GetTarget())
	{
		if (!GetTarget()->ComponentCheck(GetComponentType()))
			return;
		m_iLayer = GetTarget()->GetLayerIdx();
		m_vPos = GetTarget()->Transform()->GetRelativePos();
		m_vScale = GetTarget()->Transform()->GetRelativeScale();
		m_vRot = GetTarget()->Transform()->GetRelativeRotation();

		m_bIgnorScale = GetTarget()->Transform()->IsIgnoreParentScale();
	}

	ComponentUI::update();
}

void TransformUI::render_update()
{
	ComponentUI::render_update();
	ImGui::Text("Layer"); ImGui::SameLine(); 
	if (ImGui::InputInt("##Layer", &m_iLayer))
	{
		GetTarget()->SetLayerIdx(m_iLayer);
	}

	ImGui::Text("Position"); ImGui::SameLine(); 
	if (ImGui::InputFloat3("##Position", m_vPos))
	{
		if (GetTarget()->RigidBody3D())
			GetTarget()->RigidBody3D()->SetPosition(true);
		GetTarget()->Transform()->SetRelativePos(m_vPos);
	}

	ImGui::Text("Scale   "); ImGui::SameLine();
	if (ImGui::InputFloat3("##Scale", m_vScale))
	{

		GetTarget()->Transform()->SetRelativeScale(m_vScale);
	}

	m_vRot.ToDegree();
	ImGui::Text("Rotation"); ImGui::SameLine(); 
	if (ImGui::InputFloat3("##Rotation", m_vRot))
	{
		if (GetTarget()->RigidBody3D())
		{
			GetTarget()->RigidBody3D()->SetPosition(true);
			GetTarget()->RigidBody3D()->SetRot(m_vRot);
		}
			
		m_vRot.ToRadian();
		GetTarget()->Transform()->SetRelativeRotation(m_vRot);		
	}
		
	ImGui::Text("Ignore Parent Scale"); ImGui::SameLine(); 
	if (ImGui::Checkbox("##IgnorParentScale", &m_bIgnorScale))
	{
		GetTarget()->Transform()->SetIgnoreParentScale(m_bIgnorScale);
	}

	ImGui::Text("socket"); ImGui::SameLine();
	if (ImGui::Checkbox("##socket", &m_socket))
	{
		vector<vector<CGameObject*>> object;
		CLevelMgr::GetInst()->GetCurLevel()->GetGameObject(object);

		ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUI"));
		assert(pListUI);

		// 메테리얼 목록을 받아와서, ListUI 에 전달
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::MATERIAL);
		static vector<wstring> vecname;
		vecname.clear();

		for (int i = 0; i < object.size(); ++i)
		{
			for (int j = 0; j < object[i].size(); ++j)
			{
				vecname.push_back(object[i][j]->GetName());
			}
		}
		vecname.push_back(L"TAKE_OUT");
		pListUI->SetItemList(vecname);
		pListUI->AddDynamicDBClicked(this, (FUNC_1)&TransformUI::SetSockey);

		pListUI->Open();
	}
}

void TransformUI::SetSockey(DWORD_PTR _strobjectname)
{
	string strKey = (char*)_strobjectname;
	wstring wstrKey = wstring(strKey.begin(), strKey.end());

	CGameObject* pObject = CLevelMgr::GetInst()->FindObjectByName(wstrKey);
	if (nullptr == pObject || GetTarget()->Transform()->GetSocket() != nullptr)
		GetTarget()->Transform()->SetSocket(nullptr);	

	GetTarget()->Transform()->SetSocket(pObject);
	
}
