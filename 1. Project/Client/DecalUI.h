#pragma once
#include "ComponentUI.h"
class DecalUI :
    public ComponentUI
{
private:
    bool m_bRightType;

public:


    virtual void update() override;
    virtual void render_update() override;

public:
    DecalUI();
    ~DecalUI();
};

