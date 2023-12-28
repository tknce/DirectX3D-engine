#pragma once
#include "CEntity.h"

#include "CTexture.h"
class CAnimator2D;

class CAnimation2D :
    public CEntity
{
private:
    vector<tAnim2DFrm>  m_vecFrm;
    int                 m_iCurIdx;

    CAnimator2D*        m_pOwner;
    Ptr<CTexture>       m_AtlasTex;

    float               m_fAccTime;
    

    bool                m_bFinish;
    bool                m_bLoad;

public:
    void finaltick();


public:
    void Create(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS);
    void Reset()
    {
        m_fAccTime = 0.f;
        m_iCurIdx = 0;
        m_bFinish = false;
    }

    const vector<tAnim2DFrm>& GetAnimFrm() { return m_vecFrm; }
    Ptr<CTexture> GetAtlas() { return m_AtlasTex; }

    void SetAtlas(Ptr<CTexture> _Tex) { m_AtlasTex = _Tex; }
    void SetVector(vector<tAnim2DFrm> _2DFrm) { m_vecFrm = _2DFrm; }
    void SetFrm(tAnim2DFrm _Frm, int _idx) { if (m_vecFrm.size() < _idx) return; m_vecFrm[_idx] = _Frm; }
    void SetFrm(vector<tAnim2DFrm> _Frm) { m_vecFrm = _Frm; }
    void SetLoad(bool _Set) { m_bLoad = _Set; }
    void SetIdx(int _idx) { m_iCurIdx = _idx; }
    

    bool GetFinish() { return m_bFinish; }
    int  GetIDX() { return m_iCurIdx; }
    
public:

    void UpdateData();
    void Clear();

public:

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    CLONE(CAnimation2D);
public:
    CAnimation2D();
    //CAnimation2D(const CAnimation2D& _origin);
    ~CAnimation2D();

    friend class CAnimator2D;
};

