#pragma once
#include "ComponentUI.h"

class Light2DUI :
    public ComponentUI
{
private:
    Vec3        m_vLightColor;
    Vec3        m_vEmbient;

    LIGHT_TYPE  m_Type;

    float       m_fRadius;
    float       m_fAngle;

    float       m_fDir;


private:
    virtual void update() override;
    virtual void render_update() override;

    void SetLightType(DWORD_PTR _type);

public:
    Light2DUI();
    ~Light2DUI();
};

