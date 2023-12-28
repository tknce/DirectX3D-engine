#include "pch.h"
#include "CResMgr.h"

#include "CDevice.h"

namespace
{
	inline Vec3 GetCircleVector(size_t i, size_t tessellation) noexcept
	{
		const float angle = float(i) * XM_2PI / float(tessellation);
		float       sin;
		float       cos;

		XMScalarSinCos(&sin, &cos, angle);
		return { cos, sin, 0 };
	}
}

void CResMgr::init()
{
	InitSound();

	CreateDefaultMesh();

	CreateDefaultTexture();

	CreateDefaultGraphicsShader();

	CreateDefaultComputeShader();

	CreateDefaultMaterial();
}

void CResMgr::InitSound()
{
	FMOD::System_Create(&CSound::g_pFMOD);

	if (nullptr == CSound::g_pFMOD)
	{
		assert(nullptr);
	}

	// 32개 채널 생성
	CSound::g_pFMOD->init(32, FMOD_DEFAULT, nullptr);
}

void CResMgr::CreateDefaultMesh()
{
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;
	CMesh* pMesh = nullptr;

	// Point Mesh
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 1.f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);

	UINT idx = 0;

	pMesh = new CMesh(true);
	pMesh->Create(&v, 1, &idx, 1);
	AddRes<CMesh>(L"PointMesh", pMesh);	


	// 사각형 메쉬 만들기
	v.vPos = Vec3(-0.5f, 0.5f, 0.f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 1.f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 1.f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 1.f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 1.f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVtx.push_back(v);

	// Index
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);

	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"RectMesh", pMesh);	
	vecIdx.clear();





	// Index
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);	

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"RectMesh_Debug", pMesh);
	vecVtx.clear();
	vecIdx.clear();


	// 원형메쉬 만들기
	// 중심점	
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);

	vecVtx.push_back(v);

	int iSlice = 40;
	float fRadius = 0.5f;
	float fTheta = XM_2PI / (float)iSlice;

	for (int i = 0; i < iSlice; ++i)
	{
		v.vPos = Vec3(fRadius * cosf(fTheta * (float)i), fRadius * sinf(fTheta * (float)i), 0.f);
		v.vNormal = Vec3(0.f, 0.f, -1.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vUV = Vec2(v.vPos.x + 0.5f, 1.f - (v.vPos.y + 0.5f));

		vecVtx.push_back(v);
	}

	for (UINT i = 0; i < (UINT)iSlice; ++i)
	{
		vecIdx.push_back(0);
		if (i == iSlice - 1)
		{
			vecIdx.push_back(1);
		}
		else
		{
			vecIdx.push_back(i + 2);
		}
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"CircleMesh", pMesh);	
	vecIdx.clear();


	// Cicle_debug
	for (int i = 0; i < vecVtx.size() - 2; ++i)
	{		
		vecIdx.push_back(i + 1);				
	}
	vecIdx.push_back(1);

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"CircleMesh_Debug", pMesh);
	vecVtx.clear();
	vecIdx.clear();


	// Cube Mesh
	Vtx arrCube[24] = {};
	// 윗면
	arrCube[0].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[0].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[0].vUV = Vec2(0.f, 0.f);
	arrCube[0].vNormal = Vec3(0.f, 1.f, 0.f);
	arrCube[0].vTangent = Vec3(0.f, 0.f, 1.f);
	arrCube[0].vBinormal = Vec3(1.f, 0.f, 0.f);

	arrCube[1].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[1].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[1].vUV = Vec2(0.f, 0.f);
	arrCube[1].vNormal = Vec3(0.f, 1.f, 0.f);
	arrCube[1].vTangent = Vec3(0.f, 0.f, 1.f);
	arrCube[1].vBinormal = Vec3(1.f, 0.f, 0.f);

	arrCube[2].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[2].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[2].vUV = Vec2(0.f, 0.f);
	arrCube[2].vNormal = Vec3(0.f, 1.f, 0.f);
	arrCube[2].vTangent = Vec3(0.f, 0.f, 1.f);
	arrCube[2].vBinormal = Vec3(1.f, 0.f, 0.f);

	arrCube[3].vPos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[3].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[3].vUV = Vec2(0.f, 0.f);
	arrCube[3].vNormal = Vec3(0.f, 1.f, 0.f);
	arrCube[3].vTangent = Vec3(0.f, 0.f, 1.f);
	arrCube[3].vBinormal = Vec3(1.f, 0.f, 0.f);


	// 아랫 면	
	arrCube[4].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[4].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[4].vUV = Vec2(0.f, 0.f);
	arrCube[4].vNormal = Vec3(0.f, -1.f, 0.f);
	arrCube[4].vTangent = Vec3(0.f, 0.f, -1.f);
	arrCube[4].vBinormal = Vec3(1.f, 0.f, 0.f);


	arrCube[5].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[5].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[5].vUV = Vec2(0.f, 0.f);
	arrCube[5].vNormal = Vec3(0.f, -1.f, 0.f);
	arrCube[5].vTangent = Vec3(0.f, 0.f, -1.f);
	arrCube[5].vBinormal = Vec3(1.f, 0.f, 0.f);

	arrCube[6].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[6].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[6].vUV = Vec2(0.f, 0.f);
	arrCube[6].vNormal = Vec3(0.f, -1.f, 0.f);
	arrCube[6].vTangent = Vec3(0.f, 0.f, -1.f);
	arrCube[6].vBinormal = Vec3(1.f, 0.f, 0.f);

	arrCube[7].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[7].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[7].vUV = Vec2(0.f, 0.f);
	arrCube[7].vNormal = Vec3(0.f, -1.f, 0.f);
	arrCube[7].vTangent = Vec3(0.f, 0.f, -1.f);
	arrCube[7].vBinormal = Vec3(1.f, 0.f, 0.f);

	// 왼쪽 면
	arrCube[8].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[8].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[8].vUV = Vec2(0.f, 0.f);
	arrCube[8].vNormal = Vec3(-1.f, 0.f, 0.f);
	arrCube[8].vTangent = Vec3(0.f, 1.f, 0.f);
	arrCube[8].vBinormal = Vec3(0.f, 0.f, -1.f);

	arrCube[9].vPos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[9].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[9].vUV = Vec2(0.f, 0.f);
	arrCube[9].vNormal = Vec3(-1.f, 0.f, 0.f);
	arrCube[9].vTangent = Vec3(0.f, 1.f, 0.f);
	arrCube[9].vBinormal = Vec3(0.f, 0.f, -1.f);

	arrCube[10].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[10].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[10].vUV = Vec2(0.f, 0.f);
	arrCube[10].vNormal = Vec3(-1.f, 0.f, 0.f);
	arrCube[10].vTangent = Vec3(0.f, 1.f, 0.f);
	arrCube[10].vBinormal = Vec3(0.f, 0.f, -1.f);

	arrCube[11].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[11].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[11].vUV = Vec2(0.f, 0.f);
	arrCube[11].vNormal = Vec3(-1.f, 0.f, 0.f);
	arrCube[11].vTangent = Vec3(0.f, 1.f, 0.f);
	arrCube[11].vBinormal = Vec3(0.f, 0.f, -1.f);

	// 오른쪽 면
	arrCube[12].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[12].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[12].vUV = Vec2(0.f, 0.f);
	arrCube[12].vNormal = Vec3(1.f, 0.f, 0.f);
	arrCube[12].vTangent = Vec3(0.f, 1.f, 0.f);
	arrCube[12].vBinormal = Vec3(0.f, 0.f, 1.f);

	arrCube[13].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[13].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[13].vUV = Vec2(0.f, 0.f);
	arrCube[13].vNormal = Vec3(1.f, 0.f, 0.f);
	arrCube[13].vTangent = Vec3(0.f, 1.f, 0.f);
	arrCube[13].vBinormal = Vec3(0.f, 0.f, 1.f);

	arrCube[14].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[14].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[14].vUV = Vec2(0.f, 0.f);
	arrCube[14].vNormal = Vec3(1.f, 0.f, 0.f);
	arrCube[14].vTangent = Vec3(0.f, 1.f, 0.f);
	arrCube[14].vBinormal = Vec3(0.f, 0.f, 1.f);

	arrCube[15].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[15].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[15].vUV = Vec2(0.f, 0.f);
	arrCube[15].vNormal = Vec3(1.f, 0.f, 0.f);
	arrCube[15].vTangent = Vec3(0.f, 1.f, 0.f);
	arrCube[15].vBinormal = Vec3(0.f, 0.f, 1.f);

	// 뒷 면
	arrCube[16].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[16].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[16].vUV = Vec2(0.f, 0.f);
	arrCube[16].vNormal = Vec3(0.f, 0.f, 1.f);
	arrCube[16].vTangent = Vec3(0.f, 1.f, 0.f);
	arrCube[16].vBinormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[17].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[17].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[17].vUV = Vec2(0.f, 0.f);
	arrCube[17].vNormal = Vec3(0.f, 0.f, 1.f);
	arrCube[17].vTangent = Vec3(0.f, 1.f, 0.f);
	arrCube[17].vBinormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[18].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[18].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[18].vUV = Vec2(0.f, 0.f);
	arrCube[18].vNormal = Vec3(0.f, 0.f, 1.f);
	arrCube[18].vTangent = Vec3(0.f, 1.f, 0.f);
	arrCube[18].vBinormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[19].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[19].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[19].vUV = Vec2(0.f, 0.f);
	arrCube[19].vNormal = Vec3(0.f, 0.f, 1.f);
	arrCube[19].vTangent = Vec3(0.f, 1.f, 0.f);
	arrCube[19].vBinormal = Vec3(-1.f, 0.f, 0.f);

	// 앞 면
	arrCube[20].vPos = Vec3(-0.5f, 0.5f, -0.5f);;
	arrCube[20].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[20].vUV = Vec2(0.f, 0.f);
	arrCube[20].vNormal = Vec3(0.f, 0.f, -1.f);
	arrCube[19].vTangent = Vec3(0.f, -1.f, 0.f);
	arrCube[19].vBinormal = Vec3(1.f, 0.f, 0.f);

	arrCube[21].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[21].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[21].vUV = Vec2(0.f, 0.f);
	arrCube[21].vNormal = Vec3(0.f, 0.f, -1.f);
	arrCube[21].vTangent = Vec3(0.f, -1.f, 0.f);
	arrCube[21].vBinormal = Vec3(1.f, 0.f, 0.f);

	arrCube[22].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[22].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[22].vUV = Vec2(0.f, 0.f);
	arrCube[22].vNormal = Vec3(0.f, 0.f, -1.f);
	arrCube[22].vTangent = Vec3(0.f, -1.f, 0.f);
	arrCube[22].vBinormal = Vec3(1.f, 0.f, 0.f);

	arrCube[23].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[23].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[23].vUV = Vec2(0.f, 0.f);
	arrCube[23].vNormal = Vec3(0.f, 0.f, -1.f);
	arrCube[23].vTangent = Vec3(0.f, -1.f, 0.f);
	arrCube[23].vBinormal = Vec3(1.f, 0.f, 0.f);

	// 인덱스
	for (int i = 0; i < 12; i += 2)
	{
		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 1);
		vecIdx.push_back(i * 2 + 2);

		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 2);
		vecIdx.push_back(i * 2 + 3);
	}

	pMesh = new CMesh(true);
	pMesh->Create(arrCube, 24, vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"CubeMesh", pMesh);
	vecIdx.clear();

	// ===========
	// Sphere Mesh
	// ===========
	fRadius = 0.5f;

	// Top
	v.vPos = Vec3(0.f, fRadius, 0.f);
	v.vUV = Vec2(0.5f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, 1.f);
	vecVtx.push_back(v);

	// Body
	UINT iStackCount = 40; // 가로 분할 개수
	UINT iSliceCount = 40; // 세로 분할 개수

	float fStackAngle = XM_PI / iStackCount;
	float fSliceAngle = XM_2PI / iSliceCount;

	float fUVXStep = 1.f / (float)iSliceCount;
	float fUVYStep = 1.f / (float)iStackCount;

	for (UINT i = 1; i < iStackCount; ++i)
	{
		float phi = i * fStackAngle;

		for (UINT j = 0; j <= iSliceCount; ++j)
		{
			float theta = j * fSliceAngle;

			v.vPos = Vec3(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
				, fRadius * cosf(i * fStackAngle)
				, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle));
			v.vUV = Vec2(fUVXStep * j, fUVYStep * i);
			v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
			v.vNormal = v.vPos;
			v.vNormal.Normalize();

			v.vTangent.x = -fRadius * sinf(phi) * sinf(theta);
			v.vTangent.y = 0.f;
			v.vTangent.z = fRadius * sinf(phi) * cosf(theta);
			v.vTangent.Normalize();

			v.vTangent.Cross(v.vNormal, v.vBinormal);
			v.vBinormal.Normalize();

			vecVtx.push_back(v);
		}
	}

	// Bottom
	v.vPos = Vec3(0.f, -fRadius, 0.f);
	v.vUV = Vec2(0.5f, 1.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();

	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, -1.f);
	vecVtx.push_back(v);

	// 인덱스
	// 북극점
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	// 몸통
	for (UINT i = 0; i < iStackCount - 2; ++i)
	{
		for (UINT j = 0; j < iSliceCount; ++j)
		{
			// + 
			// | \
			// +--+
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j)+1);

			// +--+
			//  \ |
			//    +
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i)+(j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
		}
	}

	// 남극점
	UINT iBottomIdx = (UINT)vecVtx.size() - 1;
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(iBottomIdx);
		vecIdx.push_back(iBottomIdx - (i + 2));
		vecIdx.push_back(iBottomIdx - (i + 1));
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"SphereMesh", pMesh);
	vecVtx.clear();
	vecIdx.clear();

	// 콘메쉬
	CreateconeMesh(1.f, 1.f, 32);

	//				1 ------ 2
	//			   /|      / |  Far
	//			  / |     /  |
	//			 /  4 --/--- 3	
	//			/  /   /   /
	//		   /  /  /   /
	//	      /  / /  /
	//	     /  /  /
	//	    / / /
	//	   ///
	//	  //
	//	 /
	//  0
	//      Near
	// 
	// frustum Mesh
	Vtx arrfrustum[8] = {};
	arrfrustum[0].vPos = Vec3(0.f, 0.f, 0.f);
	arrfrustum[0].vUV = Vec2(0.5f, 0.5f);
	
	arrfrustum[1].vPos = Vec3(-1.f, 1.f, 1.f);
	arrfrustum[1].vUV = Vec2(0.5f, 0.5f);

	arrfrustum[2].vPos = Vec3(1.f, 1.f, 1.f);
	arrfrustum[2].vUV = Vec2(0.5f, 0.5f);

	arrfrustum[3].vPos = Vec3(1.f, -1.f, 1.f);
	arrfrustum[3].vUV = Vec2(0.5f, 0.5f);

	arrfrustum[4].vPos = Vec3(-1.f, -1.f, 1.f);
	arrfrustum[4].vUV = Vec2(0.5f, 0.5f);



	for (size_t i = 0; i < 5; ++i)
	{
		vecVtx.push_back(arrfrustum[i]);
	}

	// 윗면
	vecIdx.push_back(1);
	vecIdx.push_back(0);
	vecIdx.push_back(2);

	// 오른면
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);

	// 아랫면
	vecIdx.push_back(0);
	vecIdx.push_back(4);
	vecIdx.push_back(3);

	// 왼쪽면
	vecIdx.push_back(4);
	vecIdx.push_back(1);
	vecIdx.push_back(2);



	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"FrustumMesh", pMesh);
	vecVtx.clear();
	vecIdx.clear();
	

}

