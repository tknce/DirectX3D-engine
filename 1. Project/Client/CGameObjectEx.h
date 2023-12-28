#pragma once

#include <Engine\CGameObject.h>

class CGameObjectEx :
    public CGameObject
{
public:
    void finaltick();

public:
    CGameObjectEx();
    ~CGameObjectEx();
};

