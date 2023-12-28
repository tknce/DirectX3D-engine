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

    // �ϳ��� ���ؽ����ۿ� �������� �ε������۰� ����
    vector<tIndexInfo>		m_vecIdxInfo;
    vector<UINT>            m_vecIdxSys;

    // Animation3D ����
    vector<tMTAnimClip>		m_vecAnimClip;
    vector<tMTBone>			m_vecBones;

    CStructuredBuffer*      m_pBoneFrameData;   // ��ü �� ������ ����(ũ��, �̵�, ȸ��) (������ ������ŭ)
    CStructuredBuffer*      m_pBoneOffset;	    // �� ���� offset ���(�� ���� ��ġ�� �ǵ����� ���) (1�� ¥��)


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

    CStructuredBuffer* GetBoneFrameDataBuffer() { return m_pBoneFrameData; } // ��ü �� ������ ����
    CStructuredBuffer* GetBoneOffsetBuffer() { return  m_pBoneOffset; }	   // �� ���� offset ���	    

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
