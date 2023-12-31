#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"
#include "CFBXLoader.h"
#include "CInstancingBuffer.h"

CMesh::CMesh(bool _bEngineRes)
	: CRes(RES_TYPE::MESH, _bEngineRes)
	, m_tVBDesc{}
	, m_iVtxCount(0)
	, m_pVtxSys(nullptr)
{
}

CMesh::CMesh(const CMesh& _origin)
	: CRes(RES_TYPE::MESH, _origin.IsEngineRes())
{
}

CMesh::~CMesh()
{
	SAFE_DELETE(m_pVtxSys);

	for (size_t i = 0; i < m_vecIdxInfo.size(); ++i)
	{
		SAFE_DELETE(m_vecIdxInfo[i].pIdxSysMem);
	}

	// Animation3D 정보
	SAFE_DELETE(m_pBoneFrameData);
	SAFE_DELETE(m_pBoneOffset);
}

CMesh* CMesh::CreateFromContainer(CFBXLoader& _loader, int _index)
{
	const tContainer* container = &_loader.GetContainer(_index);

	UINT iVtxCount = (UINT)container->vecPos.size();

	D3D11_BUFFER_DESC tVtxDesc = {};

	tVtxDesc.ByteWidth = sizeof(Vtx) * iVtxCount;
	tVtxDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	tVtxDesc.Usage = D3D11_USAGE_DEFAULT;
	if (D3D11_USAGE_DYNAMIC == tVtxDesc.Usage)
		tVtxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = malloc(tVtxDesc.ByteWidth);
	Vtx* pSys = (Vtx*)tSub.pSysMem;
	for (UINT i = 0; i < iVtxCount; ++i)
	{
		pSys[i].vPos = container->vecPos[i];
		pSys[i].vUV = container->vecUV[i];
		pSys[i].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
		pSys[i].vNormal = container->vecNormal[i];
		pSys[i].vTangent = container->vecTangent[i];
		pSys[i].vBinormal = container->vecBinormal[i];
		pSys[i].vWeights = container->vecWeights[i];
		pSys[i].vIndices = container->vecIndices[i];
	}

	ComPtr<ID3D11Buffer> pVB = NULL;
	if (FAILED(DEVICE->CreateBuffer(&tVtxDesc, &tSub, pVB.GetAddressOf())))
	{
		return NULL;
	}

	CMesh* pMesh = new CMesh;
	pMesh->m_VB = pVB;
	pMesh->m_tVBDesc = tVtxDesc;
	pMesh->m_pVtxSys = pSys;

	// 인덱스 정보
	UINT iIdxBufferCount = (UINT)container->vecIdx.size();
	//if(_loader.GetContainerCount() > 1)
	//{
	//	iIdxBufferCount = _loader.GetContainerCount();
	//	D3D11_BUFFER_DESC tIdxDesc = {};

	//	for (UINT i = 0; i < iIdxBufferCount; ++i)
	//	{
	//		tIdxDesc.ByteWidth = (UINT)_loader.GetContainer(i).vecIdx[0].size() * sizeof(UINT); // Index Format 이 R32_UINT 이기 때문
	//		tIdxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//		tIdxDesc.Usage = D3D11_USAGE_DEFAULT;
	//		if (D3D11_USAGE_DYNAMIC == tIdxDesc.Usage)
	//			tIdxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//		void* pSysMem = malloc(tIdxDesc.ByteWidth);
	//		memcpy(pSysMem, _loader.GetContainer(i).vecIdx[0].data(), tIdxDesc.ByteWidth);
	//		tSub.pSysMem = pSysMem;

	//		ComPtr<ID3D11Buffer> pIB = nullptr;
	//		if (FAILED(DEVICE->CreateBuffer(&tIdxDesc, &tSub, pIB.GetAddressOf())))
	//		{
	//			return NULL;
	//		}

	//		tIndexInfo info = {};
	//		info.tIBDesc = tIdxDesc;
	//		info.iIdxCount = (UINT)_loader.GetContainer(i).vecIdx[0].size();
	//		info.pIdxSysMem = pSysMem;
	//		info.pIB = pIB;

	//		pMesh->m_vecIdxInfo.push_back(info);
	//	}
	//}
	
		D3D11_BUFFER_DESC tIdxDesc = {};

		for (UINT i = 0; i < iIdxBufferCount; ++i)
		{
			tIdxDesc.ByteWidth = (UINT)container->vecIdx[i].size() * sizeof(UINT); // Index Format 이 R32_UINT 이기 때문
			tIdxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			tIdxDesc.Usage = D3D11_USAGE_DEFAULT;
			if (D3D11_USAGE_DYNAMIC == tIdxDesc.Usage)
				tIdxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			void* pSysMem = malloc(tIdxDesc.ByteWidth);
			memcpy(pSysMem, container->vecIdx[i].data(), tIdxDesc.ByteWidth);
			tSub.pSysMem = pSysMem;

			ComPtr<ID3D11Buffer> pIB = nullptr;
			if (FAILED(DEVICE->CreateBuffer(&tIdxDesc, &tSub, pIB.GetAddressOf())))
			{
				return NULL;
			}

			tIndexInfo info = {};
			info.tIBDesc = tIdxDesc;
			info.iIdxCount = (UINT)container->vecIdx[i].size();
			info.pIdxSysMem = pSysMem;
			info.pIB = pIB;

			pMesh->m_vecIdxInfo.push_back(info);
		}
	
	
	// Animation3D
	if (!container->bAnimation)
		return pMesh;

	vector<tBone*>& vecBone = _loader.GetBones();
	UINT iFrameCount = 0;
	for (UINT i = 0; i < vecBone.size(); ++i)
	{
		tMTBone bone = {};
		bone.iDepth = vecBone[i]->iDepth;
		bone.iParentIndx = vecBone[i]->iParentIndx;
		bone.matBone = GetMatrixFromFbxMatrix(vecBone[i]->matBone);
		bone.matOffset = GetMatrixFromFbxMatrix(vecBone[i]->matOffset);
		bone.strBoneName = vecBone[i]->strBoneName;

		for (UINT j = 0; j < vecBone[i]->vecKeyFrame.size(); ++j)
		{
			tMTKeyFrame tKeyframe = {};
			tKeyframe.dTime = vecBone[i]->vecKeyFrame[j].dTime;
			tKeyframe.iFrame = j;
			tKeyframe.vTranslate.x = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetT().mData[0];
			tKeyframe.vTranslate.y = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetT().mData[1];
			tKeyframe.vTranslate.z = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetT().mData[2];

			tKeyframe.vScale.x = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetS().mData[0];
			tKeyframe.vScale.y = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetS().mData[1];
			tKeyframe.vScale.z = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetS().mData[2];

			tKeyframe.qRot.x = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[0];
			tKeyframe.qRot.y = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[1];
			tKeyframe.qRot.z = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[2];
			tKeyframe.qRot.w = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[3];

			bone.vecKeyFrame.push_back(tKeyframe);
		}

		iFrameCount = max(iFrameCount, (UINT)bone.vecKeyFrame.size());

		pMesh->m_vecBones.push_back(bone);
	}

	vector<tAnimClip*>& vecAnimClip = _loader.GetAnimClip();

	for (UINT i = 0; i < vecAnimClip.size(); ++i)
	{
		tMTAnimClip tClip = {};

		tClip.strAnimName = vecAnimClip[i]->strName;
		tClip.dStartTime = vecAnimClip[i]->tStartTime.GetSecondDouble();
		tClip.dEndTime = vecAnimClip[i]->tEndTime.GetSecondDouble();
		tClip.dTimeLength = tClip.dEndTime - tClip.dStartTime;

		tClip.iStartFrame = (int)vecAnimClip[i]->tStartTime.GetFrameCount(vecAnimClip[i]->eMode);
		tClip.iEndFrame = (int)vecAnimClip[i]->tEndTime.GetFrameCount(vecAnimClip[i]->eMode);
		tClip.iFrameLength = tClip.iEndFrame - tClip.iStartFrame;
		tClip.eMode = vecAnimClip[i]->eMode;

		pMesh->m_vecAnimClip.push_back(tClip);
	}

	// Animation 이 있는 Mesh 경우 structuredbuffer 만들어두기
	if (pMesh->IsAnimMesh())
	{
		// BoneOffet 행렬
		vector<Matrix> vecOffset;
		vector<tFrameTrans> vecFrameTrans;
		vecFrameTrans.resize((UINT)pMesh->m_vecBones.size() * iFrameCount);

		for (size_t i = 0; i < pMesh->m_vecBones.size(); ++i)
		{
			vecOffset.push_back(pMesh->m_vecBones[i].matOffset);

			for (size_t j = 0; j < pMesh->m_vecBones[i].vecKeyFrame.size(); ++j)
			{
				vecFrameTrans[(UINT)pMesh->m_vecBones.size() * j + i]
					= tFrameTrans{ Vec4(pMesh->m_vecBones[i].vecKeyFrame[j].vTranslate, 0.f)
					, Vec4(pMesh->m_vecBones[i].vecKeyFrame[j].vScale, 0.f)
					, pMesh->m_vecBones[i].vecKeyFrame[j].qRot };
			}
		}

		pMesh->m_pBoneOffset = new CStructuredBuffer;
		pMesh->m_pBoneOffset->Create(sizeof(Matrix), (UINT)vecOffset.size(), SB_TYPE::SRV_ONLY, vecOffset.data(), false);

		pMesh->m_pBoneFrameData = new CStructuredBuffer;
		pMesh->m_pBoneFrameData->Create(sizeof(tFrameTrans), (UINT)vecOffset.size() * iFrameCount
			, SB_TYPE::SRV_ONLY, vecFrameTrans.data(), false);
	}

	return pMesh;
}

