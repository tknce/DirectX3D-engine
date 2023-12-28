#include "pch.h"
#include "Animator3DUI.h"

#include "CImGuiMgr.h"
#include "ListUI.h"
#include "CEditor.h"
#include "CGameObjectEx.h"

#include <Engine/CTransform.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CGameObject.h>

Animator3DUI::Animator3DUI()
	: ComponentUI("Animator3DUI", COMPONENT_TYPE::ANIMATOR3D)
{
}

Animator3DUI::~Animator3DUI()
{
}


void Animator3DUI::update()
{
	ComponentUI::update();
	if (GetTarget() != nullptr)
	{
		m_pEntireClip = GetTarget()->Animator3D()->GetentireAnimClip();
		// AnimClip = GetTarget()->Animator3D()->GetAnimlist();
		m_dCurTime = GetTarget()->Animator3D()->GetCurTime();
	}
	
}

void Animator3DUI::render_update()
{
	ComponentUI::render_update();
	static double timestart = m_pEntireClip->at(0).dStartTime;
	static double timeend = m_pEntireClip->at(0).dEndTime;
	static double timelength = m_pEntireClip->at(0).dTimeLength;
	static int framestart = m_pEntireClip->at(0).iStartFrame;
	static int fremeend = m_pEntireClip->at(0).iEndFrame;
	static int fremelength = m_pEntireClip->at(0).iFrameLength;

	ImGui::Text("Time");
	ImGui::Text("Starttime "); ImGui::SameLine(); ImGui::InputDouble("##ani3Dstart", &timestart);
	ImGui::Text("Endtime   "); ImGui::SameLine(); ImGui::InputDouble("##ani3endD", &timeend);
	
	ImGui::Text("frame");
	ImGui::Text("Startframe"); ImGui::SameLine(); ImGui::InputInt("##ani3Dfstart", &framestart);
	ImGui::Text("Endframe  "); ImGui::SameLine(); ImGui::InputInt("##ani3fendD", &fremeend);
	ImGui::Text("CurTime  "); ImGui::SameLine(); ImGui::InputDouble("##ani3DcurTime", &m_dCurTime);
	m_dCurTime *= 30;
	ImGui::Text("Curframe  "); ImGui::SameLine(); ImGui::InputDouble("##ani3DcurFrm", &m_dCurTime);
	
	if (ImGui::Button("Play")) 
	{
		GetTarget()->Animator3D()->SetStop(false);
		if (GetTarget()->GetChildObject().size() != 0)
		{
			vector<CGameObject*> childobj = GetTarget()->GetChildObject();
			for (int i = 0; i < childobj.size(); ++i)
			{
				if (childobj[i]->Animator3D() != nullptr)
				{
					childobj[i]->Animator3D()->SetStop(false);
				}
			}
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop"))
	{
		GetTarget()->Animator3D()->SetStop(true);
		vector<CGameObject*> childobj = GetTarget()->GetChildObject();
		for (int i = 0; i < childobj.size(); ++i)
		{
			if (childobj[i]->Animator3D() != nullptr)
			{
				childobj[i]->Animator3D()->SetStop(true);
			}
		}
	}
	ImGui::SameLine();
	Show();

	ImGui::SameLine();
	Delete();

	ImGui::SameLine();
	Reset();


	if (ImGui::Button("<-"))
	{		
		GetTarget()->Animator3D()->SetTime(-0.1f);
		if (GetTarget()->GetChildObject().size() != 0)
		{
			vector<CGameObject*> childobj = GetTarget()->GetChildObject();
			for (int i = 0; i < childobj.size(); ++i)
			{
				if (childobj[i]->Animator3D() != nullptr)
				{
					childobj[i]->Animator3D()->SetTime(-0.1f);;
				}
			}
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("->"))
	{
		GetTarget()->Animator3D()->SetTime(0.1f);
		if (GetTarget()->GetChildObject().size() != 0)
		{
			vector<CGameObject*> childobj = GetTarget()->GetChildObject();
			for (int i = 0; i < childobj.size(); ++i)
			{
				if (childobj[i]->Animator3D() != nullptr)
				{
					childobj[i]->Animator3D()->SetTime(0.1f);
				}
			}
		}
	}

	if (ImGui::Button("list##listBtn_ani3d", Vec2(45.f, 15.f)))
	{
		ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUI"));
		assert(pListUI);

		map<wstring, CAnimation3D*> mapRes = GetTarget()->Animator3D()->GetAnimlist();

		static vector<wstring> vecRes;
		vecRes.clear();

		map<wstring, CAnimation3D*>::iterator iter = mapRes.begin();
		for (; iter != mapRes.end(); ++iter)
		{
			vecRes.push_back(iter->first);
		}
		pListUI->SetItemList(vecRes);
		pListUI->AddDynamicDBClicked(this, (FUNC_1)&Animator3DUI::SetCurAnimation);

		pListUI->Open();
	}
		
	if (ImGui::Checkbox("Animation Mode", &m_bCreate) || m_bCreate)
	{
		Create(m_bCreate);
	}
}

void Animator3DUI::Show()
{
	if (ImGui::Button("Show"))
	{
		map<wstring, CAnimation3D*> mapRes = GetTarget()->Animator3D()->GetAnimlist();
		int i = 0;
		for (std::pair<wstring, CAnimation3D*> elem : mapRes)
		{
			if (m_mapAnimenu.find(elem.first) == m_mapAnimenu.end())
			{
				m_mapAnimenu.insert(make_pair(elem.first, GetTarget()->Clone()));
				Instantiate(m_mapAnimenu.find(elem.first)->second, Vec3(i * 500, 0, 0));
				m_mapAnimenu.find(elem.first)->second->Animator3D()->SetAnimation3D(elem.first, AnimClip);
				m_mapAnimenu.find(elem.first)->second->Animator3D()->Play(elem.first);
			}
			++i;
		}
	}
}

void Animator3DUI::Delete()
{
	if (ImGui::Button("Delete"))
	{
		for (std::pair<wstring, CGameObject*> _map : m_mapAnimenu)
		{
			_map.second->Destroy();
		}
	}
}

void Animator3DUI::Reset()
{
	if (ImGui::Button("Reset"))
	{
		GetTarget()->Animator3D()->SetClipTime(0, 0);
		for (int i = 0; i < GetTarget()->GetChildObject().size(); ++i)
		{
			if(GetTarget()->GetChildObject()[i]->Animator3D() != nullptr)
			GetTarget()->GetChildObject()[i]->Animator3D()->SetClipTime(0, 0);
		}
	}
}



void Animator3DUI::Create(bool open)
{
	ImGui::Begin("Ani3D", &open);

	static char name[32];
	static wstring NewName;
	// 이름 생성
	if (ImGui::InputText("##Anim", name, IM_ARRAYSIZE(name)))
	{
		m_newStr = name;
		NewName = wstring(m_newStr.begin(), m_newStr.end());
	}

	// 시간 설정
	ImGui::Text("Time");
	ImGui::Text("StartTime"); ImGui::SameLine(); 
	if (ImGui::InputDouble("##ani3Dstart", &AnimClip.dStartTime))
	{
		AnimClip.iStartFrame = AnimClip.dStartTime * 30;
	}
	ImGui::Text("EndTimeD"); ImGui::SameLine(); 
	if (ImGui::InputDouble("##ani3endD", &AnimClip.dEndTime))
	{
		AnimClip.iEndFrame = AnimClip.dEndTime * 30;
	}


	// 프레임 설정
	ImGui::Text("frame");
	ImGui::Text("StartFrame"); ImGui::SameLine(); 
	if (ImGui::InputInt("##ani3Dfstart", &AnimClip.iStartFrame))
	{
		AnimClip.dStartTime = AnimClip.iStartFrame / 30;
	}
	ImGui::Text("EndFrame"); ImGui::SameLine(); 
	if (ImGui::InputInt("##ani3fendD", &AnimClip.iEndFrame))
	{
		AnimClip.dEndTime = AnimClip.iEndFrame / 30;
	}


	// 깔바꿈
	if (ImGui::Button("Set", ImVec2(35.f, 25.f)))
	{
		AnimClip.dTimeLength = AnimClip.dEndTime - AnimClip.dStartTime;
		AnimClip.iFrameLength = AnimClip.iEndFrame - AnimClip.iStartFrame;
		AnimClip.eMode = m_pEntireClip->at(0).eMode;
		if(NewName.size() < 0)
		AnimClip.strAnimName = L"3D";

		AnimClip.strAnimName = NewName;
		GetTarget()->Animator3D()->SetAnimation3D(AnimClip.strAnimName, AnimClip);
		
		if (GetTarget()->GetChildObject().size() != 0)
		{
			vector<CGameObject*> childobj = GetTarget()->GetChildObject();
			for (int i = 0; i < childobj.size(); ++i)
			{
				if (childobj[i]->Animator3D() != nullptr)
				{
					childobj[i]->Animator3D()->SetAnimation3D(AnimClip.strAnimName, AnimClip);
				}
			}
		}

		m_bCreate = false;
	}

	ImGui::End();
	
}

void Animator3DUI::SetCurAnimation(DWORD_PTR _str)
{
	string strKey = (char*)_str;
	wstring wstrKey = wstring(strKey.begin(), strKey.end());

	GetTarget()->Animator3D()->Play(wstrKey);


	if (GetTarget()->GetChildObject().size() != 0)
	{
		vector<CGameObject*> childobj = GetTarget()->GetChildObject();
		for (int i = 0; i < childobj.size(); ++i)
		{
			if (childobj[i]->Animator3D() != nullptr)
			{
				childobj[i]->Animator3D()->Play(wstrKey);;
			}
		}
	}
}
