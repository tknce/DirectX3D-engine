#include "pch.h"
#include "OutLinerUI.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine/CResMgr.h>
#include <Engine\CEventMgr.h>

#include "TreeUI.h"
#include "CImGuiMgr.h"
#include "InspectorUI.h"
#include "ListUI.h"

OutLinerUI::OutLinerUI()
	: UI("Outliner")
	, m_Tree(nullptr)
{
	m_Tree = new TreeUI("##OutLinerTree");
	AddChild(m_Tree);

	// 트리 설정
	m_Tree->ShowSeperator(false);
	m_Tree->SetDummyRoot(true);

	// Tree 에 Delegate 등록
	m_Tree->AddDynamic_Selected(this, (FUNC_1)&OutLinerUI::SetObjectToInspector);
	m_Tree->AddDynamic_DragDrop(this, (FUNC_2)&OutLinerUI::AddChildObject);
	m_Tree->AddDynamic_RightMenu(this, (FUNC_1_1)&OutLinerUI::AddPrePab);
	m_Tree->AddDynamic_Delete(this, (FUNC_1)&OutLinerUI::DeleteObject);

	// 레벨 오브젝트 갱신
	ResetLevel();
}

OutLinerUI::~OutLinerUI()
{
}

void OutLinerUI::update()
{
	if(CEventMgr::GetInst()->IsLevelChanged())
	{
		ResetLevel();
	}
}

void OutLinerUI::render_update()
{
	ImGui::SameLine();
	if (ImGui::Button("Refresh##refresh", Vec2(0, 0)))
	{
		ResetLevel();
	}
}

void OutLinerUI::ResetLevel()
{
	m_Tree->Clear();

	// 현재 레벨을 가져온다
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	string strLevelName = string(pCurLevel->GetName().begin(), pCurLevel->GetName().end());

	// 레벨 이름으로 루트노드 추가
	TreeNode* pRootNode = m_Tree->AddItem(nullptr, strLevelName.c_str(), 0, false);

	// 모든 레이어를 확인하면서, 부모 오브젝트를 기준으로 트리에 추가
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObject = pCurLevel->GetLayer(i)->GetParentObjects();

		for (size_t j = 0; j < vecObject.size(); ++j)
		{
			AddGameObjectToTree(pRootNode, vecObject[j]);
		}
	}
}

void OutLinerUI::SetObjectToInspector(DWORD_PTR _res)
{
	// _res : 클릭한 노드
	TreeNode* pSelectedNode = (TreeNode*)_res;
	CGameObject* pObject = (CGameObject*)pSelectedNode->GetData();

	// InspectorUI 에 클릭된 Resouse 를 알려준다.
	InspectorUI* Inspector = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");

	Inspector->SetTargetObject(pObject);
}

void OutLinerUI::AddGameObjectToTree(TreeNode* _ParentNode, CGameObject* _Object)
{
	string strObjectName = string(_Object->GetName().begin(), _Object->GetName().end());
	TreeNode* pCurNode = m_Tree->AddItem(_ParentNode, strObjectName.c_str(), (DWORD_PTR)_Object);

	const vector<CGameObject*>& vecChild = _Object->GetChildObject();
	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		AddGameObjectToTree(pCurNode, vecChild[i]);
	}
}

void OutLinerUI::DeleteObject(DWORD_PTR _res)
{
	TreeNode* Node = (TreeNode*)_res;
	wstring Objectname = wstring(Node->GetName().begin(), Node->GetName().end());
	CGameObject* curobject = CLevelMgr::GetInst()->FindObjectByName(Objectname);
	curobject->Destroy();
}

void OutLinerUI::AddChildObject(DWORD_PTR _ChildObject, DWORD_PTR _ParentObject)
{
	TreeNode* pChildNode = (TreeNode*)_ChildObject;
	TreeNode* pParentNode = (TreeNode*)_ParentObject;

	tEvent evt = {};
	evt.eType = EVENT_TYPE::ADD_CHILD;
	evt.wParam = pChildNode->GetData();
	evt.lParam = pParentNode->GetData();
	CEventMgr::GetInst()->AddEvent(evt);
}

