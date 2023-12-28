#pragma once
#include "ComponentUI.h"

class SliceMapImage;
class TileMapUI :
    public ComponentUI
{
private:
    Ptr<CMaterial>          m_pMaterial;

    bool                    m_bTileMap;
    bool                    m_bEditMode;
    bool                    m_bSliceImage;
    bool                    m_bTileReset;
    bool                    m_bTilecut;

    float                   m_fTileCount[2];
    float                   m_fSlice;

    ImVec2                  m_vLeftTop;
    ImVec2                  m_vSlice;

    ImVec2                  m_vGridOffset;

    ImVec2                  m_vLeftTopMouse;
    ImVec2                  m_vSliceMouse;

    ImVec2                  m_vSliceCount;

    vector<SliceMapImage*>  m_vTileMap;
    vector<SliceMapImage*>  m_vSliceMapImage;
private:
    virtual void update() override;
    virtual void render_update() override;
public:
    void SetLeftTop(ImVec2 _Lefttop) { m_vLeftTop = _Lefttop; }
    ImVec2 GetLeftTop() { return m_vLeftTop; }

    void SetSlice(ImVec2 _Slice) { m_vSlice = _Slice; }
    ImVec2 GetSlice() { return m_vSlice; }

    bool GetEditMode() { return m_bEditMode; }

    void SetGirdSlice(ImVec2 setpos, ImVec2 slicesize);
    void SetTexture(DWORD_PTR _strTexKey);

    void SetMaterial(DWORD_PTR _strTexKey);
    void SetMesh(DWORD_PTR _strTexKey);
public:
    TileMapUI();
    ~TileMapUI();
};

