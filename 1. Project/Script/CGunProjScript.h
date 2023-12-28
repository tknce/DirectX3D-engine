#pragma once
#include <Engine/CScript.h>
class CGunProjScript :
    public CScript
{
private:
    CGameObject*    m_TargetObj;

    Vec3            m_vDir;

    bool            m_bSpark;
public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:
    CLONE(CGunProjScript);

public:
    CGunProjScript();
    ~CGunProjScript();
};

