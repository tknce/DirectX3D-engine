#include "pch.h"
#include "ResUI.h"


ResUI::ResUI(const string& _UIName, RES_TYPE _eType)
    : UI(_UIName)
    , m_eResType(_eType)
{
}

ResUI::~ResUI()
{
}


void ResUI::render_update()
{
    ImGui::PushID(0);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
    ImGui::Button(ToString(m_eResType));
    ImGui::PopStyleColor(3);
    ImGui::PopID();

    // ������ ǥ���Ϸ��� ���ҽ��� �ش� UI �� ��ġ�ϴ� Ÿ������ Ȯ���Ѵ�.
    if (m_eResType != GetTarget()->GetResType())
    {
        assert(nullptr);
    }
}
