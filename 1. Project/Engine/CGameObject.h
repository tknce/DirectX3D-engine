#pragma once
#include "CEntity.h"

class CComponent;
class CTransform;
class CRenderComponent;
class CCollider2D;
class CAnimator2D;
class CAnimator3D;
class CLight2D;
class CLight3D;
class CCamera;
class CTileMap;
class CRigidBody2D;
class CRigidBody3D;
class AI;
class CDecal;
class CSkyBox;
class CLandScape;

class CMeshRender;
class CParticleSystem;

class CScript;


#define GET_COMPONENT(classname, CLASSNAME ) C##classname* classname() { return ( C##classname*)GetComponent(COMPONENT_TYPE::CLASSNAME); }
#define GET_COMPONENT_AI(classname, CLASSNAME ) classname* classname() { return ( classname*)GetComponent(COMPONENT_TYPE::CLASSNAME); }


class CGameObject :
    public CEntity
{
private:
    CGameObject*            m_pParent;

    CComponent*             m_arrCom[(UINT)COMPONENT_TYPE::END];
    vector<CScript*>        m_vecScripts;

    CRenderComponent*       m_pRenderComponent;

    int                     m_iLayerIdx;
    bool                    m_bDead;

protected:
    vector<CGameObject*>    m_vecChild;

public:
    void begin();
    void tick();
    void finaltick();
    void finaltick_module();    // 레벨에 포함되지 않고 사용 하는 경우
    void render();


public:
    void AddComponent(CComponent* _pComponent);
    CComponent* GetComponent(COMPONENT_TYPE _eComType) { return m_arrCom[(UINT)_eComType]; }
    void DeleteComponent(COMPONENT_TYPE _eComType); 
    void DeleteScript(int _scriptidx);
    bool ComponentCheck(COMPONENT_TYPE _eComType);
    void AddChild(CGameObject* _pChild);
    void AddChild_LOAD(CGameObject* _pChild);

    template<typename T>
    T* GetScript();

    GET_COMPONENT(Transform, TRANSFORM);
    GET_COMPONENT(MeshRender, MESHRENDER);
    GET_COMPONENT(Camera, CAMERA);
    GET_COMPONENT(Collider2D, COLLIDER2D);
    GET_COMPONENT(Animator2D, ANIMATOR2D);
    GET_COMPONENT(Animator3D, ANIMATOR3D);
    GET_COMPONENT(Light2D, LIGHT2D);
    GET_COMPONENT(Light3D, LIGHT3D);
    GET_COMPONENT(ParticleSystem, PARTICLESYSTEM);
    GET_COMPONENT(TileMap, TILEMAP);
    GET_COMPONENT(RigidBody2D, RIGIDBODY2D);
    GET_COMPONENT(RigidBody3D, RIGIDBODY3D);
    GET_COMPONENT(Decal, DECAL);
    GET_COMPONENT(SkyBox, SKYBOX);
    GET_COMPONENT(LandScape, LANDSCAPE);

    AI* MonAI() { return (AI*)GetComponent(COMPONENT_TYPE::AI); }

    CRenderComponent* GetRenderComponent() { return m_pRenderComponent; }

    CGameObject* GetParent() { return m_pParent; }
    CGameObject* GetChildwstring(wstring _ChildName);

    const vector<CGameObject*>& GetChildObject() const { return m_vecChild; }
    const vector<CScript*>& GetScripts()const { return m_vecScripts; }
    
    void DisconnectFromParent();

    int GetLayerIdx() { return m_iLayerIdx; }
    void SetLayerIdx(int _layeridx) { m_iLayerIdx = _layeridx; }
    bool IsDead() { return m_bDead; }
    void Destroy();

    CLONE(CGameObject);
public:
    CGameObject();
    CGameObject(const CGameObject& _origin);
    virtual ~CGameObject();

    friend class CLayer;
    friend class CEventMgr;
};

template<typename T>
inline T* CGameObject::GetScript()
{
    for (size_t i = 0; i < m_vecScripts.size(); ++i)
    {
        T* pScript = dynamic_cast<T*>(m_vecScripts[i]);
        if (nullptr != pScript)
            return pScript;
    }

    return nullptr;
}
