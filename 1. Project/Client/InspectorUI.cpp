#include "pch.h"
#include "InspectorUI.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CGameObject.h>
#include <Engine/CKeyMgr.h>
#include <Engine\CResMgr.h>
#include <Engine/GlobalComponent.h>
#include <Engine/CCollisionMgr.h>
#include <Engine/CLevel.h>


#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "CameraUI.h"
#include "Collider2DUI.h"
#include "Light2DUI.h"
#include "Light3DUI.h"
#include "ParticleSystemUI.h"
#include "TileMapUI.h"
#include "Animator2DUI.h"
#include "Animator3DUI.h"
#include "RigidBodyUI.h"
#include "LandScapeUI.h"
#include "SkyBoxUI.h"
#include "DecalUI.h"
#include "RigidBody3D.h"

#include "MaterialUI.h"
#include "TextureUI.h"
#include "MeshUI.h"

#include "ListUI.h"
#include "CImGuiMgr.h"
#include "ResUI.h"
#include "ScriptUI.h"

#include "ContentUI.h"
#include "OutLinerUI.h"

InspectorUI::InspectorUI()
	: UI("Inspector")
	, m_TargetObj(nullptr)
	, m_arrComUI()
	, m_arrResUI()
	, m_bLayerSet(false)
	, m_bLSet(false)
	, m_bLDel(false)
	, m_bReName(false)
	, m_iCurIdx(0)
{

	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformUI;	
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]);

	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = new Collider2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER] = new MeshRenderUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]);

	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA] = new CameraUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]);

	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D] = new Light2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT3D] = new Light3DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT3D]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT3D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM] = new ParticleSystemUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]->SetSize(ImVec2(0.f, 300.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]);

	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP] = new TileMapUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP]);

	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D] = new Animator2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR3D] = new Animator3DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR3D]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR3D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::RIGIDBODY2D] = new RigidBodyUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::RIGIDBODY2D]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::RIGIDBODY2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::RIGIDBODY3D] = new RigidBody3D;
	m_arrComUI[(UINT)COMPONENT_TYPE::RIGIDBODY3D]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::RIGIDBODY3D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::LANDSCAPE] = new LandScapeUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::LANDSCAPE]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::LANDSCAPE]);

	m_arrComUI[(UINT)COMPONENT_TYPE::SKYBOX] = new SkyBoxUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::SKYBOX]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::SKYBOX]);

	m_arrComUI[(UINT)COMPONENT_TYPE::DECAL] = new DecalUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::DECAL]->SetSize(ImVec2(0.f, 150.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::DECAL]);


	// ResourceUI
	m_arrResUI[(UINT)RES_TYPE::MESH] = new MeshUI;
	m_arrResUI[(UINT)RES_TYPE::MESH]->SetSize(ImVec2(0.f, 0.f));
	m_arrResUI[(UINT)RES_TYPE::MESH]->ShowSeperator(false);
	m_arrResUI[(UINT)RES_TYPE::MESH]->Close();
	AddChild(m_arrResUI[(UINT)RES_TYPE::MESH]);

	m_arrResUI[(UINT)RES_TYPE::TEXTURE] = new TextureUI;
	m_arrResUI[(UINT)RES_TYPE::TEXTURE]->SetSize(ImVec2(0.f, 0.f));
	m_arrResUI[(UINT)RES_TYPE::TEXTURE]->ShowSeperator(false);
	m_arrResUI[(UINT)RES_TYPE::TEXTURE]->Close();
	AddChild(m_arrResUI[(UINT)RES_TYPE::TEXTURE]);

	m_arrResUI[(UINT)RES_TYPE::MATERIAL] = new MaterialUI;
	m_arrResUI[(UINT)RES_TYPE::MATERIAL]->SetSize(ImVec2(0.f, 0.f));
	m_arrResUI[(UINT)RES_TYPE::MATERIAL]->ShowSeperator(false);
	m_arrResUI[(UINT)RES_TYPE::MATERIAL]->Close();
	AddChild(m_arrResUI[(UINT)RES_TYPE::MATERIAL]);

	//ScriptUI* pScriptUI = new ScriptUI;
	//pScriptUI->Close();
	//AddChild(pScriptUI);
	//m_vecScriptUI.push_back(pScriptUI);

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		string s = std::to_string(i);
		m_vLayer.push_back(s);
		m_matWord[i] = CCollisionMgr::GetInst()->GetCollisionLayer(i);
	}
}

InspectorUI::~InspectorUI()
{	
}

void InspectorUI::update()
{
	if (!IsValid(m_TargetObj))
	{
		SetTargetObject(nullptr);
	}

	UI::update();
}

