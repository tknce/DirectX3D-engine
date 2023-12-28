#pragma once
#include "CRenderComponent.h"

#include "Ptr.h"
#include "CTexture.h"

class CStructuredBuffer;

class CTileMap :
    public CRenderComponent
{
private:
    Ptr<CTexture>           m_AtlasTex;         // 타일맵이 사용하는 아틀라스 이미지
    Ptr<CTexture>           m_AtlasTex_alpha;   // 타일맵의 알파값 텍스쳐
    Vec2                    m_vTileCount;       // 타일맵 가로 세로 개수

    vector<tTile>           m_vecTile;          // 타일 구조화버퍼 속 구조체
    CStructuredBuffer*      m_TileBuffer;       // 각 타일의 아틀라스 참조정보 구조체

    Vec2                    m_vLeftTop;         // 왼쪽위 정보
    Vec2                    m_vSlice;           // 자를 정보    

    UINT                    m_iIdx;             // 바꿀 타일인덱스

    int                     m_iSkill;

    bool                    m_bDataChanged;     // 데이터 체인지 

public:
    void SetTileAtlas(Ptr<CTexture> _AtlasTex) { m_AtlasTex = _AtlasTex; }
    Ptr<CTexture> GetTileAtlas() { return m_AtlasTex; }
    void SetTileCount(UINT _iWidth, UINT _iHeight);
    void SetCount(UINT _iWidth, UINT _iHeight) { m_vTileCount = Vec2(_iWidth, _iHeight); m_bDataChanged = true; }
    Vec2 GetTileCount() { return m_vTileCount; }
    const vector<tTile>& GetvecTile() { return m_vecTile; }

    void SetIdxTile(UINT _Idx, Vec2 _LeftTop, Vec2 _Slice, bool _SetUV);

    void SetLeftTop(Vec2 _LeftTop) { m_vLeftTop = _LeftTop; }
    Vec2 GetLeftTop() { return m_vLeftTop;  }

    void SetSlice(Vec2 _Slice) { m_vSlice = _Slice; }
    Vec2 GetSlice() { return m_vSlice; }
    
    

public:
    virtual void finaltick() override;
    virtual void render() override;
    virtual void render(UINT _iSubset) override;

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    CLONE(CTileMap);

public:
    CTileMap();
    CTileMap(const CTileMap& _origin);
    ~CTileMap();
};