void CResMgr::CreateDefaultTexture()
{
	// NoiseTexture
	Load<CTexture>(L"texture\\noise\\noise_03.jpg", L"texture\\noise\\noise_01.png");
	Load<CTexture>(L"texture\\noise\\noise_03.jpg", L"texture\\noise\\noise_02.png");
	Load<CTexture>(L"texture\\noise\\noise_03.jpg", L"texture\\noise\\noise_03.jpg");

	// 텍스쳐 로딩
	Load<CTexture>(L"texture\\smokeparticle.png", L"texture\\smokeparticle.png");
	Load<CTexture>(L"texture\\link.png", L"texture\\link.png");

	Load<CTexture>(L"texture\\particle\\smokeparticle.png", L"texture\\particle\\smokeparticle.png");
	Load<CTexture>(L"texture\\particle\\CartoonSmoke.png", L"texture\\particle\\CartoonSmoke.png");
	Load<CTexture>(L"texture\\particle\\Bubbles50px.png", L"texture\\particle\\Bubbles50px.png");

	Load<CTexture>(L"texture\\TILE.bmp", L"texture\\TILE.bmp");
	 
	


	// 텍스쳐 생성
	CreateTexture(L"UAVTex", 1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM
		, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);
	CreateTexture(L"UAV_HDRTex", CDevice::GetInst()->GetRenderResolution().x, CDevice::GetInst()->GetRenderResolution().y, DXGI_FORMAT_R8G8B8A8_UNORM
		, D3D11_BIND_SHADER_RESOURCE );
	CreateTexture(L"UAV_HDRBloomTex", CDevice::GetInst()->GetRenderResolution().x, CDevice::GetInst()->GetRenderResolution().y, DXGI_FORMAT_R8G8B8A8_UNORM
		, D3D11_BIND_SHADER_RESOURCE);
}