int CMesh::Create(void* _pVtxSys, size_t _iVtxCount, void* _pIdxSys, size_t _iIdxCount)
{
	m_iVtxCount = _iVtxCount;

	tIndexInfo IndexInfo = {};
	IndexInfo.iIdxCount = _iIdxCount;

	// 정점 데이터를 저장할 버텍스 버퍼를 생성한다.	
	m_tVBDesc.ByteWidth = sizeof(Vtx) * _iVtxCount;

	// 정점 버퍼는 처음 생성이후에 버퍼를 수정하지 않는다.
	m_tVBDesc.CPUAccessFlags = 0;
	m_tVBDesc.Usage = D3D11_USAGE_DEFAULT;

	// 정점을 저장하는 목적의 버퍼 임을 알림
	m_tVBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	m_tVBDesc.MiscFlags = 0;
	m_tVBDesc.StructureByteStride = 0;

	// 초기 데이터를 넘겨주기 위한 정보 구조체
	D3D11_SUBRESOURCE_DATA tSubDesc = {};
	tSubDesc.pSysMem = _pVtxSys;

	if (FAILED(DEVICE->CreateBuffer(&m_tVBDesc, &tSubDesc, m_VB.GetAddressOf())))
	{
		return E_FAIL;
	}


	IndexInfo.tIBDesc.ByteWidth = sizeof(UINT) * _iIdxCount;

	// 버퍼 생성 이후에도, 버퍼의 내용을 수정 할 수 있는 옵션
	IndexInfo.tIBDesc.CPUAccessFlags = 0;
	IndexInfo.tIBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

	// 정점을 저장하는 목적의 버퍼 임을 알림
	IndexInfo.tIBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	IndexInfo.tIBDesc.MiscFlags = 0;
	IndexInfo.tIBDesc.StructureByteStride = 0;

	// 초기 데이터를 넘겨주기 위한 정보 구조체
	tSubDesc = {};
	tSubDesc.pSysMem = _pIdxSys;

	if (FAILED(DEVICE->CreateBuffer(&IndexInfo.tIBDesc, &tSubDesc, IndexInfo.pIB.GetAddressOf())))
	{
		return E_FAIL;
	}

	// 시스템 메모리 할당
	m_pVtxSys = new Vtx[m_iVtxCount];
	memcpy(m_pVtxSys, _pVtxSys, sizeof(Vtx) * m_iVtxCount);

	IndexInfo.pIdxSysMem = new UINT[IndexInfo.iIdxCount];
	memcpy(IndexInfo.pIdxSysMem, _pIdxSys, sizeof(UINT) * IndexInfo.iIdxCount);

	m_vecIdxInfo.push_back(IndexInfo);

	return S_OK;
}

