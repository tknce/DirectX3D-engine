#pragma once
#include "CComponent.h"
#include "PhysXMgr.h"

class CRayCast;
class CPlayerController;
struct FilterGroup
{
    enum Enum
    {
        eSUBMARINE = (1 << 0),
        eMINE_HEAD = (1 << 1),
        eMINE_LINK = (1 << 2),
        eCRAB = (1 << 3),
        eHEIGHTFIELD = (1 << 4),

        eSPHERE = (1 << 5),
        ePLAYER = (1 << 6),
    };
};
enum class TYPE
{
    BOX,
    SPHERE,
    CAPSULE,
    MESH,
    TERRAIN,
};


class CRigidBody3D :
    public CComponent
{
private:
    //PhysData* m_pPhysData;

    bool        m_bisDinamic;
    bool        m_bisgravity;
    bool        m_bEnter;
    
    bool        m_isPosition;
    bool        m_isForce;
    bool        m_isVelocity;

    Vec3        m_vForce;
    Vec3        m_vVelocity;

    Vec3        m_vRot;

    Vec3        m_vFreezePos = {0,0,0};
    Vec3        m_vFreezeRot = { 0,0,0 };

    TYPE        m_ColliderType;

    PxMaterial* m_pMaterial;
    CRayCast*   m_pRayCast;

    CPlayerController* m_pController;

    float       m_fMass;

    void* ActorObj = nullptr;
public:
    void Create();
    void Create_Static(PxShape* shape, PxTransform* Tr);
    void Create_Dynamic(PxShape* shape, PxTransform* Tr);

    void Delete();
    
    void CreateHeightMap();
    void CreateHeightMap1();
    void CreateConvex();        // phys에서 알아서 정점 만들어줌 사실상 필요없을듯
    void CreateTriangle();      // phys에서 나 이런식으로 만들었으니 만들어줘
    void CreatePlayerController(Vec3 _vec = {0,0,0});

    PxShape* CreateShape();
    void SettingColliderType();
    void SettingPlayer();

    void StaticChange(bool _staticbool);
public:
    void SetPosition(bool _isposition) { m_isPosition = _isposition; }
    void SetVelocity(Vec3 _velocity) { m_isVelocity = true;  m_vVelocity = _velocity; }
    void SetAxisLock(Vec3 _rockPos,Vec3 _rockRot);
    void SetMass(float _mass);
    void SetRot(Vec3 _Rot);

    bool GetPosition() { return m_isPosition; }
    Vec3 GetPosition_info();
    Vec3 GetVelocity_info();
    Vec3 GetAngularVelocity_info();
    Vec3 GetRot() { return m_vRot; }
    PxTransform GetCurTransform();
    CPlayerController* GetPlayercontroller() { if (m_pController == nullptr) return nullptr;  return m_pController; }

    void AddControllVelocity(Vec3 _velop);
    void setupFiltering(PxRigidActor* actor, PxU32 filterGroup, PxU32 filterMask);

    bool isvaild();

public:
    // 편의 관련 기능들
    PxVec3 Vec3toPxVec3(Vec3 _vec3, Vec3 _Scale = Vec3(1.f,1.f,1.f)) { return PxVec3(_vec3.x * _Scale.x, _vec3.y * _Scale.y, _vec3.z * _Scale.z); }
    void GetVertexData(vector<PxVec3>& _vdata, vector<Triangle>& _vidata);

public:
    virtual void begin() override;
    virtual void finaltick() override;

    void tickActor();
    void tickDynamicPosition(PxRigidDynamic* _Dynamic);
    void tickDynamicForce(PxRigidDynamic* _Dynamic);
    void tickDynamicVelocity(PxRigidDynamic* _Dynamic);
    void tickPosnRot(PxRigidDynamic* Dynamic);

    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    bool  GetDinamic() { return m_bisDinamic; }
    bool  GetGravity() { return m_bisgravity; }


    CLONE(CRigidBody3D);

public:
    CRigidBody3D();
    CRigidBody3D(const CRigidBody3D& _origin);
    ~CRigidBody3D();
};

