#pragma once
#include <Engine/CScript.h>
class CUIScript :
    public CScript
{
private:
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CUIScript);

public:
    CUIScript();
    ~CUIScript();
};

