#pragma once
#include <Engine/CScript.h>
class C3DPlayerScript :
    public CScript
{
private:


public:
    virtual void begin() override;
    virtual void tick() override;


    CLONE(C3DPlayerScript)
public:
    C3DPlayerScript();
    ~C3DPlayerScript();
};

