#pragma once
#include "CComponent.h"

#include "Ptr.h"
#include "CMesh.h"
#include "CMaterial.h"

class CLight3D :
    public CComponent
{
private:
    tLightInfo      m_Info; // 광원 정보
    Ptr<CMesh>      m_pVolumeMesh;  // 광원의 범위를 표현하는 메쉬
    Ptr<CMaterial>  m_pLightMtrl;

    int             m_iLightIdx;    // 광원 버퍼에서의 인덱스

    CGameObject*    m_pLightCam;    // 광원 시점용 카메라

public:
    const tLightInfo& GetLightInfo() { return m_Info; }

    void SetLightDirection(Vec3 _vDir);
    void SetLightColor(Vec3 _vColor) { m_Info.vDiff = _vColor; }
    void SetLightSpecular(Vec3 _vSpec) { m_Info.vSpec = _vSpec; }
    void SetLightAmbient(Vec3 _vColor) { m_Info.vAmb = _vColor; }
    void SetLightType(LIGHT_TYPE _eType);

    void SetRadius(float _fRadius) { m_Info.fRadius = _fRadius; }
    void SetAngle(float _fAngle) { m_Info.fAngle = _fAngle; }

    Vec3 GetLightColor() { return m_Info.vDiff; }
    Vec3 GetLightEmbient() { return m_Info.vAmb; }
    LIGHT_TYPE GetLightType() { return m_Info.iLightType; }
    float GetRadius() { return m_Info.fRadius; }
    float GetAngle() { return m_Info.fAngle; }


public:
    virtual void finaltick() override;
    void render();
    void render_depthmap();

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CLight3D);
public:
    CLight3D();
    CLight3D(const CLight3D& _origin);
    ~CLight3D();
};

