#include "pch.h"
#include "PhysXMgr.h"

#include "EventCallBack.h"
#include "RaycastMgr.h"
#include "CRayCast.h"
#include "UserErrorCallback.h"
#include "MyPxControllerBehaviorCallback.h"
#include "MyPxUserControllerHitReport.h"
//#include <PhysX/PxConfig.h>
enum
{
	COLLISION_FLAG_GROUND = 1 << 0,
	COLLISION_FLAG_WHEEL = 1 << 1,
	COLLISION_FLAG_CHASSIS = 1 << 2,
	COLLISION_FLAG_OBSTACLE = 1 << 3,
	COLLISION_FLAG_DRIVABLE_OBSTACLE = 1 << 4,

	COLLISION_FLAG_GROUND_AGAINST = COLLISION_FLAG_CHASSIS | COLLISION_FLAG_OBSTACLE | COLLISION_FLAG_DRIVABLE_OBSTACLE,
	COLLISION_FLAG_WHEEL_AGAINST = COLLISION_FLAG_WHEEL | COLLISION_FLAG_CHASSIS | COLLISION_FLAG_OBSTACLE,
	COLLISION_FLAG_CHASSIS_AGAINST = COLLISION_FLAG_GROUND | COLLISION_FLAG_WHEEL | COLLISION_FLAG_CHASSIS | COLLISION_FLAG_OBSTACLE | COLLISION_FLAG_DRIVABLE_OBSTACLE,
	COLLISION_FLAG_OBSTACLE_AGAINST = COLLISION_FLAG_GROUND | COLLISION_FLAG_WHEEL | COLLISION_FLAG_CHASSIS | COLLISION_FLAG_OBSTACLE | COLLISION_FLAG_DRIVABLE_OBSTACLE,
	COLLISION_FLAG_DRIVABLE_OBSTACLE_AGAINST = COLLISION_FLAG_GROUND | COLLISION_FLAG_CHASSIS | COLLISION_FLAG_OBSTACLE | COLLISION_FLAG_DRIVABLE_OBSTACLE
};

PxFilterFlags VehicleFilterShader
(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	PX_UNUSED(attributes0);
	PX_UNUSED(attributes1);
	PX_UNUSED(constantBlock);
	PX_UNUSED(constantBlockSize);
	
	if ((0 == (filterData0.word0 & filterData1.word1)) && (0 == (filterData1.word0 & filterData0.word1)))
		return PxFilterFlag::eSUPPRESS;

	pairFlags = PxPairFlag::eCONTACT_DEFAULT;
	pairFlags |= PxPairFlags(PxU16(filterData0.word2 | filterData1.word2));

	return PxFilterFlags();
}

PxFilterFlags SampleSubmarineFilterShader
(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	// let triggers through
	// if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	// {
	// 	pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
	// 	return PxFilterFlag::eDEFAULT;
	// }

	// generate contacts for all that were not filtered above
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	// trigger the contact callback for pairs (A,B) where
	// the filtermask of A contains the ID of B and vice versa.
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;

	return PxFilterFlag::eDEFAULT;
}

PxFilterFlags SampleFilterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}
	pairFlags |= PxPairFlag::eCONTACT_DEFAULT;
	pairFlags |= PxPairFlag::eNOTIFY_CONTACT_POINTS;
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
	{
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
	}
	else
	{
		return PxFilterFlag::eSUPPRESS;
	}
	return PxFilterFlags();
}
static PxFilterFlags SampleBridgesFilterShader(	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
												PxFilterObjectAttributes attributes1, PxFilterData filterData1,
												PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	return PxFilterFlags();
}




PhysXMgr::PhysXMgr()
: m_pDefaultErrorCallback(nullptr)
,m_pDefaultAllocatorCallback(nullptr)
,m_pDispatcher(nullptr)
,m_scale()
,m_pPvd(nullptr)
,m_pTransport(nullptr)
,m_pPhysics(nullptr)
,m_pFoundation(nullptr)
,m_pCooking(nullptr)
,m_pScene(nullptr)
,m_pEventCallback(nullptr)
,m_pRaycastMgr(nullptr)
,m_pCudaContextMgr(nullptr)
//,m_pMyPxUserControllerHitReport(nullptr)
//,m_pMyPxControllerBehaviorCallback(nullptr)
{
	//m_pPhysEngine = new PhysEngine;


}

PhysXMgr::~PhysXMgr()
{
	// gpu 쓸때 중단점 걸림
	// 이유를 모르겠음
	// m_pCudaContextMgr 이거 생성하고 씬에 넣고 나서 생겨서
	// 나중에 문제 생기면 해결
	PX_RELEASE(m_pControllerMgr);
	PX_RELEASE(m_pScene);
	//PX_RELEASE(m_pCudaContextMgr);
	PX_RELEASE(m_pDispatcher);
	PX_RELEASE(m_pPhysics);
	PX_RELEASE(m_pCooking);
	// 디버거 릴리즈
	if (m_pPvd)
	{
		
		m_pPvd->release();
		m_pPvd = nullptr;
		m_pTransport->release();
	}
	
	PX_RELEASE(m_pFoundation);
	
	delete m_pDefaultErrorCallback;
	delete m_pDefaultAllocatorCallback;
	delete m_pEventCallback;
	delete m_pRaycastMgr;
}




void PhysXMgr::init()
{
	Create_Start();

#ifdef _DEBUG
	init_Debug();
#else
	init_Relese();
#endif
}

void PhysXMgr::tick()
{
	m_pScene->setVisualizationParameter(PxVisualizationParameter::eCONTACT_POINT, 1.0f);
	m_pScene->setVisualizationParameter(PxVisualizationParameter::eCONTACT_NORMAL, 1.0f);

	m_pScene->simulate(1 / 60.f);
	m_pScene->fetchResults(true);
}

