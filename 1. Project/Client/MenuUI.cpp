#include "pch.h"
#include "MenuUI.h"

#include <Engine/CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CEventMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CScript.h>
#include <Script\CScriptMgr.h>


#include "CImGuiMgr.h"
#include "InspectorUI.h"
#include "ContentUI.h"
#include "OutLinerUI.h"
#include "CSaveLoadMgr.h"



MenuUI::MenuUI()
	: UI("##MenuUI")
{
}

MenuUI::~MenuUI()
{
}


void MenuUI::render()
{
	
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Level Save"))
			{
				// Level 저장
				CSaveLoadMgr::GetInst()->SaveLevel(CLevelMgr::GetInst()->GetCurLevel(), CLevelMgr::GetInst()->GetCurLevel()->GetName());
				//CSaveLoadMgr::GetInst()->SaveLevel(CLevelMgr::GetInst()->GetCurLevel(), L"Test1.lv");
			}


			
			if (ImGui::MenuItem("Prefab Save"))
			{
				// Level 저장
				vector<CGameObject*> vecobj;
				CLevelMgr::GetInst()->GetCurLevel()->FindObjectByWord(L"UI", vecobj);

				for (size_t i = 0; i < vecobj.size(); ++i)
				{
					CPrefab* a = new CPrefab(vecobj[i]->Clone());
					a->SetName(vecobj[i]->GetName());
					wstring strRelativePath;
					strRelativePath = L"prefab//";
					strRelativePath += a->GetName();
					strRelativePath += L".pref";
					a->Save(strRelativePath);
					//CResMgr::GetInst()->AddRes<CPrefab>(vecobj[i]->GetName(), new CPrefab(vecobj[i]->Clone()));
					
				}
				/*wstring strRelativePath;
				strRelativePath = L"prefab//";
				strRelativePath += pRes->GetKey();
				strRelativePath += L".pref";
				pRes->Save(strRelativePath);*/
				
			}


			ImGui::EndMenu();

		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z"))
			{

			}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Component"))
		{
			InspectorUI* pInspector = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
			CGameObject* pTargetObject = pInspector->GetTargetObject();
			if (ImGui::BeginMenu("AddScript"))
			{
				vector<wstring> vecScriptName;
				CScriptMgr::GetScriptInfo(vecScriptName);

				for (size_t i = 0; i < vecScriptName.size(); ++i)
				{
					string strScriptName = WStringToString(vecScriptName[i]);
					if (ImGui::MenuItem(strScriptName.c_str()))
					{

						if (nullptr != pTargetObject)
						{
							pTargetObject->AddComponent(CScriptMgr::GetScript(vecScriptName[i]));
							pInspector->SetTargetObject(pTargetObject);
						}
					}
				}

				ImGui::EndMenu();
			}
			
			if (ImGui::BeginMenu("DeleteScript"))
			{
				vector<wstring> vecScriptName;
				if (pTargetObject != nullptr)
				{
					vector<CScript*> vecScripts = pTargetObject->GetScripts();
					if (vecScripts.size() != 0)
					{
						for (size_t i = 0; i < vecScripts.size(); ++i)
						{
							CScriptMgr::GetScriptName(vecScripts[i]);
							string strScriptName = WStringToString(CScriptMgr::GetScriptName(vecScripts[i]));
							if (ImGui::MenuItem(strScriptName.c_str()))
							{
								pTargetObject->DeleteScript(vecScripts[i]->GetScriptType());
								InspectorUI* inspector = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
								inspector->SetTargetObject(pTargetObject);
							}

						}
					}
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
			if (ImGui::BeginMenu("Level"))
			{
				LEVEL_STATE State = CLevelMgr::GetInst()->GetLevelState();

				bool PlayEnable = true;
				bool PauseEnable = true;
				bool StopEnable = true;

				if (LEVEL_STATE::PLAY != State) PlayEnable = true; else PlayEnable = false;
				if (LEVEL_STATE::PLAY == State) PauseEnable = true; else  PauseEnable = false;
				if (LEVEL_STATE::STOP != State) StopEnable = true; else StopEnable = false;

				if (ImGui::MenuItem("Play", nullptr, nullptr, PlayEnable))
				{
					tEvent evn = {};
					evn.eType = EVENT_TYPE::CHANGE_LEVEL_STATE;
					evn.wParam = (DWORD_PTR)LEVEL_STATE::PLAY;
					CEventMgr::GetInst()->AddEvent(evn);
				}

				if (ImGui::MenuItem("Pause", nullptr, nullptr, PauseEnable))
				{
					tEvent evn = {};
					evn.eType = EVENT_TYPE::CHANGE_LEVEL_STATE;
					evn.wParam = (DWORD_PTR)LEVEL_STATE::PAUSE;
					CEventMgr::GetInst()->AddEvent(evn);
				}

				if (ImGui::MenuItem("Stop", nullptr, nullptr, StopEnable))
				{
					tEvent evn = {};
					evn.eType = EVENT_TYPE::CHANGE_LEVEL_STATE;
					evn.wParam = (DWORD_PTR)LEVEL_STATE::STOP;
					CEventMgr::GetInst()->AddEvent(evn);
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Resource"))
			{
				if (ImGui::MenuItem("Create Material"))
				{
					Ptr<CMaterial> pMtrl = new CMaterial;
					wstring strKey = CResMgr::GetInst()->GetNewResName<CMaterial>();
					//strKey += L".mtrl";
					CResMgr::GetInst()->AddRes<CMaterial>(strKey, pMtrl.Get());

					ContentUI* pContentUI = (ContentUI*)CImGuiMgr::GetInst()->FindUI("Content");
					pContentUI->ResetContent();
				}

				if (ImGui::MenuItem("Create Material_SetName"))
				{
					Ptr<CMaterial> pMtrl = new CMaterial;
					ImGui::Begin("NewName_mtrl");
					static char rename[32];
					string Name;
					strncpy_s(rename, Name.c_str(), sizeof(Name) - 1);
					ImGui::InputText("##Rename_mtrl", rename, sizeof(rename) - 1);
					Name = rename;


					if (ImGui::Button("OK##mtrl"))
					{
						wstring strKey = wstring(Name.begin(), Name.end());
						strKey += L".mtrl";
						CResMgr::GetInst()->AddRes<CMaterial>(strKey, pMtrl.Get());
						ContentUI* pContentUI = (ContentUI*)CImGuiMgr::GetInst()->FindUI("Content");
						pContentUI->ResetContent();
					}
					ImGui::End();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("GameObject"))
			{
				if (ImGui::MenuItem("Create GameObject"))
				{
					wstring strName = L"newGameobject";
					CGameObject* GameObject = new CGameObject;
					UINT Count = 1;
					while (true)
					{
						wchar_t szBuff[36] = {};
						_itow_s(Count, szBuff, 36);

						wstring strTemp = strName;
						strTemp += szBuff;

						if (nullptr != CLevelMgr::GetInst()->FindObjectByName(strTemp))
						{
							++Count;
						}
						else
						{
							strName = strTemp;
							break;
						}
					}
					GameObject->SetName(strName);
					GameObject->AddComponent(new CTransform);
					CLevel* curlevel = CLevelMgr::GetInst()->GetCurLevel();
					curlevel->AddGameObject(GameObject, 0);

					OutLinerUI* pOutLinerUI = (OutLinerUI*)CImGuiMgr::GetInst()->FindUI("Outliner");
					pOutLinerUI->ResetLevel();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}

void MenuUI::render_update()
{
}