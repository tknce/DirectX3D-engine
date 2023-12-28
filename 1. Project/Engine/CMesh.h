#pragma once
#include "CRes.h"

#include "CFBXLoader.h"
#include "CStructuredBuffer.h"

struct tIndexInfo
{
    ComPtr<ID3D11Buffer>    pIB;
    D3D11_BUFFER_DESC       tIBDesc;
    UINT				    iIdxCount;
    void* pIdxSysMem;
};



class CMesh :
    public CRes
{
private:
    ComPtr<ID3D11Buffer>    m_VB;
    D3D11_BUFFER_DESC       m_tVBDesc;
    UINT                    m_iVtxCount;
    void*                   m_pVtxSys;

    // 하나의 버텍스버퍼에 여러개의 인덱스버퍼가 연결
    vector<tIndexInfo>		m_vecIdxInfo;
    vector<UINT>            m_vecIdxSys;

    // Animation3D 정보
    vector<tMTAnimClip>		m_vecAnimClip;
    vector<tMTBone>			m_vecBones;

    CStructuredBuffer*      m_pBoneFrameData;   // 전체 본 프레임 정보(크기, 이동, 회전) (프레임 개수만큼)
    CStructuredBuffer*      m_pBoneOffset;	    // 각 뼈의 offset 행렬(각 뼈의 위치를 되돌리는 행렬) (1행 짜리)


public:
    Vtx* GetVtxSysMem() { return (Vtx*)m_pVtxSys; }
    UINT GetVtxCount() { return m_iVtxCount; }
    vector<UINT> GetIdxData() { return m_vecIdxSys; }
    UINT GetSubsetCount() { return (UINT)m_vecIdxInfo.size(); }
    vector<Vtx> GetVtxData();
    vector<UINT> GetIndexData();
    vector<Vtx> GetVtx();

    const vector<tMTBone>* GetBones() { return &m_vecBones; }
    UINT GetBoneCount() { return (UINT)m_vecBones.size(); }
    const vector<tMTAnimClip>* GetAnimClip() { return &m_vecAnimClip; }
    bool IsAnimMesh() { return !m_vecAnimClip.empty(); }

    CStructuredBuffer* GetBoneFrameDataBuffer() { return m_pBoneFrameData; } // 전체 본 프레임 정보
    CStructuredBuffer* GetBoneOffsetBuffer() { return  m_pBoneOffset; }	   // 각 뼈의 offset 행렬	    

protected:
    ComPtr<ID3D11Buffer> GetVB() { return m_VB; }
    vector<tIndexInfo> GetIB() { return m_vecIdxInfo; }
    

public:
    static CMesh* CreateFromContainer(CFBXLoader& _loader, int _index = 0);
    int Create(void* _pVtxSys, size_t _iVtxCount, void* _pIdxSys, size_t _iIdxCount);
    int Create(void* _pVtxSys, size_t _iVtxCount);


public:
    
    void render(UINT _iSubset);
    void render_instancing(UINT _iSubset);
    void render_particle(UINT _iCount);

    void UpdateData(UINT _iSubset);
    void UpdateData_Inst(UINT _iSubset);

    virtual void Save(const wstring& _strRelativePath) override;
    virtual int Load(const wstring& _strFilePath) override;


    CLONE_ASSERT(CMesh);

    friend class CRigidBody3D;
public:
    CMesh(bool _bEngineRes = false);
    CMesh(const CMesh& _origin);
    ~CMesh();
};
