#include "pch.h"
#include "TreeUI.h"
#include "CImGuiMgr.h"
#include "ContentUI.h"


// ========
// TreeNode
// ========
TreeNode::TreeNode()
	: m_ParentNode(nullptr)
	, m_data(0)
	, m_TreeUI(nullptr)
	, m_bFrame(false)
	, m_bSelected(false)
	, m_bPrefab(false)
	, m_bInstantiate(false)
	, m_iIdx(0)
{
}

TreeNode::~TreeNode()
{
	Safe_Del_Vec(m_vecChildNode);
}


void TreeNode::render_update()
{
	int iFlag = 0;
	if (m_bFrame)
		iFlag |= ImGuiTreeNodeFlags_Framed;
	if (m_bSelected)
		iFlag |= ImGuiTreeNodeFlags_Selected;
	if (m_vecChildNode.empty())
		iFlag |= ImGuiTreeNodeFlags_Leaf;			// 숨기기모드

	string strName = m_strName;
	if (m_bFrame && m_vecChildNode.empty()) // 이름 정렬맞추기 최상위 부모
		strName = "\t" + strName;

	// 뒤에 식별번호 붙여서 이름(키) 이 같지 않도록 함
	char szTag[50] = "";
	sprintf_s(szTag, 50, "##%d", m_iIdx);
	strName += szTag;

	if (ImGui::TreeNodeEx(strName.c_str(), iFlag))
	{
		// 드래그 체크
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			m_TreeUI->SetBeginDragNode(this);

			ImGui::SetDragDropPayload(m_TreeUI->GetName().c_str(), (void*)this, sizeof(TreeNode));	// 델리게이트
			ImGui::Text(strName.c_str());																	// 이름

			ImGui::EndDragDropSource();
		}

		if (!m_bFrame && ImGui::IsItemHovered(0) && ImGui::IsMouseReleased(0))
		{
			m_TreeUI->SetSelecteNode(this);															// 클릭체크
		}
			
			if (ImGui::BeginPopupContextItem(strName.c_str(),ImGuiPopupFlags_MouseButtonRight)) // <-- use last item id as popup id
			{
				if(ImGui::Button("Prefab") || m_bPrefab)
				{
					m_bPrefab = true;
					static char name[32] = "";
					char buf[64];
					sprintf_s(buf, "PrefabName: %s###Prefab", name); // ### operator override ID ignoring the preceding label
					if (ImGui::Begin("Name"))
					{
						/*ImGui::Button(buf);
						{*/
							ImGui::Text("Prefab name:");
							ImGui::InputText("##edit", name, IM_ARRAYSIZE(name));
							if (ImGui::Button("OK"))
							{
								ImGui::CloseCurrentPopup();
								m_strDatename = name;
								m_TreeUI->SetPrefabNode(this);
								ContentUI* content = (ContentUI*)CImGuiMgr::GetInst()->FindUI("Content");
								content->ResetContent();
								m_bPrefab = false;
							}
							ImGui::End();
					}	
				}
				if (ImGui::Button("Instantiate") || m_bInstantiate)
				{
					m_bInstantiate = true;
					m_TreeUI->SetPrefabInstiate(this);
					m_bInstantiate = false;

				}
				if (ImGui::Button("Delete"))
				{
					m_TreeUI->Delete(this);

				}
				if (ImGui::Button("Close"))
				{
					ImGui::CloseCurrentPopup();
					m_bPrefab = false;
				}
				
				ImGui::EndPopup();
			}

			

			
		


		// 드랍 체크
		if (ImGui::BeginDragDropTarget())
		{
			m_TreeUI->SetDropTargetNode(this);														// 드랍노드전달

			ImGui::EndDragDropTarget();																			
		}

		
		for (size_t i = 0; i < m_vecChildNode.size(); ++i)
		{
			m_vecChildNode[i]->render_update();																	// 자식노드 업데이트
		}

		ImGui::TreePop();
	}
}





