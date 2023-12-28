#pragma once
#include <Engine/CScript.h>
class CBloodScript :
    public CScript
{
private:
    vector<tTile>           m_vecTile;
    CStructuredBuffer*      m_TileBuffer;

    Vec2                    m_vTileCount;

    bool                    m_bDataChanged;
public:
    virtual void begin() override;
    virtual void tick() override;

    void SetTileCount(UINT _iWidth, UINT _iHeight); // 만드는거
    void SetCount(UINT _iWidth, UINT _iHeight) { m_vTileCount = Vec2(_iWidth, _iHeight); m_bDataChanged = true; } // 바꾸는거

public:
    CLONE(CBloodScript);
public:
    CBloodScript();
    ~CBloodScript();
};

