#pragma once
#include <Engine/CScript.h>
class CUIcursorScript :
    public CScript
{
private:
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CUIcursorScript);

public:
    CUIcursorScript();
    ~CUIcursorScript();
};

