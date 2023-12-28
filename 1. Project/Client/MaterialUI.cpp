#include "pch.h"
#include "MaterialUI.h"

#include <Engine\CMaterial.h>
#include <Engine\CResMgr.h>

#include "CImGuiMgr.h"
#include "ParamUI.h"
#include "ListUI.h"

MaterialUI::MaterialUI()
	: ResUI("Material##UI", RES_TYPE::MATERIAL)
{
}

MaterialUI::~MaterialUI()
{
}

void MaterialUI::update()
{
	ResUI::update();
}

void MaterialUI::render_update()
{
	ResUI::render_update();

	CMaterial* pMtrl = ((CMaterial*)GetTarget().Get());

	string strKey = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());
	string strRelativePath = string(pMtrl->GetRelativePath().begin(), pMtrl->GetRelativePath().end());

	ImGui::Text("Key           ");
	ImGui::SameLine();
	ImGui::InputText("##MtrlKey", (char*)strKey.data(), strKey.length(), ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Path          ");
	ImGui::SameLine();
	ImGui::InputText("##MtrlPath", (char*)strRelativePath.data(), strRelativePath.length(), ImGuiInputTextFlags_ReadOnly);

	string strShaderKey;
	if (nullptr != pMtrl->GetShader())
	{
		strShaderKey = string(pMtrl->GetShader()->GetKey().begin(), pMtrl->GetShader()->GetKey().end());
	}

	ImGui::Text("GraphicsShader");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##ShaderName", (char*)strShaderKey.data(), strShaderKey.length(), ImGuiInputTextFlags_ReadOnly);

	// 쉐이더 변경 및 선택
	ImGui::SameLine();
	if (ImGui::Button("##ShaderBtn", Vec2(15.f, 15.f)))
	{
		ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUI"));
		assert(pListUI);

		// 목록을 받아와서, ListUI 에 전달
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::GRAPHICS_SHADER);
		static vector<wstring> vecRes;
		vecRes.clear();

		map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
		for (; iter != mapRes.end(); ++iter)
		{
			vecRes.push_back(iter->first);
		}
		pListUI->SetItemList(vecRes);
		pListUI->AddDynamicDBClicked(this, (FUNC_1)&MaterialUI::SetShader);

		pListUI->Open();
	}


	ImGui::NewLine();
	ImGui::Text("Shader Parameter");

	// Material 에 Shader 가 세팅되지 않았다면
	if (nullptr == pMtrl->GetShader())
	{
		return;
	}

	const vector<tScalarParam> vecScalar = pMtrl->GetShader()->GetScalarParam();
	for (size_t i = 0; i < vecScalar.size(); ++i)
	{
		switch (vecScalar[i].eParam)
		{
		case INT_0:
		case INT_1:
		case INT_2:
		case INT_3:
		{
			int iInput = 0;
			pMtrl->GetScalarParam(vecScalar[i].eParam, &iInput);
			if (ParamUI::Param_Int(vecScalar[i].strName, &iInput))
			{
				pMtrl->SetScalarParam(vecScalar[i].eParam, &iInput);
			}
		}
		break;
		case FLOAT_0:
		case FLOAT_1:
		case FLOAT_2:
		case FLOAT_3:
		{
			float fData = 0;
			pMtrl->GetScalarParam(vecScalar[i].eParam, &fData);
			ParamUI::Param_Float(vecScalar[i].strName, &fData);
			pMtrl->SetScalarParam(vecScalar[i].eParam, &fData);
		}
		break;
		case VEC2_0:
		case VEC2_1:
		case VEC2_2:
		case VEC2_3:
		{
			Vec2 data;
			pMtrl->GetScalarParam(vecScalar[i].eParam, &data);
			ParamUI::Param_Vec2(vecScalar[i].strName, &data);
			pMtrl->SetScalarParam(vecScalar[i].eParam, &data);
		}
		break;
		case VEC4_0:
		case VEC4_1:
		case VEC4_2:
		case VEC4_3:
		{
			Vec4 data;
			pMtrl->GetScalarParam(vecScalar[i].eParam, &data);
			ParamUI::Param_Vec4(vecScalar[i].strName, &data);
			pMtrl->SetScalarParam(vecScalar[i].eParam, &data);
		}
		break;
		case MAT_0:
		case MAT_1:
		case MAT_2:
		case MAT_3:
		{

		}
		break;
		}
	}

	const vector<tTextureParam> vecTex = pMtrl->GetShader()->GetTextureParam();
	for (size_t i = 0; i < vecTex.size(); ++i)
	{
		Ptr<CTexture> pTex = pMtrl->GetTexParam(vecTex[i].eParam);
		if (ParamUI::Param_Tex(vecTex[i].strName, pTex, this, (FUNC_1)&MaterialUI::SetTexture))
		{
			m_eSelectTexParam = vecTex[i].eParam;
		}
		else
		{
			pMtrl->SetTexParam(vecTex[i].eParam, pTex);
		}
	}

}

void MaterialUI::SetTexture(DWORD_PTR _strTexKey)
{
	string strKey = (char*)_strTexKey;
	wstring wstrKey = wstring(strKey.begin(), strKey.end());

	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(wstrKey);
	assert(nullptr != pTex);

	CMaterial* pMtrl = ((CMaterial*)GetTarget().Get());
	pMtrl->SetTexParam(m_eSelectTexParam, pTex);
}

void MaterialUI::SetShader(DWORD_PTR _strShaderKey)
{
	string strkey = (char*)_strShaderKey;
	wstring wstrkey = wstring(strkey.begin(), strkey.end());

	Ptr<CGraphicsShader> pShader = CResMgr::GetInst()->FindRes<CGraphicsShader>(wstrkey);
	assert(nullptr != pShader);

	((CMaterial*)GetTarget().Get())->SetShader(pShader);

}