// ======
// TreeUI
// ======
UINT TreeUI::m_iNextNodeIdx = 0;

TreeUI::TreeUI(const string& _strName)
	: UI(_strName)
	, m_RootNode(nullptr)
	, m_bDummyRootUse(false)
	, m_SelectedNode(nullptr)
	, m_BeginDragNode(nullptr)
	, m_DropTargetNode(nullptr)
	, m_SelectInst(nullptr)
	, m_SelectFunc(nullptr)
	, m_DragDropInst(nullptr)
	, m_DragDropFunc(nullptr)
{
}

TreeUI::~TreeUI()
{
	Clear();
}



void TreeUI::update()
{
}

void TreeUI::render_update()
{
	if (nullptr == m_RootNode)
		return;

	if (!m_bDummyRootUse)
	{
		m_RootNode->render_update();
	}
	else
	{
		const vector<TreeNode*>& vecChildNode = m_RootNode->GetChild();
		for (size_t i = 0; i < vecChildNode.size(); ++i)
		{
			vecChildNode[i]->render_update();
		}
	}

	// 마우스 왼쪽 릴리즈 체크
	if (ImGui::IsMouseReleased(0))
	{
		m_BeginDragNode = nullptr;
		m_DropTargetNode = nullptr;
	}
}

TreeNode* TreeUI::AddItem(TreeNode* _parent, const string& _strName, DWORD_PTR _data, bool _IsFrame)
{
	TreeNode* pNode = new TreeNode;
	pNode->SetNodeName(_strName);
	pNode->SetData(_data);
	pNode->SetFrame(_IsFrame);
	pNode->m_TreeUI = this;
	pNode->m_iIdx = m_iNextNodeIdx++;

	// 루트로 지정되는 노드
	if (nullptr == _parent)
	{
		assert(!m_RootNode);
		m_RootNode = pNode;
	}
	else
	{
		_parent->AddChild(pNode);
	}

    return pNode;
}

void TreeUI::Clear()
{
	if (nullptr != m_RootNode)
	{
		delete m_RootNode;
		m_RootNode = nullptr;
	}
}

void TreeUI::SetSelecteNode(TreeNode* _SelectedNode)
{
	if (nullptr != m_SelectedNode)
	{
		m_SelectedNode->m_bSelected = false;
	}
	
	m_SelectedNode = _SelectedNode;
	m_SelectedNode->m_bSelected = true;

	if (m_SelectInst && m_SelectFunc)
	{
		(m_SelectInst->*m_SelectFunc)((DWORD_PTR)m_SelectedNode);
	}
}

void TreeUI::SetDropTargetNode(TreeNode* _DropTargetNode)
{
	m_DropTargetNode = _DropTargetNode;

	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(GetName().c_str()))  // 드래그앤 드롭 이벤트 잡아주는거
	{
		if (m_DragDropInst && m_DragDropFunc)
		{
			(m_DragDropInst->*m_DragDropFunc)((DWORD_PTR)m_BeginDragNode, (DWORD_PTR)m_DropTargetNode);
		}
	}

	// 외부 드랍관련 PayLoad 처리하기
}

void TreeUI::SetPrefabNode(TreeNode* _DropTargetNode)
{

	if (m_RightInst && m_RightFunc)
	{
		(m_RightInst->*m_RightFunc)((DWORD_PTR)_DropTargetNode);
	}
}

void TreeUI::SetPrefabInstiate(TreeNode* _InstiateNode)
{
	if (m_InstiateInst && m_InstiateFunc)
	{
		(m_InstiateInst->*m_InstiateFunc)((DWORD_PTR)_InstiateNode);
	}
}

void TreeUI::Delete(TreeNode* _Node)
{
	if (m_DeleteInst && m_DeleteFunc)
	{
		(m_DeleteInst->*m_DeleteFunc)((DWORD_PTR)_Node);
	}
}


