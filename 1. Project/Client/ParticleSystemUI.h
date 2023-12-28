#pragma once
#include "ComponentUI.h"
class ParticleSystemUI :
    public ComponentUI
{
private:
    int                         m_iMaxCount;
    int                         m_iAliveCount;

    Ptr<CParticleUpdateShader>  m_pComputueshader;
    Ptr<CTexture>               m_pTex;
    Ptr<CMaterial>              m_pMaterrial;

    Vec4                        m_vStartScale;
    Vec4                        m_vEndScale;

    Vec4                        m_vStartColor;
    Vec4                        m_vEndColor;

    Vec2                        m_vMinMaxSpeed;
    Vec2                        m_vMinMaxLifeTime;

    float                       m_fSpawnRange;

    float                       m_Frequency; 

    bool                        m_WorldSpawn;



private:
    virtual void update() override;
    virtual void render_update() override;

    void SetComputeshader(DWORD_PTR _strMaterialKey);
    void SetTexture(DWORD_PTR _strMaterialKey);
    void SetMaterial(DWORD_PTR _strMaterialKey);

public:
    ParticleSystemUI();
    ~ParticleSystemUI();
};

