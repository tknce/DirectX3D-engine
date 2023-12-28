#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"

struct tMtrlSet
{
    Ptr<CMaterial>  pCurMtrl;       // 현재 사용 할 메테리얼
    Ptr<CMaterial>  pSharedMtrl;    // 공유 메테리얼
    Ptr<CMaterial>  pDynamicMtrl;   // 공유 메테리얼의 복사본    
};


class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>              m_pMesh;

    vector<tMtrlSet>        m_vecMtrls;     // 재질    

    bool                    m_bUseFrustumCulling;
    bool                    m_bDynamicShadow;

public:
    virtual void render() = 0;
    virtual void render(UINT _iSubset) = 0;
    virtual void render_depthmap();

public:
    void SetMesh(Ptr<CMesh> _pMesh);
    void SetMesh_edit(Ptr<CMesh> _pMesh) { m_pMesh = _pMesh; };
    Ptr<CMesh> GetMesh() { if (m_pMesh == nullptr) return nullptr; return m_pMesh; }

    UINT GetMtrlCount() { return (UINT)m_vecMtrls.size(); }

    void SetSharedMaterial(Ptr<CMaterial> _pMtrl, UINT _iIdx);

    Ptr<CMaterial> GetCurMaterial(UINT _iIdx);
    Ptr<CMaterial> GetSharedMaterial(UINT _idx);
    Ptr<CMaterial> GetDynamicMaterial(UINT _iIdx);

    void SetFrustumCulling(bool _Use) { m_bUseFrustumCulling = _Use; }
    bool IsUseFrustumCulling() { return m_bUseFrustumCulling; }

    void SetDynamicShadow(bool _bSet) { m_bDynamicShadow = _bSet; }
    bool IsDynamicShadow() { return m_bDynamicShadow; }

    ULONG64 GetInstID(UINT _iMtrlIdx);
public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    virtual CRenderComponent* Clone() = 0;
public:
    CRenderComponent(COMPONENT_TYPE _eComponentType);
    CRenderComponent(const CRenderComponent& _origin);
    ~CRenderComponent();
};

