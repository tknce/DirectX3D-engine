#pragma once
#include "ComponentUI.h"

#include <Engine/Ptr.h>
#include <Engine/CTexture.h>

class CAnimation2D;

class Animator2DUI :
    public ComponentUI
{
private:
    int                 m_iCurIdx;

    int                 m_iSelectedIdx;
    int                 m_AnimPreviewIdx;
    int                 m_iItemCurIdx;

    float               m_Acctime;
    float               m_fFPS;

    Ptr<CTexture>       m_AtlasTex;
    
    CAnimation2D*       m_Animation;
    vector<tAnim2DFrm>  m_vecFrmUI;
    vector<string>      m_vCurAni;

    string              m_sNewName;
    string              m_PreName;
    wstring             m_PreOwnerName;

    ImVec2              m_vLeftTopMouse;
    ImVec2              m_vSliceMouse;

    ImVec2              m_vSceneSize;

    Vec2                m_vSliceEX;

    Vec4                m_vSlice;

    bool                m_bPlay;
    bool                m_bAnimation;
    bool                m_bnewani;
    bool                m_bOrginNomove;
    bool                m_bSign;
    bool                m_Revise;

private:
    virtual void update() override;
    virtual void render_update() override;

    void OriginalImage();
    void Animation();
    void SetInfo();
    void NewFrm();
    void ReName();
    void SetIsDrag();

    void Reset();
public:
    void SetTexture(DWORD_PTR _strTexKey);


public:
    Animator2DUI();
    ~Animator2DUI();
};

