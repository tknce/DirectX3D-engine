#pragma once
#include <Engine/CScript.h>
class CClear_1Script :
    public CScript
{
private:
    bool nextstap;

public:

    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;

    CLONE(CClear_1Script);
public:

    CClear_1Script();
    ~CClear_1Script();

};