void OutLinerUI::AddPrePab(DWORD_PTR _GameObject)
{
	TreeNode* pGameObject = (TreeNode*)_GameObject;	
	wstring Name = wstring(pGameObject->GetName().begin(), pGameObject->GetName().end());
	Name += L"#1";
	CGameObject* Gameobject = dynamic_cast<CGameObject*>((CGameObject*)pGameObject->GetData());
	
	if (!pGameObject->GetDataName().empty())
	{
		Name.clear();
		Name = wstring(pGameObject->GetDataName().begin(), pGameObject->GetDataName().end());
	}

	CResMgr::GetInst()->AddRes<CPrefab>(Name, new CPrefab(Gameobject->Clone()));
	pGameObject->ResetDataName();
}

//IMGUI_DEMO_MARKER("Popups/Context menus");
//if (ImGui::TreeNode("Context menus"))
//{
//	HelpMarker("\"Context\" functions are simple helpers to associate a Popup to a given Item or Window identifier.");
//
//	// BeginPopupContextItem() is a helper to provide common/simple popup behavior of essentially doing:
//	//     if (id == 0)
//	//         id = GetItemID(); // Use last item id
//	//     if (IsItemHovered() && IsMouseReleased(ImGuiMouseButton_Right))
//	//         OpenPopup(id);
//	//     return BeginPopup(id);
//	// For advanced uses you may want to replicate and customize this code.
//	// See more details in BeginPopupContextItem().
//
//	// Example 1
//	// When used after an item that has an ID (e.g. Button), we can skip providing an ID to BeginPopupContextItem(),
//	// and BeginPopupContextItem() will use the last item ID as the popup ID.
//	{
//		const char* names[5] = { "Label1", "Label2", "Label3", "Label4", "Label5" };
//		for (int n = 0; n < 5; n++)
//		{
//			ImGui::Selectable(names[n]);
//			if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
//			{
//				ImGui::Text("This a popup for \"%s\"!", names[n]);
//				if (ImGui::Button("Close"))
//					ImGui::CloseCurrentPopup();
//				ImGui::EndPopup();
//			}
//			if (ImGui::IsItemHovered())
//				ImGui::SetTooltip("Right-click to open popup");
//		}
//	}
//
//	// Example 2
//	// Popup on a Text() element which doesn't have an identifier: we need to provide an identifier to BeginPopupContextItem().
//	// Using an explicit identifier is also convenient if you want to activate the popups from different locations.
//	{
//		HelpMarker("Text() elements don't have stable identifiers so we need to provide one.");
//		static float value = 0.5f;
//		ImGui::Text("Value = %.3f <-- (1) right-click this text", value);
//		if (ImGui::BeginPopupContextItem("my popup"))
//		{
//			if (ImGui::Selectable("Set to zero")) value = 0.0f;
//			if (ImGui::Selectable("Set to PI")) value = 3.1415f;
//			ImGui::SetNextItemWidth(-FLT_MIN);
//			ImGui::DragFloat("##Value", &value, 0.1f, 0.0f, 0.0f);
//			ImGui::EndPopup();
//		}
//
//		// We can also use OpenPopupOnItemClick() to toggle the visibility of a given popup.
//		// Here we make it that right-clicking this other text element opens the same popup as above.
//		// The popup itself will be submitted by the code above.
//		ImGui::Text("(2) Or right-click this text");
//		ImGui::OpenPopupOnItemClick("my popup", ImGuiPopupFlags_MouseButtonRight);
//
//		// Back to square one: manually open the same popup.
//		if (ImGui::Button("(3) Or click this button"))
//			ImGui::OpenPopup("my popup");
//	}
//
//	// Example 3
//	// When using BeginPopupContextItem() with an implicit identifier (NULL == use last item ID),
//	// we need to make sure your item identifier is stable.
//	// In this example we showcase altering the item label while preserving its identifier, using the ### operator (see FAQ).
//	{
//		HelpMarker("Showcase using a popup ID linked to item ID, with the item having a changing label + stable ID using the ### operator.");
//		static char name[32] = "Label1";
//		char buf[64];
//		sprintf(buf, "Button: %s###Button", name); // ### operator override ID ignoring the preceding label
//		ImGui::Button(buf);
//		if (ImGui::BeginPopupContextItem())
//		{
//			ImGui::Text("Edit name:");
//			ImGui::InputText("##edit", name, IM_ARRAYSIZE(name));
//			if (ImGui::Button("Close"))
//				ImGui::CloseCurrentPopup();
//			ImGui::EndPopup();
//		}
//		ImGui::SameLine(); ImGui::Text("(<-- right-click here)");
//	}
//
//	ImGui::TreePop();
//}