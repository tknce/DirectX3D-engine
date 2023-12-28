#pragma once
#include <Engine/CScript.h>
class CTileMapState :
    public CScript
{
private:
    bool        m_bAttack;

public:
    bool GetAttackState() { return m_bAttack; }

    void tick() override {}

    CLONE(CTileMapState);
public:
    CTileMapState();
    ~CTileMapState();
};