int CMesh::Create(void* _pVtxSys, size_t _iVtxCount)
{
	m_VB->Release();
	m_iVtxCount = _iVtxCount;

	// 정점 데이터를 저장할 버텍스 버퍼를 생성한다.	
	m_tVBDesc.ByteWidth = sizeof(Vtx) * _iVtxCount;

	// 정점 버퍼는 처음 생성이후에 버퍼를 수정하지 않는다.
	m_tVBDesc.CPUAccessFlags = 0;
	m_tVBDesc.Usage = D3D11_USAGE_DEFAULT;

	// 정점을 저장하는 목적의 버퍼 임을 알림
	m_tVBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	m_tVBDesc.MiscFlags = 0;
	m_tVBDesc.StructureByteStride = 0;
	
	// 초기 데이터를 넘겨주기 위한 정보 구조체
	D3D11_SUBRESOURCE_DATA tSubDesc = {};
	tSubDesc.pSysMem = _pVtxSys;

	if (FAILED(DEVICE->CreateBuffer(&m_tVBDesc, &tSubDesc, m_VB.GetAddressOf())))
	{
		return E_FAIL;
	}
	delete m_pVtxSys;

	// 시스템 메모리 할당
	m_pVtxSys = new Vtx[m_iVtxCount];
	memcpy(m_pVtxSys, _pVtxSys, sizeof(Vtx) * m_iVtxCount);
	return 0;
}

