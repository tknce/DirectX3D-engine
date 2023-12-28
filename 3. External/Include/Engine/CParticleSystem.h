#pragma once
#include "CRenderComponent.h"

#include "CParticleUpdateShader.h"

class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
private:
    UINT                        m_iMaxCount;
    UINT                        m_iAliveCount;

    Vec4                        m_vStartScale;
    Vec4                        m_vEndScale;

    Vec4                        m_vStartColor;
    Vec4                        m_vEndColor;

    Vec2                        m_vMinMaxSpeed;
    Vec2                        m_vMinMaxLifeTime;    

    Vec2                        m_vVelopcity;
    Vec2                        m_vForce;

    float                       m_fSpawnRange;
    Vec4                        m_vSeta;

    float                       m_Frequency;    // 초당 파티클 생성 수
    float                       m_fAccTime;     // 시간 누적

    int                         m_WorldSpawn;   

    CStructuredBuffer*          m_ParticleBuffer;
    CStructuredBuffer*          m_ParticleShare;
    Ptr<CParticleUpdateShader>  m_UpdateCS;


public:
    bool GetWorldSpawn() { return m_WorldSpawn; }
    void SetWorldSpawn(bool _bSet) { m_WorldSpawn = (int)_bSet; }

    float GetSpawnRange() { return m_fSpawnRange; }
    void SetSpawnRange(float _Range) { m_fSpawnRange = _Range; }
    
    UINT GetMaxCount() { return m_iMaxCount; }
    void SetMaxCount(int _Count) { m_iMaxCount = (UINT)_Count; }

    UINT GetAliveCount() { return m_iAliveCount; }
    void SetAliveCount(int _Count) { m_iAliveCount = (UINT)_Count; }

    Vec4 GetStartScale() { return m_vStartScale; }
    Vec4 GetEndScale() { return m_vEndScale; }
    void SetStartEndScale(Vec4 _Start, Vec4 _End) { m_vStartScale = _Start; m_vEndScale = _End; }

    Vec4 GetStartColor() { return m_vStartColor; }
    Vec4 GetEndColor() { return m_vEndColor; }
    void SetStartEndColor(Vec4 _Start, Vec4 _End) { m_vStartColor = _Start; m_vEndColor = _End; }

    Vec2 GetMinMaxSpeed() { return m_vMinMaxSpeed; }
    void SetMinMaxSpeed(Vec2 _MinMaxSpeed) { m_vMinMaxSpeed = _MinMaxSpeed; }

    Vec2 GetMinMaxLifeTime() { return m_vMinMaxLifeTime; }
    void SetMinMaxLifeTime(Vec2 _MinMaxLifeTime) { m_vMinMaxLifeTime = _MinMaxLifeTime; }
    
    float GetFrequency() { return m_Frequency; }
    void SetFrequency(float _Frequency) {  m_Frequency = _Frequency; }

    Vec4 GetDir() { return m_vSeta; }
    void SetDir(Vec4 _vSeta) { m_vSeta = _vSeta; }


public:
    Ptr<CParticleUpdateShader> GetComputeShader() { return m_UpdateCS; }
    void SetComputeShader(wstring _Compute) { m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(_Compute).Get();}

public:
    virtual void finaltick() override;
    virtual void render() override;
    virtual void render(UINT _iSubset) override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    CLONE(CParticleSystem);
public:
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _origin);
    ~CParticleSystem();
};

