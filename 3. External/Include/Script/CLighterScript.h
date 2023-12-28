#pragma once
#include <Engine/CScript.h>
class CLighterScript :
    public CScript
{
private:
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;
    CLONE(CLighterScript);

public:
    CLighterScript();
    ~CLighterScript();
};