void CMesh::UpdateData(UINT _iSubset)
{
	UINT iStride = sizeof(Vtx);
	UINT iOffset = 0;

	CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->IASetIndexBuffer(m_vecIdxInfo[_iSubset].pIB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::UpdateData_Inst(UINT _iSubset)
{
	if (_iSubset >= m_vecIdxInfo.size())
		assert(nullptr);

	ID3D11Buffer* arrBuffer[2] = { m_VB.Get(), CInstancingBuffer::GetInst()->GetBuffer().Get() };
	UINT		  iStride[2] = { sizeof(Vtx), sizeof(tInstancingData) };
	UINT		  iOffset[2] = { 0, 0 };

	CONTEXT->IASetVertexBuffers(0, 2, arrBuffer, iStride, iOffset);
	CONTEXT->IASetIndexBuffer(m_vecIdxInfo[_iSubset].pIB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::render(UINT _iSubset)
{
	UpdateData(_iSubset);
	CONTEXT->DrawIndexed(m_vecIdxInfo[_iSubset].iIdxCount, 0, 0);
}

void CMesh::render_instancing(UINT _iSubset)
{
	UpdateData_Inst(_iSubset);

	CONTEXT->DrawIndexedInstanced(m_vecIdxInfo[_iSubset].iIdxCount
		, CInstancingBuffer::GetInst()->GetInstanceCount(), 0, 0, 0);
}


void CMesh::render_particle(UINT _iCount)
{
	UpdateData(0);
	CONTEXT->DrawIndexedInstanced(m_vecIdxInfo[0].iIdxCount, _iCount, 0, 0, 0);
}

void CMesh::Save(const wstring& _strRelativePath)
{
	// 상대경로 저장
	SetRelativePath(_strRelativePath);

	// 파일 경로 만들기
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + _strRelativePath;

	// 파일 쓰기모드로 열기
	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);

	// 키값, 상대 경로	
	SaveWStringToFile(GetName(), pFile);
	SaveWStringToFile(GetKey(), pFile);
	SaveWStringToFile(GetRelativePath(), pFile);

	// 정점 데이터 저장				
	int iByteSize = m_tVBDesc.ByteWidth;
	fwrite(&iByteSize, sizeof(int), 1, pFile);
	fwrite(m_pVtxSys, iByteSize, 1, pFile);

	// 인덱스 정보
	UINT iMtrlCount = (UINT)m_vecIdxInfo.size();
	fwrite(&iMtrlCount, sizeof(int), 1, pFile);

	UINT iIdxBuffSize = 0;
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		fwrite(&m_vecIdxInfo[i], sizeof(tIndexInfo), 1, pFile);
		fwrite(m_vecIdxInfo[i].pIdxSysMem
			, m_vecIdxInfo[i].iIdxCount * sizeof(UINT)
			, 1, pFile);
	}

	// Animation3D 정보 
	UINT iCount = (UINT)m_vecAnimClip.size();
	fwrite(&iCount, sizeof(int), 1, pFile);
	for (UINT i = 0; i < iCount; ++i)
	{
		SaveWStringToFile(m_vecAnimClip[i].strAnimName, pFile);
		fwrite(&m_vecAnimClip[i].dStartTime, sizeof(double), 1, pFile);
		fwrite(&m_vecAnimClip[i].dEndTime, sizeof(double), 1, pFile);
		fwrite(&m_vecAnimClip[i].dTimeLength, sizeof(double), 1, pFile);
		fwrite(&m_vecAnimClip[i].eMode, sizeof(int), 1, pFile);
		fwrite(&m_vecAnimClip[i].fUpdateTime, sizeof(float), 1, pFile);
		fwrite(&m_vecAnimClip[i].iStartFrame, sizeof(int), 1, pFile);
		fwrite(&m_vecAnimClip[i].iEndFrame, sizeof(int), 1, pFile);
		fwrite(&m_vecAnimClip[i].iFrameLength, sizeof(int), 1, pFile);
	}

	iCount = (UINT)m_vecBones.size();
	fwrite(&iCount, sizeof(int), 1, pFile);

	for (UINT i = 0; i < iCount; ++i)
	{
		SaveWStringToFile(m_vecBones[i].strBoneName, pFile);
		fwrite(&m_vecBones[i].iDepth, sizeof(int), 1, pFile);
		fwrite(&m_vecBones[i].iParentIndx, sizeof(int), 1, pFile);
		fwrite(&m_vecBones[i].matBone, sizeof(Matrix), 1, pFile);
		fwrite(&m_vecBones[i].matOffset, sizeof(Matrix), 1, pFile);

		int iFrameCount = (int)m_vecBones[i].vecKeyFrame.size();
		fwrite(&iFrameCount, sizeof(int), 1, pFile);

		for (int j = 0; j < m_vecBones[i].vecKeyFrame.size(); ++j)
		{
			fwrite(&m_vecBones[i].vecKeyFrame[j], sizeof(tMTKeyFrame), 1, pFile);
		}
	}

	fclose(pFile);
}

int CMesh::Load(const wstring& _strFilePath)
{
	// 읽기모드로 파일열기
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	// 키값, 상대경로
	wstring strName, strKey, strRelativePath;
	LoadWStringFromFile(strName, pFile);
	LoadWStringFromFile(strKey, pFile);
	LoadWStringFromFile(strRelativePath, pFile);

	SetName(strName);
	SetKey(strKey);
	SetRelativePath(strRelativePath);

	// 정점데이터
	UINT iByteSize = 0;
	fread(&iByteSize, sizeof(int), 1, pFile);

	m_pVtxSys = (Vtx*)malloc(iByteSize);
	fread(m_pVtxSys, 1, iByteSize, pFile);

	wstring a = GetKey();
	if (GetKey() == L"mesh\\GreateSword.mesh")
	{
		vector<Vtx> vtx;
		Vtx* a = (Vtx*)m_pVtxSys;
		size_t b = iByteSize / sizeof(Vtx);
		for (int i = 0; i < b; ++i)
		{
			a[i].vPos = Vec3(-a[i].vPos.x, -a[i].vPos.y, -a[i].vPos.z);
		}
		m_pVtxSys = a;
	}


	m_iVtxCount = iByteSize / sizeof(Vtx);

	D3D11_BUFFER_DESC tDesc = {};
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.ByteWidth = iByteSize;
	tDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem = m_pVtxSys;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tSubData, m_VB.GetAddressOf())))
	{
		assert(nullptr);
	}

	// 인덱스 정보
	UINT iMtrlCount = 0;
	fread(&iMtrlCount, sizeof(int), 1, pFile);

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		tIndexInfo info = {};
		fread(&info, sizeof(tIndexInfo), 1, pFile);

		UINT iByteWidth = info.iIdxCount * sizeof(UINT);

		void* pSysMem = malloc(iByteWidth);
		info.pIdxSysMem = pSysMem;
		fread(info.pIdxSysMem, iByteWidth, 1, pFile);

		tSubData.pSysMem = info.pIdxSysMem;

		if (FAILED(DEVICE->CreateBuffer(&info.tIBDesc, &tSubData, info.pIB.GetAddressOf())))
		{
			assert(nullptr);
		}

		m_vecIdxInfo.push_back(info);
	}

	// Animation3D 정보 읽기
	int iCount = 0;
	fread(&iCount, sizeof(int), 1, pFile);
	for (int i = 0; i < iCount; ++i)
	{
		tMTAnimClip tClip = {};

		LoadWStringFromFile(tClip.strAnimName, pFile);
		fread(&tClip.dStartTime, sizeof(double), 1, pFile);
		fread(&tClip.dEndTime, sizeof(double), 1, pFile);
		fread(&tClip.dTimeLength, sizeof(double), 1, pFile);
		fread(&tClip.eMode, sizeof(int), 1, pFile);
		fread(&tClip.fUpdateTime, sizeof(float), 1, pFile);
		fread(&tClip.iStartFrame, sizeof(int), 1, pFile);
		fread(&tClip.iEndFrame, sizeof(int), 1, pFile);
		fread(&tClip.iFrameLength, sizeof(int), 1, pFile);

		m_vecAnimClip.push_back(tClip);
	}

	iCount = 0;
	fread(&iCount, sizeof(int), 1, pFile);
	m_vecBones.resize(iCount);

	UINT _iFrameCount = 0;
	for (int i = 0; i < iCount; ++i)
	{
		LoadWStringFromFile(m_vecBones[i].strBoneName, pFile);
		fread(&m_vecBones[i].iDepth, sizeof(int), 1, pFile);
		fread(&m_vecBones[i].iParentIndx, sizeof(int), 1, pFile);
		fread(&m_vecBones[i].matBone, sizeof(Matrix), 1, pFile);
		fread(&m_vecBones[i].matOffset, sizeof(Matrix), 1, pFile);

		UINT iFrameCount = 0;
		fread(&iFrameCount, sizeof(int), 1, pFile);
		m_vecBones[i].vecKeyFrame.resize(iFrameCount);
		_iFrameCount = max(_iFrameCount, iFrameCount);
		for (UINT j = 0; j < iFrameCount; ++j)
		{
			fread(&m_vecBones[i].vecKeyFrame[j], sizeof(tMTKeyFrame), 1, pFile);
		}
	}

	// Animation 이 있는 Mesh 경우 Bone StructuredBuffer 만들기
	if (m_vecAnimClip.size() > 0 && m_vecBones.size() > 0)
	{
		wstring strBone = GetName();

		// BoneOffet 행렬
		vector<Matrix> vecOffset;
		vector<tFrameTrans> vecFrameTrans;
		vecFrameTrans.resize((UINT)m_vecBones.size() * _iFrameCount);

		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			vecOffset.push_back(m_vecBones[i].matOffset);

			for (size_t j = 0; j < m_vecBones[i].vecKeyFrame.size(); ++j)
			{
				vecFrameTrans[(UINT)m_vecBones.size() * j + i]
					= tFrameTrans{ Vec4(m_vecBones[i].vecKeyFrame[j].vTranslate, 0.f)
					, Vec4(m_vecBones[i].vecKeyFrame[j].vScale, 0.f)
					, Vec4(m_vecBones[i].vecKeyFrame[j].qRot) };
			}
		}

		m_pBoneOffset = new CStructuredBuffer;
		m_pBoneOffset->Create(sizeof(Matrix), (UINT)vecOffset.size(), SB_TYPE::SRV_ONLY, vecOffset.data(), false);

		m_pBoneFrameData = new CStructuredBuffer;
		m_pBoneFrameData->Create(sizeof(tFrameTrans), (UINT)vecOffset.size() * (UINT)_iFrameCount
			, SB_TYPE::SRV_ONLY, vecFrameTrans.data(), false);
	}



	fclose(pFile);

	return S_OK;
}