void InspectorUI::render_update()
{
	if (ImGui::Button("AddComponent##ComponentBtn", Vec2(0, 0)))
	{
		ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUI"));
		assert(pListUI);

		// 컴포넌트 목록을 받아와서, ListUI 에 전달

		static vector<string> vecRes;
		vecRes.clear();

		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{

			vecRes.push_back(ToString((COMPONENT_TYPE)i));
		}


		pListUI->SetItemList(vecRes);
		pListUI->AddDynamicDBClicked(this, (FUNC_1)&InspectorUI::AddComponent);

		pListUI->Open();
	}
	ImGui::SameLine();
	if (ImGui::Button("DeleteComponet##LayerBtn", Vec2(0, 0)))
	{
		ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUI"));
		assert(pListUI);

		// 컴포넌트 목록을 받아와서, ListUI 에 전달

		static vector<string> vecRes;
		vecRes.clear();

		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{

			vecRes.push_back(ToString((COMPONENT_TYPE)i));
		}


		pListUI->SetItemList(vecRes);
		pListUI->AddDynamicDBClicked(this, (FUNC_1)&InspectorUI::DelComponent);

		pListUI->Open();
	}
	ImGui::SameLine();
	if (ImGui::Button("Obj_LaySet##LayerBtn", Vec2(0, 0)))
	{
		ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUI"));
		assert(pListUI);

		pListUI->SetItemList(m_vLayer);
		pListUI->AddDynamicDBClicked(this, (FUNC_1)&InspectorUI::ChangeLayer);

		pListUI->Open();
	}


	ImGui::SameLine();
	if (ImGui::Button("LaySet##", Vec2(0, 0)) || m_bLayerSet)
	{
		m_bLayerSet = true;

		CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
		ImGui::Begin("Layer");
		ImGui::SameLine(); ImGui::Checkbox("Set##LayerSet", &m_bLSet); ImGui::SameLine(); ImGui::Checkbox("Del##LayerDel", &m_bLDel);
		for (int iRow = 0; iRow < MAX_LAYER; ++iRow)
		{
			// Layer 이름과 인덱스 나열
			CLayer* Layer = CurLevel->GetLayer(iRow);
			string Layername = "     ";
			Layername += string(Layer->GetName().begin(), Layer->GetName().end());
			string LayerIdx = std::to_string(Layer->GetLayerIdx());
			if (ImGui::Button(LayerIdx.c_str()) && m_bLSet)
			{
				// 레이어 세팅
				ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUI"));
				assert(pListUI);
				m_iCurIdx = iRow;
				pListUI->SetItemList(m_vLayer);
				pListUI->AddDynamicDBClicked(this, (FUNC_1)&InspectorUI::SetColliderLayer);
				pListUI->Open();
			}
			ImGui::SameLine();  ImGui::Text(Layername.c_str());
			ImGui::SameLine(); ImGui::Text("           ");
			for (int iCol = iRow; iCol < MAX_LAYER; ++iCol)
			{
				if (m_matWord[iRow] & (1 << iCol))
				{
					// 만약 레이어 충돌 설정이 되어있으면 설정
					LayerIdx.clear();
					LayerIdx = std::to_string(iRow);
					LayerIdx += "|";
					LayerIdx += std::to_string(iCol);
					ImGui::SameLine();
					if (ImGui::Button(LayerIdx.c_str()))
					{
						if (m_bLDel)
						{
							CCollisionMgr::GetInst()->CollisionLayerRelease(iCol, iRow);
							for (int i = 0; i < MAX_LAYER; ++i)
							{
								m_matWord[i] = CCollisionMgr::GetInst()->GetCollisionLayer(i);
							}
						}
					}
				}
			}
		}
		if (ImGui::Button("OK##", Vec2(0, 0)))
		{

			m_bLayerSet = false;
		}
		ImGui::End();
		//SetColliderLayer();
	}
	ImGui::SameLine();
	if (ImGui::Button("ObjReName##OBJRENAME") || m_bReName)
	{
		m_bReName = true;
		ImGui::Begin("OBJRENAME");
		static char rename[32];

		strncpy_s(rename, m_OBJRENAME.c_str(), sizeof(m_OBJRENAME) - 1);
		ImGui::InputText("##Rename", rename, sizeof(rename) - 1);
		m_OBJRENAME = rename;
		
		// 셋버튼 클릭시 이름 바꿈
		if (ImGui::Button("Set##rename"))
		{
			wstring Newname = wstring(m_OBJRENAME.begin(), m_OBJRENAME.end());
			// 이전 이름을 지우고 새로운 네임을 넣는다
			m_TargetObj->SetName(Newname);
			m_bReName = false;
			OutLinerUI* pContentUI = (OutLinerUI*)CImGuiMgr::GetInst()->FindUI("Outliner");
			pContentUI->ResetLevel();
		}	
		if (ImGui::Button("SET##OBJRENAME"))
			m_bReName = false;

		ImGui::End();
	}



	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::LBTN))
	{
		Vec3 Mouse = Vec3(CKeyMgr::GetInst()->GetCameraMousePos().x, CKeyMgr::GetInst()->GetCameraMousePos().y, 0.f);
		if(m_TargetObj != nullptr)
		m_TargetObj->Transform()->SetRelativePos(Mouse);
	}

}


