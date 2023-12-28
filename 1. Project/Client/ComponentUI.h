#pragma once
#include "UI.h"


class ComponentUI :
    public UI
{
private:
    CGameObject*    m_TargetObj;
    COMPONENT_TYPE  m_eType;

    bool            m_bDrag;
    ImVec2            m_vLeftTop;
    

public:
    void SetTarget(CGameObject* _pTarget) { m_TargetObj = _pTarget; }
    CGameObject* GetTarget() { return m_TargetObj; }
    COMPONENT_TYPE GetComponentType() { return m_eType; }


    

    void IsDrag(ImVec2 _LeftTop);



    virtual void render_update() override;

public:
    ComponentUI(const string& _UIName, COMPONENT_TYPE _eType);
    ~ComponentUI();
};

