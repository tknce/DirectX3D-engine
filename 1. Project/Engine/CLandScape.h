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
    UINT                    m_iXFaceCount;      // ���� �� ����
    UINT                    m_iZFaceCount;      // ���� �� ����

    Vec2                    m_vBrushScale;      // �귯�� ũ��(��ü ������� ũ�� ������)
    Ptr<CTexture>           m_pBrushTex;        // �귯���� �ؽ���

    Ptr<CTexture>           m_pTileArrTex;      // Ÿ�� �迭 �ؽ���

    Ptr<CRaycastShader>     m_pCSRaycast;       // ��ŷ ���̴�
    CStructuredBuffer*      m_pCrossBuffer;	    // ���콺 ��ŷ�Ǵ� ���� ���� �޴� ����

    Ptr<CHeightMapShader>   m_pCSHeightMap;     // ���̸� ���̴�
    Ptr<CTexture>           m_pHeightMap;       // ���̸� �ؽ���


    Ptr<CWeightMapShader>   m_pCSWeightMap;     // ����ġ ���̴�
    CStructuredBuffer*      m_pWeightMapBuffer; // ����ġ ���� ����
    UINT					m_iWeightWidth;		// ����ġ ���� ���μ��� ��� ��
    UINT					m_iWeightHeight;	// ����ġ ���� ���μ��� ��� ��
    UINT                    m_iWeightIdx;		// ���� ��ų ����ġ ����

    LANDSCAPE_MOD           m_eMod; 	        // ���� ����忡�� ���°�

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


