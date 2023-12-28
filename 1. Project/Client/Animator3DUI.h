#pragma once
#include "ComponentUI.h"

#include <Engine/CAnimation3D.h>

class Animator3DUI :
    public ComponentUI
{
private:
    const vector<tMTAnimClip>*  m_pEntireClip;
    map<wstring,CGameObject*>   m_mapAnimenu;
    tMTAnimClip                 AnimClip;
    double                      m_dCurTime;

    string                      m_newStr;

    bool                        m_bCreate;
public:
    virtual void update() override;
    virtual void render_update() override;

    void Show();
    void Delete();
    void Reset();
public:
    void SetCurAnimation(DWORD_PTR str);

    void Create(bool open);

public:
    Animator3DUI();
    ~Animator3DUI();
};

