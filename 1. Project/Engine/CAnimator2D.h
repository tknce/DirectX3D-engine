#pragma once
#include "CComponent.h"

#include "CTexture.h"

class CAnimation2D;

class CAnimator2D :
    public CComponent
{
private:
    map<wstring, CAnimation2D*> m_mapAnim;
    CAnimation2D*               m_pCurAnim;


    float                       m_fPI;
    bool                        m_bRepeat;

public:    
    virtual void finaltick() override;

public:
    void CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS);
    CAnimation2D* FindAnimation(const wstring& _strKey);
    const map<wstring, CAnimation2D*>& GetMapAnim() { return m_mapAnim; }
    void ChangeName(wstring _EraseName, wstring _NewName);

    CAnimation2D* GetCurAnim() { return m_pCurAnim; }
    float GetPI() { return m_fPI; }


public:

    CAnimation2D* Play(const wstring& _strKey, bool _bRepeat, bool _bReverse = false);
    CAnimation2D* Play_Rotation(const wstring& _strKey, bool _bRepeat, bool _bReverse = false);
    void Play_Load(const wstring& _strKey, int _idx);

    void UpdateData();
    void Clear();

public:
    void TurnbacktimeSave(FILE* _File);
    void TurnbacktimeLoad(FILE* _File);

    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);
    CLONE(CAnimator2D);

public:
    CAnimator2D();
    CAnimator2D(const CAnimator2D& _origin);
    ~CAnimator2D();
};

