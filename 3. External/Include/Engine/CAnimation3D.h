#pragma once
#include "CEntity.h"

#include "CAnimator3D.h"

class CAnimation3D :
    public CEntity
{
private:
    CAnimator3D*            m_pOwner;

    vector<tMTAnimClip>    m_pVecClip;


public:
    void UpdateData();

public:
    void SetAnim3D(tMTAnimClip _Clip) { m_pVecClip.push_back(_Clip); }
    void SetOwner(CAnimator3D* _powner) { m_pOwner = _powner; }
    vector<tMTAnimClip> GetAnim3D() { return m_pVecClip; }
    
    CLONE(CAnimation3D);

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CAnimation3D();
    ~CAnimation3D();

    friend CAnimator3D;
};

