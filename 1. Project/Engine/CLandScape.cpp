#include "pch.h"
#include "CLandScape.h"
#include "CDevice.h"

#include "CStructuredBuffer.h"

#include "CTransform.h"
#include "CCamera.h"

#include "CRenderMgr.h"
#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CResMgr.h"

CLandScape::CLandScape()
	: CRenderComponent(COMPONENT_TYPE::LANDSCAPE)
	, m_iXFaceCount(1)
	, m_iZFaceCount(1)
	, m_vBrushScale(Vec2(0.3f, 0.3f))
	, m_pCrossBuffer(nullptr)
	, m_pWeightMapBuffer(nullptr)
	, m_iWeightWidth(0)
	, m_iWeightHeight(0)
	, m_iWeightIdx(0)
	, m_eMod(LANDSCAPE_MOD::NONE)
{
	SetFaceCount(1, 1);

	CreateMaterial();

	CreateTexture();

	m_pCrossBuffer = new CStructuredBuffer;
	m_pCrossBuffer->Create(sizeof(tRaycastOut), 1, SB_TYPE::UAV_INC, nullptr, true);

	// 타일 텍스쳐(Color, Normal 혼합, 총 6장)	
	m_pTileArrTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_ARRR.dds", L"texture\\tile\\TILE_ARRR.dds");
	m_pTileArrTex->GenerateMip(8);

	//m_pTileArrTex = CResMgr::GetInst()->LoadTexture(L"TileArrayTex", L"texture\\tile\\TILE_ARRR.dds", 8);

}

CLandScape::~CLandScape()
{
	if (nullptr != m_pCrossBuffer)
		delete m_pCrossBuffer;

	if (nullptr != m_pWeightMapBuffer)
		delete m_pWeightMapBuffer;
}


void CLandScape::finaltick()
{
	if (KEY_TAP(KEY::NUM_0))
		m_eMod = LANDSCAPE_MOD::NONE;
	else if (KEY_TAP(KEY::NUM_1))
		m_eMod = LANDSCAPE_MOD::HEIGHT_MAP;
	else if (KEY_TAP(KEY::NUM_2))
		m_eMod = LANDSCAPE_MOD::SPLAT;

	// 없앨꺼
	if (KEY_TAP(KEY::NUM_6))
	{
		vector<vector<float>> Rgb;
		m_pHeightMap->GetPixelVector_float(Rgb);



		int a = 0;
	}
	
	if (LANDSCAPE_MOD::NONE == m_eMod)
	{
		return;
	}

	Raycasting();



	if (KEY_PRESSED(KEY::LBTN))
	{
		// 픽킹 정보를 system memory 로 가져옴
		// tRaycastOut out = {};
		// m_pCrossBuffer->GetData(&out);

		if (LANDSCAPE_MOD::HEIGHT_MAP == m_eMod)
		{
			// 픽킹 정보를 system memory 로 가져옴
			// tRaycastOut out = {};
			// m_pCrossBuffer->GetData(&out);

			// 교점 위치정보를 토대로 높이를 수정 함
			m_pCSHeightMap->SetInputBuffer(m_pCrossBuffer); // 픽킹 정보를 HeightMapShader 에 세팅
			m_pCSHeightMap->SetBrushTex(m_pBrushTex);		// 사용할 브러쉬 텍스쳐 세팅
			m_pCSHeightMap->SetBrushIndex(0);				// 브러쉬 인덱스 설정
			m_pCSHeightMap->SetBrushScale(m_vBrushScale);   // 브러쉬 크기
			m_pCSHeightMap->SetHeightMap(m_pHeightMap);
			m_pCSHeightMap->Execute();
		}
		else if (LANDSCAPE_MOD::SPLAT == m_eMod)
		{
			// 교점 위치정보를 가중치를 수정함	
			m_pCSWeightMap->SetInputBuffer(m_pCrossBuffer); // 레이 캐스트 위치
			m_pCSWeightMap->SetBrushArrTex(m_pBrushTex);
			m_pCSWeightMap->SetBrushIndex(0);
			m_pCSWeightMap->SetBrushScale(m_vBrushScale); // 브러쉬 크기
			m_pCSWeightMap->SetWeightMap(m_pWeightMapBuffer, m_iWeightWidth, m_iWeightHeight); // 가중치맵, 가로 세로 개수			
			m_pCSWeightMap->SetWeightIdx(m_iWeightIdx);
			m_pCSWeightMap->Execute();
		}
		
	}

	if (KEY_TAP(KEY::RIGHT))
	{
		if (LANDSCAPE_MOD::SPLAT == m_eMod)
		{
			m_iWeightIdx++;
			if (3 <= m_iWeightIdx)
				m_iWeightIdx = 0;
		}
	}

}

