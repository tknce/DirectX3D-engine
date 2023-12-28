#pragma once
#include "UI.h"

class CGameObject;
class ComponentUI;
class ScriptUI;

#include <Engine\Ptr.h>
#include <Engine\CRes.h>
class ResUI;


class InspectorUI :
    public UI
{
private:
    CGameObject*    m_TargetObj;
    ComponentUI*    m_arrComUI[(UINT)COMPONENT_TYPE::END];

    vector<ScriptUI*> m_vecScriptUI;
    vector<string>  m_vLayer;

    string          m_OBJRENAME;

    WORD            m_matWord[MAX_LAYER];

    Ptr<CRes>       m_TargetRes;
    ResUI*          m_arrResUI[(UINT)RES_TYPE::END];

    int             m_iCurIdx;

    bool            m_bLayerSet;
    bool            m_bLSet;
    bool            m_bLDel;
    bool            m_bReName;

public:
    CGameObject* GetTargetObject() { return m_TargetObj; }


    void SetTargetObject(CGameObject* _Target);
    void SetTargetResource(CRes* _Resource);
    void AddComponent(DWORD_PTR _componenttype);
    void DelComponent(DWORD_PTR _componenttype);
    void ChangeLayer(DWORD_PTR _LayerIdx);
    void SetColliderLayer(DWORD_PTR _LayerIdx);
    
    void ClearScript() { m_vecScriptUI.clear(); }

private:
    virtual void update() override;
    virtual void render_update() override;

public:
    InspectorUI();
    ~InspectorUI();
};