vector<Vtx> CMesh::GetVtxData()
{
	// 스태이징을 활용해 cpu로 읽어드리겠다는 버퍼 생성
	// 자주 활용 안 하는 함수라 생성하고 바로 제거
	D3D11_BUFFER_DESC stagingBufferDesc;
	stagingBufferDesc.ByteWidth = sizeof(Vtx) * m_iVtxCount;
	stagingBufferDesc.Usage = D3D11_USAGE_STAGING;
	stagingBufferDesc.BindFlags = 0;
	stagingBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	stagingBufferDesc.MiscFlags = 0;

	ID3D11Buffer* stagingBuffer = nullptr;
	vector<Vtx> arr;
	// 버퍼 생성
	if (FAILED(DEVICE->CreateBuffer(&stagingBufferDesc, nullptr, &stagingBuffer)))
	{
		assert(nullptr);
	}
	// gpu영역에서 정점 가져오기
	// 주로 테셀레이션으로 늘어난 정점을 가져올 때 쓸거
	CONTEXT->CopyResource(stagingBuffer, m_VB.Get());
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	// 데이터 카피
	CONTEXT->Map(stagingBuffer, 0, D3D11_MAP_READ, 0, &mappedResource);
	D3D11_BUFFER_DESC BufferDesc = {};
	stagingBuffer->GetDesc(&BufferDesc);

	UINT a = BufferDesc.ByteWidth / sizeof(Vtx);
	Vtx* data = static_cast<Vtx*>(mappedResource.pData);
	

	if (m_iVtxCount != 0)
	{	// 늘어난 정점의 사이즈를 구한다.
		for (int i = 0; i < m_iVtxCount; ++i)
		{
			arr.push_back(data[i]);
		}

	}
	else
	{
		// 늘어난 정점의 사이즈를 구한다.
		int size = (sizeof(Vtx) / sizeof(data));

		for (int i = 0; i < size; ++i)
		{
			arr.push_back(data[i]);
		}
	}
	int size = (sizeof(Vtx) / sizeof(data));

	CONTEXT->Unmap(stagingBuffer, 0);
	stagingBuffer->Release();
	return arr;
}