void CResMgr::CreateDefaultGraphicsShader()
{
	AddInputLayout(DXGI_FORMAT_R32G32B32_FLOAT, "POSITION", 0, 0);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "COLOR", 0, 0);
	AddInputLayout(DXGI_FORMAT_R32G32_FLOAT, "TEXCOORD", 0, 0);
	AddInputLayout(DXGI_FORMAT_R32G32B32_FLOAT, "TANGENT", 0, 0);
	AddInputLayout(DXGI_FORMAT_R32G32B32_FLOAT, "BINORMAL", 0, 0);
	AddInputLayout(DXGI_FORMAT_R32G32B32_FLOAT, "NORMAL", 0, 0);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "BLENDWEIGHT", 0, 0);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "BLENDINDICES", 0, 0);

	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WORLD", 1, 0);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WORLD", 1, 1);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WORLD", 1, 2);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WORLD", 1, 3);

	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WV", 1, 0);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WV", 1, 1);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WV", 1, 2);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WV", 1, 3);

	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WVP", 1, 0);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WVP", 1, 1);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WVP", 1, 2);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WVP", 1, 3);

	AddInputLayout(DXGI_FORMAT_R32_UINT, "ROWINDEX", 1, 0);

	CGraphicsShader* pShader = nullptr;

	// Std2D Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	//pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	

	pShader->AddScalarParam(INT_0, "Test Int");
	pShader->AddScalarParam(FLOAT_2, "Test Float");
	pShader->AddScalarParam(VEC2_3, "Test Vec2");
	pShader->AddScalarParam(VEC4_2, "Test Vec4");
	pShader->AddTexureParam(TEX_0, "Output Texture 1");
	pShader->AddTexureParam(TEX_1, "Output Texture 2");
	pShader->AddTexureParam(TEX_2, "Output Texture 3");

	AddRes<CGraphicsShader>(L"Std2DShader", pShader);
	

	// Player
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_PLAYER");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	//pShader->SetDSType(DS_TYPE::GREATER);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);
	//pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT)
	AddRes<CGraphicsShader>(L"Std2DPLAYERShader", pShader);

	// UI
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_UI");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	//pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);
	//pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT)
	AddRes<CGraphicsShader>(L"Std2DUIShader", pShader);

	// Std2dAlphaBlend Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_AlphaBlend");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POST_PROCESS);

	pShader->AddTexureParam(TEX_0, "Output Texture 1");
	pShader->AddTexureParam(TEX_1, "Output Texture 2");
	pShader->AddTexureParam(TEX_2, "Output Texture 3");

	AddRes<CGraphicsShader>(L"Std2DAlphaBlendShader", pShader);


	// Grid Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\editor.fx", "VS_Grid");
	pShader->CreatePixelShader(L"shader\\editor.fx", "PS_Grid");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"GridShader", pShader);

	// DebugDraw Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\debugdraw.fx", "VS_DebugDraw");
	pShader->CreatePixelShader(L"shader\\debugdraw.fx", "PS_DebugDraw");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"DebugDrawShader", pShader);


	// DeadCell Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\test.fx", "VS_DeadCell");
	pShader->CreatePixelShader(L"shader\\test.fx", "PS_DeadCell");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::LESS);	
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);	

	AddRes<CGraphicsShader>(L"DeadCellShader", pShader);


	// ParticleRenderShader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\particlerender.fx", "VS_ParticleRender");
	pShader->CreateGeometryShader(L"shader\\particlerender.fx", "GS_ParticleRender");
	pShader->CreatePixelShader(L"shader\\particlerender.fx", "PS_ParticleRender");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"ParticleRenderShader", pShader);


	// PostProcess Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_PostProcess");	
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_PostProcess");
	
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POST_PROCESS);

	//pShader->AddScalarParam(FLOAT_2, "Test Float");
	

	AddRes<CGraphicsShader>(L"PostProcessShader", pShader);

	// TileMap Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\tilemap.fx", "VS_TileMap");
	pShader->CreatePixelShader(L"shader\\tilemap.fx", "PS_TileMap");
	//pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_AlphaBlend");

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);
	//pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	//pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetRSType(RS_TYPE::CULL_NONE);

	AddRes<CGraphicsShader>(L"TileMapShader", pShader);

	//// TileMapAlpha Shader
	//pShader = new CGraphicsShader;
	//pShader->CreateVertexShader(L"shader\\tilemap.fx", "VS_TileMap");
	//pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_AlphaBlend");

	////pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);
	//pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	//pShader->SetBSType(BS_TYPE::ALPHABLEND);
	//pShader->SetRSType(RS_TYPE::CULL_NONE);

	//AddRes<CGraphicsShader>(L"TileMapAlphaShader", pShader);


	// Std3DShader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std3d.fx", "VS_Std3D");
	pShader->CreatePixelShader(L"shader\\std3d.fx", "PS_Std3D");

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

	pShader->AddScalarParam(FLOAT_0, "Specular Coefficient");
	pShader->AddTexureParam(TEX_0, "ObjectColor");
	pShader->AddTexureParam(TEX_1, "NormalMap");

	AddRes<CGraphicsShader>(L"Std3DShader", pShader);


	// Std3D_Deferred Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std3d_deferred.fx", "VS_Std3D_Deferred");
	pShader->CreatePixelShader(L"shader\\std3d_deferred.fx", "PS_Std3D_Deferred");

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED_OPAQUE);

	pShader->AddScalarParam(FLOAT_0, "Specular Coefficient");
	pShader->AddTexureParam(TEX_0, "ObjectColor");
	pShader->AddTexureParam(TEX_1, "NormalMap");

	AddRes<CGraphicsShader>(L"Std3DDeferredShader", pShader);

	// DirLightShader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\light.fx", "VS_DirLightShader");
	pShader->CreatePixelShader(L"shader\\light.fx", "PS_DirLightShader");

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	// 원래 색에 라이트값을 더해야되니깐 블랜드 타입 원원
	pShader->SetBSType(BS_TYPE::ONE_ONE);
	// 하지만 깊이 기록을 안남기기 때문에 노테노라
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

	// 제일 마지막에 그려져야되서 도메인 라이트
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHT);

	AddRes<CGraphicsShader>(L"DirLightShader", pShader);

	// PointLightShader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\light.fx", "VS_PointLightShader");
	pShader->CreatePixelShader(L"shader\\light.fx", "PS_PointLightShader");

	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetBSType(BS_TYPE::ONE_ONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHT);

	AddRes<CGraphicsShader>(L"PointLightShader", pShader);


	// Merge Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\merge.fx", "VS_Merge");
	pShader->CreatePixelShader(L"shader\\merge.fx", "PS_Merge");

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::NONE);

	pShader->AddScalarParam(INT_0, "Tonemapping");
	
	pShader->AddScalarParam(INT_1, "Bloom");
	pShader->AddScalarParam(FLOAT_0, "MiddleGrey");
	pShader->AddScalarParam(FLOAT_1, "LunWhiteSqr");


	AddRes<CGraphicsShader>(L"MergeShader", pShader);

	// Decal Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\decal.fx", "VS_Decal");
	pShader->CreatePixelShader(L"shader\\decal.fx", "PS_Decal");

	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DECAL);

	AddRes<CGraphicsShader>(L"DecalShader", pShader);

	// SkyBox Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\skybox.fx", "VS_SkyBox");
	pShader->CreatePixelShader(L"shader\\skybox.fx", "PS_SkyBox");

	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::LESS_EQUAL);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

	pShader->AddScalarParam(INT_1, "tonemapping");

	AddRes<CGraphicsShader>(L"SkyBoxShader", pShader);

	// frustum
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\frustum.fx", "VS_Frustum");
	pShader->CreatePixelShader(L"shader\\frustum.fx", "PS_Frustum");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"frustumShader", pShader);

	// TessShader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\tess.fx", "VS_Tess");
	pShader->CreateHullShader(L"shader\\tess.fx", "HS_Tess");
	pShader->CreateDomainShader(L"shader\\tess.fx", "DS_Tess");
	pShader->CreatePixelShader(L"shader\\tess.fx", "PS_Tess");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);


	pShader->AddScalarParam(INT_0, "Tess Factor");
	pShader->AddScalarParam(INT_1, "Tess Factor");
	pShader->AddScalarParam(INT_2, "Tess Factor");
	pShader->AddScalarParam(INT_3, "Tess Factor");

	AddRes<CGraphicsShader>(L"TessShader", pShader);


	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\bloomPS.fx", "VS_BLOOM");
	pShader->CreatePixelShader(L"shader\\bloomPS.fx", "PS_BLOOM");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POST_PROCESS);

	//pShader->AddScalarParam(FLOAT_2, "Test Float");


	AddRes<CGraphicsShader>(L"Bloomshader", pShader);

	// DepthmapShader
	pShader = new CGraphicsShader;
	pShader->SetDomain(SHADER_DOMAIN::NONE);
	pShader->CreateVertexShader(L"Shader\\light.fx", "VS_DepthMap");
	pShader->CreatePixelShader(L"Shader\\light.fx", "PS_DepthMap");

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	AddRes<CGraphicsShader>(L"DepthMapShader", pShader);
}


