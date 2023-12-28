#pragma once
#include <Engine/CScript.h>

class CTargetScript :
    public CScript
{
private:
    bool        m_b;
    float       m_fAcctime;
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;
    CLONE(CTargetScript);

public:
    CTargetScript();
    ~CTargetScript();
};

