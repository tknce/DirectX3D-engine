#pragma once
#include <Engine/CScript.h>
class CStairScript :
    public CScript
{
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;
    CLONE(CStairScript);


    CStairScript();
    ~CStairScript();
};