void InspectorUI::SetTargetObject(CGameObject* _Target)
{
	// 리소스가 타겟인 상태였다면
	if (nullptr != _Target && nullptr != m_TargetRes)
	{
		SetTargetResource(nullptr);
	}

	m_TargetObj = _Target;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComUI[i])
		{
			if (nullptr != m_TargetObj && nullptr == m_TargetObj->GetComponent((COMPONENT_TYPE)i))
			{
				m_arrComUI[i]->SetTarget(nullptr);
				m_arrComUI[i]->Close();
			}
			else
			{
				m_arrComUI[i]->SetTarget(_Target);

				if (nullptr != _Target)
					m_arrComUI[i]->Open();
				else
					m_arrComUI[i]->Close();
			}
		}
	}

	// Object 의 스크립트 확인
	if (nullptr == m_TargetObj)
	{
		for (auto& pScript : m_vecScriptUI)
		{
			pScript->Close();
		}
	}

	else
	{
		// 모든 스크립트 UI 비활성화
		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			m_vecScriptUI[i]->Close();
		}

		// 오브젝트의 보유 스크립트 가져옴
		const vector<CScript*>& vecScript = m_TargetObj->GetScripts();

		// 스트립트 수가 스트립트UI 보다 많으면, 추가로 UI 를 더 생성
		if (m_vecScriptUI.size() < vecScript.size())
		{
			for (size_t i = 0; i < vecScript.size() - m_vecScriptUI.size(); ++i)
			{
				ScriptUI* pScriptUI = new ScriptUI;
				pScriptUI->Close();
				AddChild(pScriptUI);
				m_vecScriptUI.push_back(pScriptUI);
			}
		}

		// 스크립트를 스크립트UI 에 배정
		for (size_t i = 0; i < vecScript.size(); ++i)
		{
			m_vecScriptUI[i]->Open();
			m_vecScriptUI[i]->SetTargetScript(vecScript[i]);
		}
	}


}

void InspectorUI::SetTargetResource(CRes* _Resource)
{
	// Object 가 타겟인 상태였다면
	if (nullptr != m_TargetObj)
	{
		SetTargetObject(nullptr);
	}

	if (nullptr != _Resource)
	{
		// 기존에 가리키던 리소스가 있으면, 해당 UI를 끄고
		if (nullptr != m_TargetRes && nullptr != m_arrResUI[(UINT)m_TargetRes->GetResType()])
		{
			m_arrResUI[(UINT)m_TargetRes->GetResType()]->Close();
		}

		// 새로 지정된 리소스를 담당하는 UI 를 활성화
		m_TargetRes = _Resource;
		RES_TYPE eType = m_TargetRes->GetResType();

		if (nullptr != m_arrResUI[(UINT)eType])
		{
			m_arrResUI[(UINT)eType]->SetTarget(m_TargetRes);
			m_arrResUI[(UINT)eType]->Open();
		}
	}
	else
	{
		for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
		{
			if (nullptr != m_arrResUI[i])
			{
				m_arrResUI[i]->SetTarget(nullptr);
				m_arrResUI[i]->Close();
			}
		}
	}
}

