#include "pch.h"
#include "ScriptUI.h"

#include <Script/CScriptMgr.h>
#include <Engine/CScript.h>

#include "ParamUI.h"

ScriptUI::ScriptUI()
	: ComponentUI("##ScriptUI", COMPONENT_TYPE::SCRIPT)
	, m_pTargetScript(nullptr)
{
}

ScriptUI::~ScriptUI()
{

}


void ScriptUI::render_update()
{
    if (m_pTargetScript != nullptr)
    {
        ImGui::PushID(0);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));

        wstring strScriptName = CScriptMgr::GetScriptName(m_pTargetScript);
        ImGui::Button(string(strScriptName.begin(), strScriptName.end()).c_str());

        ImGui::PopStyleColor(3);
        ImGui::PopID();

        render_scriptParam();
    }
}

void ScriptUI::render_scriptParam()
{
    if (m_pTargetScript != nullptr)
    {
        const vector<tScriptParam>& vecParam = m_pTargetScript->GetScriptParam();

        for (size_t i = 0; i < vecParam.size(); ++i)
        {
            switch (vecParam[i].Type)
            {
            case SCRIPT_PARAM::INT:
                break;
            case SCRIPT_PARAM::FLOAT:
                ParamUI::Param_Float(vecParam[i].strParamName, (float*)vecParam[i].pParam);
                break;
            case SCRIPT_PARAM::VEC2:
                break;
            case SCRIPT_PARAM::VEC3:
                break;
            case SCRIPT_PARAM::VEC4:
                break;
            case SCRIPT_PARAM::TEXTURE:
                break;
            case SCRIPT_PARAM::MATERIAL:
                break;
            case SCRIPT_PARAM::PREFAB:
                break;
            case SCRIPT_PARAM::SOUND:
                break;
            default:
                break;
            }

        }
    }
}