#include "CPaintShader.h"
#include "CParticleUpdateShader.h"
#include "CHDR.h"
#include "CBloom.h"
#include "CAnimation3DShader.h"
#include "CCopyBoneShader.h"
void CResMgr::CreateDefaultComputeShader()
{
	CComputeShader* pShader = nullptr;

	// Paint Shader
	pShader = new CPaintShader;
	pShader->CreateComputeShader(L"shader\\compute.fx", "CS_Paint");
	AddRes<CComputeShader>(L"PaintShader", pShader);

	// Particle Update Shader
	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(L"shader\\particleupdate.fx", "CS_ParticleUpdate");
	AddRes<CComputeShader>(L"ParticleUpdateShader", pShader);

	// Razer Update Shader
	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(L"shader\\RazerParticle.fx", "CS_RazerParticle");
	AddRes<CComputeShader>(L"RazerParticleShader", pShader);

	// Blood Update Shader
	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(L"shader\\BloodParticle.fx", "CS_BloodParticle");
	AddRes<CComputeShader>(L"BloodParticleShader", pShader);

	// Razer_spark Update Shader
	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(L"shader\\Razer_spark.fx", "CS_RazerSpark");
	AddRes<CComputeShader>(L"Razer_sparkShader", pShader);

	// Rain Update Shader
	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(L"shader\\Rain.fx", "CS__RAIN");
	AddRes<CComputeShader>(L"Rainshadershader", pShader);
	
	// 첫번째 휘도 계산
	pShader = new CHDR;
	pShader->CreateComputeShader(L"shader\\HDR.fx", "DownScaleFirstPass");
	AddRes<CComputeShader>(L"HDRFirstPassshader", pShader);

	// 두번째 휘도 계산
	pShader = new CHDR;
	pShader->CreateComputeShader(L"shader\\HDR.fx", "DownScaleSecondPass");
	AddRes<CComputeShader>(L"HDRSecondPassshader", pShader);
	
	// 블롬 계산
	pShader = new CBloom;
	pShader->CreateComputeShader(L"shader\\Bloom.fx", "BloomReveal");
	AddRes<CComputeShader>(L"HDRBloom", pShader);
	

	// Animation3D Update Shader	
	pShader = new CAnimation3DShader;
	pShader->CreateComputeShader(L"shader\\animation3d.fx", "CS_Animation3D");
	AddRes<CComputeShader>(L"Animation3DUpdateShader", pShader);

	// CopyBone Shader	
	pShader = new CCopyBoneShader;
	pShader->CreateComputeShader(L"shader\\copybone.fx", "CS_CopyBoneMatrix");
	AddRes<CComputeShader>(L"CopyBoneShader", pShader);

	
}

