#pragma once
#include <Engine/CScript.h>
class CDoorScript :
    public CScript
{
private:
    bool        m_bDoor;

    CAnimation2D* m_pAni;
public:

    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

    CLONE(CDoorScript);
public:

    CDoorScript();
    ~CDoorScript();
};

