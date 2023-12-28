#pragma once
#include <Engine/CScript.h>
class CRazerScript :
    public CScript
{
private:
    float m_fRazer;

    CGameObject* m_pParticle;
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

    CLONE(CRazerScript);

public:
    CRazerScript();
    ~CRazerScript();
};

