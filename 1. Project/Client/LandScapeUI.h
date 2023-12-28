#pragma once
#include "ComponentUI.h"
//Vec2 GetFaceCount() { return Vec2(m_iXFaceCount, m_iZFaceCount); }
//Vec2 GetWeightResolution() { return Vec2(m_iWeightWidth, m_iWeightHeight); }
//Vec2 GetBrushScale() { return m_vBrushScale; }
//
//UINT GetWeightIdx() { return m_iWeightIdx; }
//UINT GetMod() { return UINT(m_eMod); }
//vector<Ptr<CTexture>> GetHightBrushArrTex();

class LandScapeUI :
    public ComponentUI
{
private:

    Vec2                    m_vFaceCount;
    Vec2                    m_vWeightResolution;
    Vec2                    m_vBrushScale;

    int                    m_iWeightIdx;
    int                    m_iMod;

    vector<Ptr<CTexture>>   m_vTex;

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    LandScapeUI();
    ~LandScapeUI();
};

