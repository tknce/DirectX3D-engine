#pragma once
#include "CRenderComponent.h"

#include "Ptr.h"
#include "CTexture.h"

class CStructuredBuffer;

class CTileMap :
    public CRenderComponent
{
private:
    Ptr<CTexture>           m_AtlasTex;         // Ÿ�ϸ��� ����ϴ� ��Ʋ�� �̹���
    Ptr<CTexture>           m_AtlasTex_alpha;   // Ÿ�ϸ��� ���İ� �ؽ���
    Vec2                    m_vTileCount;       // Ÿ�ϸ� ���� ���� ����

    vector<tTile>           m_vecTile;          // Ÿ�� ����ȭ���� �� ����ü
    CStructuredBuffer*      m_TileBuffer;       // �� Ÿ���� ��Ʋ�� �������� ����ü

    Vec2                    m_vLeftTop;         // ������ ����
    Vec2                    m_vSlice;           // �ڸ� ����    

    UINT                    m_iIdx;             // �ٲ� Ÿ���ε���

    int                     m_iSkill;

    bool                    m_bDataChanged;     // ������ ü���� 

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

