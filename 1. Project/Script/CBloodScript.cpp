#include "pch.h"
#include "CBloodScript.h"

#include <Engine/CResMgr.h>
#include <Engine/CStructuredBuffer.h>
CBloodScript::CBloodScript()
	:CScript(BLOODSCRIPT)
{
}

CBloodScript::~CBloodScript()
{
}

void CBloodScript::begin()
{
	GetOwner()->Animator2D()->Play(L"1", false);
	//GetOwner()->MeshRender()->GetCurMaterial()->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"katana_zero_texture//bunker_end.png"));
}

void CBloodScript::tick()
{
	//m_TileBuffer->UpdateData(59, PIPELINE_STAGE::PS);

	//if (m_bDataChanged)
	//{
	//	SetTileCount(m_vTileCount.x, m_vTileCount.y);
	//}

	//GetOwner()->MeshRender()->GetCurMaterial()->SetScalarParam(VEC2_2, &m_vTileCount);
}

void CBloodScript::SetTileCount(UINT _iWidth, UINT _iHeight)
{
	m_vTileCount = Vec2((float)_iWidth, (float)_iHeight);

	m_vecTile.clear();
	m_vecTile.resize(_iWidth * _iHeight); // 벡터사이즈를 크기만큼 미리 할당

	if (m_TileBuffer->GetElementCount() < _iWidth * _iHeight)
	{
		delete m_TileBuffer;
		m_TileBuffer = new CStructuredBuffer;
		m_TileBuffer->Create(sizeof(tTile), _iWidth * _iHeight, SB_TYPE::SRV_ONLY, nullptr, true);
		vector<tTile>  m_pvecTile;
		m_pvecTile.resize(m_vTileCount.x * m_vTileCount.y);
		m_TileBuffer->GetData(m_pvecTile.data());
	}
	m_bDataChanged = false;
}




