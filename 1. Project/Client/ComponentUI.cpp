#include "pch.h"
#include "ComponentUI.h"

#include <Engine\CGameObject.h>
#include <Engine/CMeshRender.h>
#include <Engine/CTileMap.h>
#include <Engine/CTexture.h>

#include "ListUI.h"
#include "CImGuiMgr.h"

ComponentUI::ComponentUI(const string& _UIName, COMPONENT_TYPE _eType)
	: UI(_UIName)
	, m_TargetObj(nullptr)
	, m_eType(_eType)	
    , m_bDrag(true)
{
}

ComponentUI::~ComponentUI()
{
}


void ComponentUI::IsDrag(ImVec2 _LeftTop)
{
    if (m_bDrag && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        m_vLeftTop = _LeftTop;
        m_bDrag = false;
    }
    if(!m_bDrag)
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 a = ImGui::GetCursorPos();
        draw_list->AddRect(m_vLeftTop, ImVec2(_LeftTop.x, _LeftTop.y), IM_COL32(0, 255, 0, 255));
    }   
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
    {
        m_bDrag = true;
    }
}

void ComponentUI::render_update()
{


    ImGui::PushID(0);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
    ImGui::Button(ToString(m_eType));
    ImGui::PopStyleColor(3);
    ImGui::PopID();
}



