#pragma once
#include "CComponent.h"

#include "Ptr.h"
#include "CMesh.h"
#include "CMaterial.h"

class CLight3D :
    public CComponent
{
private:
    tLightInfo      m_Info; // ���� ����
    Ptr<CMesh>      m_pVolumeMesh;  // ������ ������ ǥ���ϴ� �޽�
    Ptr<CMaterial>  m_pLightMtrl;

    int             m_iLightIdx;    // ���� ���ۿ����� �ε���

    CGameObject*    m_pLightCam;    // ���� ������ ī�޶�

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