void PhysXMgr::init_Debug()
{
	Create_Foundation();
	Create_Pvd();
	Create_Physics();
	Create_Cooking();
	Create_Scene();
	// Create_BroadPhaseRegion(100);
	Create_ControllerMgr();
	Create_Plane();
}
void PhysXMgr::init_Relese()
{
	// Foundation 만들기
	Create_Foundation();
	// 본체 설정 및 다양한 충돌체 설정하는 객체 생성
	Create_Physics();
	Create_Cooking();
	Create_Scene();
	Create_ControllerMgr();
}


void PhysXMgr::Create_Start()
{
	// 동적할당
	m_pDefaultErrorCallback = new UserErrorCallback;
	m_pDefaultAllocatorCallback = new PxDefaultAllocator;
	m_pRaycastMgr = new RaycastMgr;
	m_pEventCallback = new EventCallBack();
	//m_pMyPxUserControllerHitReport = new MyPxUserControllerHitReport;
	//m_pMyPxControllerBehaviorCallback = new MyPxControllerBehaviorCallback;
}
void PhysXMgr::Create_Scene()
{
	// 장면 생성
	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	m_pDispatcher = PxDefaultCpuDispatcherCreate(4);
	
	// 장면 생성
	sceneDesc.gravity					= PxVec3(0.0f, -9.81f, 0.0f);
	sceneDesc.cpuDispatcher				= m_pDispatcher;
	sceneDesc.simulationEventCallback	= m_pEventCallback; // 이벤트 어디다가 들여오는지 정해줌
	sceneDesc.filterShader = SampleSubmarineFilterShader;
	m_pScene = m_pPhysics->createScene(sceneDesc);

	// 디버그 설정
	PxPvdSceneClient* pvdClient = m_pScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
}

void PhysXMgr::Create_Foundation()
{
	// Foundation 만들기
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *m_pDefaultAllocatorCallback, *m_pDefaultErrorCallback);
	VAILD(m_pFoundation);
}

void PhysXMgr::Create_Pvd()
{
	// 디버그 만들기
	// 연결
	// https://developer.nvidia.com/physx-visual-debugger
	// physx visual Debugger와 연결
	m_pPvd = PxCreatePvd(*m_pFoundation);
	VAILD(m_pPvd);
	m_pTransport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_pPvd->connect(*m_pTransport, PxPvdInstrumentationFlag::eALL);
}

void PhysXMgr::Create_BroadPhaseRegion(int _size)
{
	float size = _size;
	//Phys Engine 충돌될 범위를 지정해준다
	// 이 영역 안에 있는 곳은 충돌 판정 o
	PxBroadPhaseRegion regions[4] =
	{
		{	PxBounds3(PxVec3(-size, -size, -size),  PxVec3(0, size,   0)),	reinterpret_cast<void*>(1) },
		{	PxBounds3(PxVec3(-size, -size,    0),  PxVec3(0, size, size)),	reinterpret_cast<void*>(2) },
		{	PxBounds3(PxVec3(0, -size, -size),	PxVec3(size, size,   0)),	reinterpret_cast<void*>(3) },
		{	PxBounds3(PxVec3(0, -size,    0),	PxVec3(size, size, size)),	reinterpret_cast<void*>(4) }
	};
	for (PxU32 i = 0; i < 4; i++)
	{
		m_pScene->addBroadPhaseRegion(regions[i]);
	}
}

void PhysXMgr::Create_Physics()
{
	// 본체 설정 및 다양한 충돌체 설정하는 객체 생성
	m_scale.length = 100;        // typical length of an object
	m_scale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice
	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, m_scale, true, m_pPvd);
	VAILD(m_pPhysics);
}

void PhysXMgr::Create_ControllerMgr()
{
	m_pControllerMgr = PxCreateControllerManager(*m_pScene);
}

void PhysXMgr::Create_Cooking()
{
	PxCookingParams params(m_scale);
	params.meshWeldTolerance = 0.001f;
	params.meshPreprocessParams = PxMeshPreprocessingFlags(PxMeshPreprocessingFlag::eWELD_VERTICES);
	params.buildGPUData = true; //Enable GRB data being produced in cooking.
	// 각종 정점만들 때 쓰는 객체
	m_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale());
	VAILD(m_pCooking);

	//PxTolerancesScale scale;
	//PxCookingParams params(scale);
	//// disable mesh cleaning - perform mesh validation on development configurations
	//params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
	//// disable edge precompute, edges are set for each triangle, slows contact generation
	//params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
	//m_pCooking->setParams(params);
}

void PhysXMgr::Create_Plane()
{
	// 만들기 샘플
	PxMaterial* gMaterial;
	gMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	PxBroadPhaseCallback;
	PxRigidStatic* groundPlane = PxCreatePlane(*m_pPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
	groundPlane->setName("hanjoo");
	m_pScene->addActor(*groundPlane);
	m_pRaycastMgr->Initialize(m_pScene);
}

void PhysXMgr::addActor(PxActor* _actor ,bool _isDynamic)
{
	m_pScene->addActor(*_actor);
	if (_isDynamic)
	{
		PxRigidDynamic* Dynamic = reinterpret_cast<PxRigidDynamic*>(_actor);
		PxShape* shape;
		Dynamic->getShapes(&shape,1);
		m_pRaycastMgr->registerRaycastCCDObject(Dynamic, shape);
	}	
}



void PhysXMgr::DelActor(PxActor* _actor)
{
	m_pRaycastMgr->removeActor(_actor);
}

bool PhysXMgr::Raycast(CRayCast* _ray)
{
	return m_pRaycastMgr->RayCastMultiple(_ray);
}

