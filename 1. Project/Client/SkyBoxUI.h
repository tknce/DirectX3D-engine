#pragma once
#include "ComponentUI.h"
class SkyBoxUI :
    public ComponentUI
{
private:
    int             m_iType; // 0 : ±∏ 1 : ¡§¿∞∏È√º

    Ptr<CTexture>   m_pTexture;


public:
    virtual void update() override;
    virtual void render_update() override;

public:
    SkyBoxUI();
    ~SkyBoxUI();
};

