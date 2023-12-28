#include "pch.h"
#include "CTileMap.h"

#include "CResMgr.h"
#include "CKeyMgr.h"
#include "CTransform.h"
#include "CStructuredBuffer.h"

#include "CKeyMgr.h"

CTileMap::CTileMap()
	:CRenderComponent(COMPONENT_TYPE::TILEMAP)
	, m_vLeftTop()
	, m_vSlice()
	, m_bDataChanged(false)
	, m_iIdx()
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TileMapMtrl"), 0);

	m_TileBuffer = new CStructuredBuffer;
	//SetTileAtlas(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\TILE.bmp"));
	

	m_vLeftTop = Vec2(64.f, 64.f);
	m_vSlice = Vec2(64.f, 64.f);
}

CTileMap::CTileMap(const CTileMap& _origin)
	:CRenderComponent(COMPONENT_TYPE::TILEMAP)
	,m_AtlasTex(_origin.m_AtlasTex)
	,m_vTileCount(_origin.m_vTileCount)
	,m_vLeftTop(_origin.m_vLeftTop)
	,m_vSlice(_origin.m_vSlice)
	,m_iIdx(_origin.m_iIdx)
	,m_bDataChanged(false)
{
	m_TileBuffer = new CStructuredBuffer;
	SetTileCount(_origin.m_vTileCount.x, _origin.m_vTileCount.y);
	for (size_t i = 0; i < _origin.m_vecTile.size(); ++i)
	{
		m_vecTile[i] = _origin.m_vecTile[i];
	}
	m_TileBuffer->SetData(m_vecTile.data(), (UINT)(m_vTileCount.x * m_vTileCount.y));
}

CTileMap::~CTileMap()
{
	if (nullptr != m_TileBuffer)
		delete m_TileBuffer;
}




void CTileMap::finaltick()
{
}

void CTileMap::render()
{
	Transform()->UpdateData();

	m_TileBuffer->UpdateData(17, PIPELINE_STAGE::PS);
	//m_TileBuffer->UpdateData(18, PIPELINE_STAGE::PS);

	if (m_bDataChanged)
	{
		SetTileCount(m_vTileCount.x , m_vTileCount.y);
	}


	GetCurMaterial(0)->SetScalarParam(VEC2_0, &m_vLeftTop);
	GetCurMaterial(0)->SetScalarParam(VEC2_1, &m_vSlice);
	GetCurMaterial(0)->SetScalarParam(VEC2_2, &m_vTileCount);
	if (KEY_PRESSED(KEY::LSHIFT))
		m_iSkill = 1;
	else
		m_iSkill = 0;

	GetCurMaterial(0)->SetScalarParam(INT_0, &m_iSkill);


	GetCurMaterial(0)->SetTexParam(TEX_0, m_AtlasTex);
	

	GetCurMaterial(0)->UpdateData();

	GetMesh()->render(0);

}

void CTileMap::render(UINT _iSubset)
{
	render();
}

void CTileMap::SetTileCount(UINT _iWidth, UINT _iHeight)
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
	/*float fWidth = m_AtlasTex->GetWidth();
	float fHeight = m_AtlasTex->GetHeight();

	Vec2 vLeftTop = m_vLeftTop;
	Vec2 vSlice = m_vSlice;

	vLeftTop /= Vec2(fWidth, fHeight);
	vSlice /= Vec2(fWidth, fHeight);

	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		m_vecTile[i].vLeftTop = vLeftTop;
		m_vecTile[i].vSlice = vSlice;
	}

	m_TileBuffer->SetData(m_vecTile.data(), (UINT)(m_vTileCount.x * m_vTileCount.y));

	*/
}

void CTileMap::SetIdxTile(UINT _Idx, Vec2 _LeftTop, Vec2 _Slice, bool _SetUV)
{
	float fWidth = m_AtlasTex->GetWidth();
	float fHeight = m_AtlasTex->GetHeight();

	Vec2 vLeftTop = _LeftTop;
	Vec2 vSlice = _Slice;

	if (_SetUV)
	{
		vLeftTop /= Vec2(fWidth, fHeight);
		vSlice /= Vec2(fWidth, fHeight);
	}
	

	if (m_vecTile.size() > _Idx)
	{
		m_vecTile[_Idx].vLeftTop = vLeftTop;
		m_vecTile[_Idx].vSlice = vSlice;
	}

	m_TileBuffer->SetData(m_vecTile.data(), (UINT)(m_vTileCount.x * m_vTileCount.y));
}

void CTileMap::SaveToFile(FILE* _File)
{
	CRenderComponent::SaveToFile(_File);

	SaveResourceRef<CTexture>(m_AtlasTex, _File);

	fwrite(&m_vTileCount, sizeof(Vec2), 1, _File);
	fwrite(m_vecTile.data(), sizeof(tTile), m_vecTile.size(), _File);
}

void CTileMap::LoadFromFile(FILE* _File)
{
	CRenderComponent::LoadFromFile(_File);

	LoadResourceRef<CTexture>(m_AtlasTex, _File);
	fread(&m_vTileCount, sizeof(Vec2), 1, _File);

	/*wstring alpha;
	alpha = L"a_";
	alpha += m_AtlasTex.Get()->GetName();
	m_AtlasTex_alpha = CResMgr::GetInst()->FindRes<CTexture>(alpha);*/

	size_t iTileCount = (size_t)(m_vTileCount.x * m_vTileCount.y);
	m_vecTile.reserve(iTileCount);
	SetTileCount(m_vTileCount.x, m_vTileCount.y);
	for (size_t i = 0; i < iTileCount; ++i)
	{
		tTile tile = {};
		fread(&tile, sizeof(tTile), 1, _File);
		m_vecTile[i] = tile;
	}
	if (m_vTileCount == Vec2(0, 0))
	{
		m_vTileCount = Vec2(1, 1);
	}
	m_TileBuffer->SetData(m_vecTile.data(), (UINT)(m_vTileCount.x * m_vTileCount.y));
	int a = 0;
}