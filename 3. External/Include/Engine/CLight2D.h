#pragma once
#include "CComponent.h"

class CLight2D :
    public CComponent
{
private:
    tLightInfo      m_Info; // ±¤¿ø Á¤º¸

    int             m_iSkill;

public:
    const tLightInfo& GetLightInfo() { return m_Info; }

    void SetLightColor(Vec3 _vColor) { m_Info.vDiff = _vColor; }
    void SetLightAmbient(Vec3 _vColor) { m_Info.vAmb = _vColor; }

    void SetLightType(LIGHT_TYPE _eType) { m_Info.iLightType = _eType; }

    void SetRadius(float _fRadius) { m_Info.fRadius = _fRadius; }
    void SetAngle(float _fAngle){ m_Info.fAngle = _fAngle; }

    Vec3 GetLightColor() { return m_Info.vDiff; }
    Vec3 GetLightEmbient() { return m_Info.vAmb; }
    LIGHT_TYPE GetLightType() { return m_Info.iLightType; }
    float GetRadius() { return m_Info.fRadius; }
    float GetAngle() { return m_Info.fAngle; }
    


public:
    virtual void finaltick() override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CLight2D);

public:
    CLight2D();
    CLight2D(const CLight2D& _origin);
    ~CLight2D();
};