void CLandScape::render()
{
	if (nullptr == GetMesh() || nullptr == GetCurMaterial(0))
		return;

	Transform()->UpdateData();

	// 재질 정보 업데이트
	{
		// 가중치 버퍼 전달
		m_pWeightMapBuffer->UpdateData(17, PIPELINE_STAGE::PS);

		// 가중치 버퍼 해상도 전달
		Vec2 vWeightMapResolution = Vec2((float)m_iWeightWidth, (float)m_iWeightHeight);
		GetCurMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_1, &vWeightMapResolution);

		
		// 지형 면 개수 전달
		GetCurMaterial(0)->SetScalarParam(INT_0, &m_iXFaceCount);
		GetCurMaterial(0)->SetScalarParam(INT_1, &m_iZFaceCount);

		// 타일 배열 개수 전달
		float m_fTileCount = float(m_pTileArrTex->GetArraySize() / 2); // 색상, 노말 합쳐져있어서 나누기 2 해줌
		GetCurMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_1, &m_fTileCount);

		tRaycastOut out = {};
		m_pCrossBuffer->GetData(&out);

		//BrushMap                        g_tex_1     // 브러쉬
		//BrushMapUV                      g_vec2_2    // 브러쉬해상도
		//BrushVaild                      g_int_2     // 유효성체크

		GetCurMaterial(0)->SetTexParam(TEX_1, m_pBrushTex);
		GetCurMaterial(0)->SetScalarParam(VEC2_2, &out.vUV);
		GetCurMaterial(0)->SetScalarParam(VEC2_3, m_pBrushTex->GetResolution());
		GetCurMaterial(0)->SetScalarParam(INT_2, &out.bSuccess);
		



		// 타일 텍스쳐 전달
		GetCurMaterial(0)->SetTexParam(TEX_PARAM::TEX_ARR_0, m_pTileArrTex);

		// 업데이트
		GetCurMaterial(0)->UpdateData();
	}

	GetMesh()->render(0);

	// Clear
	CMaterial::Clear();
	m_pWeightMapBuffer->Clear();
}

void CLandScape::render(UINT _iSubset)
{
	render();
}

void CLandScape::SetFaceCount(UINT _X, UINT _Z)
{
	m_iXFaceCount = _X;
	m_iZFaceCount = _Z;

	// 면 개수에 맞는 메시 생성
	CreateMesh();
}

void CLandScape::SetHightBrushArrTex(vector<Ptr<CTexture>> tex)
{
	if (tex[0].Get() != nullptr)
		tex[0] = m_pBrushTex;
	if (tex[1].Get() != nullptr)
		tex[1] = m_pHeightMap;
	if (tex[2].Get() != nullptr)
		tex[2] = m_pTileArrTex;
}

vector<Ptr<CTexture>> CLandScape::GetHightBrushArrTex()
{
	vector<Ptr<CTexture>> Tex;
	Tex.push_back(m_pBrushTex);
	Tex.push_back(m_pHeightMap);
	Tex.push_back(m_pTileArrTex);
	return Tex;
}


void CLandScape::Raycasting()
{
	// 시점 카메라를 가져옴
	CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();
	if (nullptr == pMainCam)
		return;

	// 월드 기준 광선을 지형의 로컬로 보냄
	const Matrix& matWorldInv = Transform()->GetWorldInvMat();
	const tRay& ray = pMainCam->GetRay();

	tRay CamRay = {};
	CamRay.vStart = XMVector3TransformCoord(ray.vStart, matWorldInv);
	CamRay.vDir = XMVector3TransformNormal(ray.vDir, matWorldInv);
	CamRay.vDir.Normalize();

	// 지형과 카메라 Ray 의 교점을 구함
	tRaycastOut out = { Vec2(0.f, 0.f), 0x7fffffff, 0 };
	m_pCrossBuffer->SetData(&out, 1);

	m_pCSRaycast->SetHeightMap(m_pHeightMap);
	m_pCSRaycast->SetFaceCount(m_iXFaceCount, m_iZFaceCount);
	m_pCSRaycast->SetCameraRay(CamRay);
	m_pCSRaycast->SetOuputBuffer(m_pCrossBuffer);

	m_pCSRaycast->Execute();
}

void CLandScape::SaveToFile(FILE* _File)
{
	COMPONENT_TYPE type = GetType();
	fwrite(&type, sizeof(UINT), 1, _File);

	// 면 개수 저장
	fwrite(&m_iXFaceCount, sizeof(UINT), 1, _File);
	fwrite(&m_iZFaceCount, sizeof(UINT), 1, _File);
}

void CLandScape::LoadFromFile(FILE* _File)
{


	// 면 개수 불러오기
	fread(&m_iXFaceCount, sizeof(UINT), 1, _File);
	fread(&m_iZFaceCount, sizeof(UINT), 1, _File);

	SetFaceCount(m_iXFaceCount, m_iZFaceCount);
}
