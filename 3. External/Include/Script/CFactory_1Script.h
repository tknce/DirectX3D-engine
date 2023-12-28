#pragma once
#include <Engine/CScript.h>
class CFactory_1Script :
    public CScript
{
private:


public:
    virtual void begin() override;
    virtual void tick() override;


    CLONE(CFactory_1Script)
public:
    CFactory_1Script();
    ~CFactory_1Script();
};

