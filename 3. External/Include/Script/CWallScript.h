#pragma once
#include <Engine/CScript.h>
class CWallScript :
    public CScript
{
private:
    bool m_bColliderState;

public:
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

public:

    void Defend(CCollider2D* _pOther);


    CLONE(CWallScript);
public:
    CWallScript();
    ~CWallScript();

};