vector<UINT> CMesh::GetIndexData()
{
	UINT* a = (UINT*)(m_vecIdxInfo[0].pIdxSysMem);
	UINT size1 = (sizeof(UINT) / sizeof(a));

	// 스태이징을 활용해 cpu로 읽어드리겠다는 버퍼 생성
	// 자주 활용 안 하는 함수라 생성하고 바로 제거
	// landscape때만 쓸거라 인덱스 생성을 한번만 함
	D3D11_BUFFER_DESC stagingBufferDesc;
	stagingBufferDesc.ByteWidth = sizeof(UINT) * m_vecIdxInfo[0].iIdxCount;
	stagingBufferDesc.Usage = D3D11_USAGE_STAGING;
	stagingBufferDesc.BindFlags = 0;
	stagingBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	stagingBufferDesc.MiscFlags = 0;

	ID3D11Buffer* stagingBuffer = nullptr;
	vector<UINT> arr;
	// 버퍼 생성
	if (FAILED(DEVICE->CreateBuffer(&stagingBufferDesc, nullptr, &stagingBuffer)))
	{
		assert(nullptr);
	}
	// gpu영역에서 정점 가져오기
	// 주로 테셀레이션으로 늘어난 정점을 가져올 때 쓸거
	CONTEXT->CopyResource(stagingBuffer, m_vecIdxInfo[0].pIB.Get());
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	// 데이터 카피
	CONTEXT->Map(stagingBuffer, 0, D3D11_MAP_READ, 0, &mappedResource);

	UINT* data = static_cast<UINT*>(mappedResource.pData);


	// 늘어난 정점의 사이즈를 구한다.
	int size = (sizeof(UINT) / sizeof(data));

	for (int i = 0; i < size; ++i)
	{
		arr.push_back(data[i]);
	}

	CONTEXT->Unmap(stagingBuffer, 0);
	stagingBuffer->Release();



	return arr;
}

vector<Vtx> CMesh::GetVtx()
{
	vector<Vtx> vtx;
	Vtx* a = (Vtx*)m_pVtxSys;
	size_t b = sizeof(m_pVtxSys) / sizeof(Vtx);
	for (int i = 0; i < m_iVtxCount; ++i)
	{
		vtx.push_back(a[i]);
	}
	return vtx;
}