#pragma once
#include "CRenderComponent.h"

#include "CRaycastShader.h"
#include "CHeightMapShader.h"
#include "CWeightMapShader.h"

class CStructuredBuffer;

class CLandScape :
    public CRenderComponent
{
private:
    UINT                    m_iXFaceCount;      // 지형 면 개수
    UINT                    m_iZFaceCount;      // 지형 면 개수

    Vec2                    m_vBrushScale;      // 브러쉬 크기(전체 지형대비 크기 비율값)
    Ptr<CTexture>           m_pBrushTex;        // 브러쉬용 텍스쳐

    Ptr<CTexture>           m_pTileArrTex;      // 타일 배열 텍스쳐

    Ptr<CRaycastShader>     m_pCSRaycast;       // 픽킹 쉐이더
    CStructuredBuffer*      m_pCrossBuffer;	    // 마우스 피킹되는 지점 정보 받는 버퍼

    Ptr<CHeightMapShader>   m_pCSHeightMap;     // 높이맵 쉐이더
    Ptr<CTexture>           m_pHeightMap;       // 높이맵 텍스쳐


    Ptr<CWeightMapShader>   m_pCSWeightMap;     // 가중치 쉐이더
    CStructuredBuffer*      m_pWeightMapBuffer; // 가중치 저장 버퍼
    UINT					m_iWeightWidth;		// 가중치 버퍼 가로세로 행렬 수
    UINT					m_iWeightHeight;	// 가중치 버퍼 가로세로 행렬 수
    UINT                    m_iWeightIdx;		// 증가 시킬 가중치 부위

    LANDSCAPE_MOD           m_eMod; 	        // 지형 툴모드에서 상태값

    Ptr<CMaterial>          m_LandScapeMtrl;
    vector<UINT>            m_sample;
public:
    virtual void finaltick() override;
    virtual void render() override;
    virtual void render(UINT _iSubset) override;
public:
    void SetFaceCount(UINT _X, UINT _Z);
    void SetWeightResolution(Vec2 weightresolution) { m_iWeightWidth = weightresolution.x; m_iWeightHeight = weightresolution.y; }
    void SetWeightIdx(UINT idx) { m_iWeightIdx = idx; }
    void SetBrushScale(Vec2 _vscale) { m_vBrushScale = _vscale; }
    void SetBrushTex(Ptr<CTexture> brushtex) { m_pBrushTex = brushtex; }
    void SetHeightTex(Ptr<CTexture> heighttex) { m_pHeightMap = heighttex; }
    void SetTileArrTex(Ptr<CTexture> tiletex) { m_pTileArrTex = tiletex; }
    void SetHightBrushArrTex(vector<Ptr<CTexture>> tex);
    void SetMod(UINT mod)
    {
        if (LANDSCAPE_MOD::NONE > (LANDSCAPE_MOD)mod)
            m_eMod = (LANDSCAPE_MOD)mod;
    }

public:
    Vec2 GetFaceCount() { return Vec2(m_iXFaceCount, m_iZFaceCount); }
    Vec2 GetWeightResolution() { return Vec2(m_iWeightWidth, m_iWeightHeight); }
    Vec2 GetBrushScale() { return m_vBrushScale; }

    Ptr<CTexture>GetHeightTex() { return m_pHeightMap; }

    UINT GetWeightIdx() { return m_iWeightIdx; }
    UINT GetMod() { return UINT(m_eMod); }

    vector<Ptr<CTexture>> GetHightBrushArrTex();

    
    vector<UINT> getsample() { return m_sample; }
   // 


private:
    void CreateMesh();
    void CreateMaterial();
    void CreateTexture();
    void Raycasting();

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CLandScape);
public:
    CLandScape();
    ~CLandScape();
};