void InspectorUI::AddComponent(DWORD_PTR _componenttype)
{
	string Type = (char*)_componenttype;
	if (m_TargetObj != nullptr)
	{
		if ("TRANSFORM" == Type) { m_TargetObj->AddComponent(new CTransform); }
		else if ("CAMERA" == Type) { m_TargetObj->AddComponent(new CCamera); }
		else if ("COLLIDER2D" == Type) { m_TargetObj->AddComponent(new CCollider2D); }
		else if ("COLLIDER3D" == Type) {}
		else if ("ANIMATOR2D" == Type) { m_TargetObj->AddComponent(new CAnimator2D); }
		else if ("ANIMATOR3D" == Type) { m_TargetObj->AddComponent(new CAnimator3D); }
		else if ("LIGHT2D" == Type) { m_TargetObj->AddComponent(new CLight2D); }
		else if ("LIGHT3D" == Type) { m_TargetObj->AddComponent(new CLight3D); }
		else if ("RIGIDBODY2D" == Type) { m_TargetObj->AddComponent(new CRigidBody2D); }
		else if ("RIGIDBODY3D" == Type) { m_TargetObj->AddComponent(new CRigidBody3D); }
		else if ("MESHRENDER" == Type) { m_TargetObj->AddComponent(new CMeshRender); }
		else if ("TILEMAP" == Type) { m_TargetObj->AddComponent(new CTileMap); }
		else if ("PARTICLESYSTEM" == Type) { m_TargetObj->AddComponent(new CParticleSystem); }
		else if ("SKYBOX" == Type) { m_TargetObj->AddComponent(new CSkyBox); }
		else if ("DECAL" == Type) { m_TargetObj->AddComponent(new CDecal); }
		else if ("LANDSCAPE" == Type) { m_TargetObj->AddComponent(new CLandScape); }
		else if ("AI" == Type) { m_TargetObj->AddComponent(new AI); }
	}
	if (m_TargetObj != nullptr)
	{
		if ("TRANSFORM" == Type) { m_TargetObj->Transform()->begin(); }
		else if ("CAMERA" == Type) { m_TargetObj->Camera()->begin(); }
		else if ("COLLIDER2D" == Type) { m_TargetObj->Collider2D()->begin(); }
		else if ("COLLIDER3D" == Type) {}
		else if ("ANIMATOR2D" == Type) { m_TargetObj->Animator2D()->begin(); }
		else if ("ANIMATOR3D" == Type) { m_TargetObj->Animator3D()->begin(); }
		else if ("LIGHT2D" == Type) { m_TargetObj->Light2D()->begin(); }
		else if ("LIGHT3D" == Type) { m_TargetObj->Light3D()->begin(); }
		else if ("RIGIDBODY2D" == Type) { m_TargetObj->RigidBody2D()->begin(); }
		else if ("RIGIDBODY3D" == Type) { m_TargetObj->RigidBody3D()->begin(); }
		else if ("MESHRENDER" == Type) { m_TargetObj->MeshRender()->begin(); }
		else if ("TILEMAP" == Type) { m_TargetObj->TileMap()->begin(); }
		else if ("PARTICLESYSTEM" == Type) { m_TargetObj->ParticleSystem()->begin(); }
		else if ("SKYBOX" == Type) { m_TargetObj->SkyBox()->begin(); }
		else if ("DECAL" == Type) { m_TargetObj->Decal()->begin(); }
		else if ("LANDSCAPE" == Type) { m_TargetObj->LandScape()->begin(); }
		else if ("AI" == Type) {}
	}
}

void InspectorUI::DelComponent(DWORD_PTR _componenttype)
{
	string Type = (char*)_componenttype;
	if (m_TargetObj != nullptr)
	{

		if ("TRANSFORM" == Type) { m_TargetObj->DeleteComponent(COMPONENT_TYPE::TRANSFORM); }
		else if ("CAMERA" == Type) { m_TargetObj->DeleteComponent(COMPONENT_TYPE::CAMERA); }
		else if ("COLLIDER2D" == Type) { m_TargetObj->DeleteComponent(COMPONENT_TYPE::COLLIDER2D); }
		else if ("COLLIDER3D" == Type) {}
		else if ("ANIMATOR2D" == Type) { m_TargetObj->DeleteComponent(COMPONENT_TYPE::ANIMATOR2D); }
		else if ("ANIMATOR3D" == Type) { m_TargetObj->DeleteComponent(COMPONENT_TYPE::ANIMATOR2D); }
		else if ("LIGHT2D" == Type) { m_TargetObj->DeleteComponent(COMPONENT_TYPE::LIGHT2D); }
		else if ("LIGHT3D" == Type) {}
		else if ("RIGIDBODY2D" == Type) { m_TargetObj->DeleteComponent(COMPONENT_TYPE::RIGIDBODY2D); }
		else if ("RIGIDBODY3D" == Type) { m_TargetObj->DeleteComponent(COMPONENT_TYPE::RIGIDBODY3D); }
		else if ("MESHRENDER" == Type) { m_TargetObj->DeleteComponent(COMPONENT_TYPE::MESHRENDER); }
		else if ("TILEMAP" == Type) { m_TargetObj->DeleteComponent(COMPONENT_TYPE::TILEMAP); }
		else if ("PARTICLESYSTEM" == Type) { m_TargetObj->DeleteComponent(COMPONENT_TYPE::PARTICLESYSTEM); }
		else if ("SKYBOX" == Type) {}
		else if ("DECAL" == Type) {}
		else if ("LANDSCAPE" == Type) {}
	}
}

void InspectorUI::ChangeLayer(DWORD_PTR _LayerIdx)
{
	string idx = (char*)_LayerIdx;
	int LayerIdx = std::stoi(idx);

	ChangeIDX(GetTargetObject(), LayerIdx);
}

void InspectorUI::SetColliderLayer(DWORD_PTR _LayerIdx)
{
	string idx = (char*)_LayerIdx;
	int LayerIdx = std::stoi(idx);
	CCollisionMgr::GetInst()->CollisionLayerCheck(m_iCurIdx, LayerIdx);
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_matWord[i] = CCollisionMgr::GetInst()->GetCollisionLayer(i);
	}
}





