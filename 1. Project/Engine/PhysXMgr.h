#pragma once
#include "singleton.h"

#pragma comment(lib,"PhysX\\PhysX_64.lib")
#pragma comment(lib,"PhysX\\PhysXFoundation_64.lib")
#pragma comment(lib,"PhysX\\PhysXExtensions_static_64.lib")
#pragma comment(lib,"PhysX\\PhysXCooking_64.lib")
#pragma comment(lib,"PhysX\\PhysXCommon_64.lib")
#pragma comment(lib,"PhysX\\PhysXPvdSDK_static_64.lib")
#pragma comment(lib,"PhysX\\PhysXCharacterKinematic_static_64.lib")



#include <PhysX/PxPhysics.h>
#include <PhysX/PxPhysicsAPI.h>

using namespace physx;

class EventCallBack;
class RaycastMgr;
class CRayCast;
class UserErrorCallback;
class MyPxUserControllerHitReport;
class MyPxControllerBehaviorCallback;

class PhysXMgr :
    public CSingleton<PhysXMgr>
{
private:
    //PhysEngine* m_pPhysEngine;
    
    UserErrorCallback*                      m_pDefaultErrorCallback;
    PxDefaultAllocator*                     m_pDefaultAllocatorCallback;
    PxDefaultCpuDispatcher*                 m_pDispatcher;
    PxTolerancesScale                       m_scale;

    // 디버그용
    PxPvd*                                  m_pPvd;
    PxPvdTransport*                         m_pTransport;

    // 관리객체
    PxPhysics*                              m_pPhysics;

    // 객체생성 본체
    PxFoundation*                           m_pFoundation;
    PxCooking*                              m_pCooking;
    PxScene*                                m_pScene;

    // 충돌이벤트 전달용 
    // PxSimulationEventCallback 상속받음
    EventCallBack*                          m_pEventCallback;

    // 
    RaycastMgr*                             m_pRaycastMgr;
    PxCudaContextManager*                   m_pCudaContextMgr;    
    PxControllerManager*                    m_pControllerMgr;

   // MyPxUserControllerHitReport*            m_pMyPxUserControllerHitReport;
   // MyPxControllerBehaviorCallback*         m_pMyPxControllerBehaviorCallback;
public:
    void init();
    void tick();

public:
    void init_Debug();
    void init_Relese();

    void Create_Start();
    void Create_Scene();
    void Create_Foundation();
    void Create_Pvd();
    void Create_Physics();
    void Create_Cooking();
    void Create_Plane();
    void Create_BroadPhaseRegion(int _size);
    void Create_ControllerMgr();
public:
    PxPhysics* GetPhys() { return m_pPhysics; }
    PxCooking* GetCooking() { return m_pCooking; }
    PxScene* GetScene() { return m_pScene; }
    RaycastMgr* GetRaycast() { return m_pRaycastMgr; }
    PxControllerManager* GetControllerMgr() { return m_pControllerMgr; }
    EventCallBack* GetEventCallback() { return m_pEventCallback; }
    //MyPxUserControllerHitReport* GetControllerHit() { return m_pMyPxUserControllerHitReport; }
    //MyPxControllerBehaviorCallback* GetControllerBehavior() { return m_pMyPxControllerBehaviorCallback; }
    bool Raycast(CRayCast* _ray);

    void addActor(PxActor* _actor, bool _isDynamic);
    void DelActor(PxActor* _actor);
public:
    PhysXMgr();
    ~PhysXMgr();
};