void CResMgr::CreateDefaultMaterial()
{
	CMaterial* pMtrl = nullptr;

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"Std2DMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DAlphaBlendShader"));
	AddRes<CMaterial>(L"Std2DAlphaBlendMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"GridShader"));
	AddRes<CMaterial>(L"GridMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DebugDrawShader"));
	AddRes<CMaterial>(L"DebugDrawMtrl", pMtrl);	

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DeadCellShader"));
	AddRes<CMaterial>(L"DeadCellMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"ParticleRenderShader"));
	pMtrl->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\particle\\Sparks.png"));
	AddRes<CMaterial>(L"ParticleRenderMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"PostProcessShader"));
	AddRes<CMaterial>(L"PostProcessMtrl", pMtrl);
	

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"TileMapShader"));
	AddRes<CMaterial>(L"TileMapMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std3DShader"));
	AddRes<CMaterial>(L"Std3DMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std3DDeferredShader"));
	AddRes<CMaterial>(L"Std3DDeferredMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"MergeShader"));

	//pShader->AddScalarParam(INT_0, "Tonemapping");

	//pShader->AddScalarParam(INT_1, "Bloom");
	//pShader->AddScalarParam(FLOAT_0, "MiddleGrey");
	//pShader->AddScalarParam(FLOAT_1, "LunWhiteSqr");
	int a = 0;
	pMtrl->SetScalarParam(INT_0, &a);
	a = 1;
	pMtrl->SetScalarParam(INT_1, &a);
	float b = 0.8;
	pMtrl->SetScalarParam(FLOAT_0, &b);
	b = 0.1;
	pMtrl->SetScalarParam(FLOAT_1, &b);
	AddRes<CMaterial>(L"MergeMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DirLightShader"));
	AddRes<CMaterial>(L"DirLightMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"PointLightShader"));
	AddRes<CMaterial>(L"PointLightMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DecalShader"));
	AddRes<CMaterial>(L"DecalMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"SkyBoxShader"));
	AddRes<CMaterial>(L"SkyBoxMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"frustumShader"));
	AddRes<CMaterial>(L"frustumMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"TessShader"));
	AddRes<CMaterial>(L"TessMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"HDRFirstPassshader"));
	AddRes<CMaterial>(L"HDRFirstPassMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Bloomshader"));
	AddRes<CMaterial>(L"BloomMtrl", pMtrl);

	// DepthMap Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DepthMapShader"));
	AddRes<CMaterial>(L"DepthMapMtrl", pMtrl);
	
}

void CResMgr::CreateconeMesh(float radius, float height, size_t tessellation)
{
	if (tessellation < 3)
	{
		throw std::invalid_argument("tessellation parameter must be at least 3");
	}

	vector<Vtx>  vertices;
	vector<UINT> indices;

	vertices.reserve(tessellation + 1); // 1 + x
	indices.reserve(tessellation * 6 - 6);  // 3x + 3(x-2)

	const Vec3   topPosition = Vec3::Zero;
	vertices.push_back(Vtx{ topPosition, {}, {}, {}, {}, {} });

	// 동그라미 부분 인덱스 잡기
	for (size_t i = 0; i < tessellation; i++)
	{
		// 코사인 사인 값 알려주는 함수
		const Vec3 circleVec = GetCircleVector(i, tessellation);
		// 코사인 사인 값에 반지름 곱하고 높이 더함
		const Vec3 position = circleVec * radius + Vec3(0, 0, height);
		// 버텍스 정보에 포지션 값만 넣고 나머지 널포인트
		vertices.push_back(Vtx{ position, {}, {}, {}, {}, {} });

		// 인덱스에 정보 넣기
		indices.push_back(0);
		indices.push_back(i + 2);
		indices.push_back(i + 1);
	}

	indices[indices.size() - 2] = 1;

	// 원점에서 인덱스 동그라미 부분이랑 이어주기
	for (size_t i = 0; i < tessellation - 2; i++)
	{
		indices.push_back(1);
		indices.push_back(1 + i + 1);
		indices.push_back(1 + i + 2);
	}


	// 추가
	CMesh* pMesh = new CMesh(true);
	pMesh->Create(vertices.data(), vertices.size(), indices.data(), indices.size());
	AddRes<CMesh>(L"ConeMesh", pMesh);
}


void CResMgr::AddInputLayout(DXGI_FORMAT _eFormat, const char* _strSemanticName, UINT _iSlotNum, UINT _iSemanticIdx)
{
	D3D11_INPUT_ELEMENT_DESC LayoutDesc = {};

	if (0 == _iSlotNum)
	{
		LayoutDesc.AlignedByteOffset = m_iLayoutOffset_0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.InstanceDataStepRate = 0;
	}
	else if (1 == _iSlotNum)
	{
		LayoutDesc.AlignedByteOffset = m_iLayoutOffset_1;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		LayoutDesc.InstanceDataStepRate = 1;
	}

	LayoutDesc.Format = _eFormat;
	LayoutDesc.InputSlot = _iSlotNum;
	LayoutDesc.SemanticName = _strSemanticName;
	LayoutDesc.SemanticIndex = _iSemanticIdx;

	m_vecLayoutInfo.push_back(LayoutDesc);


	// Offset 증가
	if (0 == _iSlotNum)
		m_iLayoutOffset_0 += GetSizeofFormat(_eFormat);
	else if (1 == _iSlotNum)
		m_iLayoutOffset_1 += GetSizeofFormat(_eFormat);
}

bool CResMgr::DeleteRes(RES_TYPE _Type, const wstring& _strKey)
{
	map<wstring, Ptr<CRes>>::iterator iter = m_arrRes[(UINT)_Type].find(_strKey);

	if (m_arrRes[(UINT)_Type].end() != iter)
	{
		m_arrRes[(UINT)_Type].erase(iter);
		m_bChanged = true;
		return true;
	}

	return false;
}
