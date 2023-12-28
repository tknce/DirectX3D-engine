#pragma once
#include <Engine/CScript.h>
class CBrokenObject :
    public CScript
{
private:

public:
    virtual void tick() override;

    CLONE(CBrokenObject);

public:
    CBrokenObject();
    ~CBrokenObject();

};

