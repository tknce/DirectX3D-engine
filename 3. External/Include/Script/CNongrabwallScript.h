#pragma once
#include <Engine/CScript.h>
class CNongrabwallScript :
    public CScript
{
public:
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:

    CLONE(CNongrabwallScript);
public:
    CNongrabwallScript();
    ~CNongrabwallScript();
};

