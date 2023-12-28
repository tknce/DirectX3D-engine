#include "pch.h"
#include "CRigidBody3D.h"

#include "PhysXMgr.h"
#include "CTimeMgr.h"

#include "CRayCast.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CLandScape.h"


#include "CKeyMgr.h"

#include "CPlayerController.h"

CRigidBody3D::CRigidBody3D()
	: CComponent(COMPONENT_TYPE::RIGIDBODY3D)
	//,m_pPhysData(nullptr)
	, m_bEnter(false)
	, m_bisDinamic(true)
	, m_bisgravity(false)
	, m_vForce(Vec3(0, 0, 0))
	, m_vVelocity(Vec3(0, 0, 0))
	, m_fMass(5.f)
	, m_pController(nullptr)
{
	m_pMaterial = PhysXMgr::GetInst()->GetPhys()->createMaterial(0.5f, 0.5f, 0.6f);
	m_pController = nullptr;
}

CRigidBody3D::CRigidBody3D(const CRigidBody3D& _origin)
	:CComponent(COMPONENT_TYPE::RIGIDBODY3D)
	, m_bisDinamic(_origin.m_bisDinamic)
	, m_bisgravity(_origin.m_bisgravity)
	, m_bEnter(_origin.m_bEnter)
	, m_isPosition(_origin.m_isPosition)
	, m_isForce(_origin.m_isForce)
	, m_isVelocity(_origin.m_isVelocity)
	, m_vForce(_origin.m_vForce)
	, m_vVelocity(_origin.m_vVelocity)
	, m_vRot(_origin.m_vRot)
	, m_vFreezePos(_origin.m_vFreezePos)
	, m_vFreezeRot(_origin.m_vFreezeRot)
	, m_fMass(_origin.m_fMass)
{
	if (_origin.m_pController != nullptr)
		CreatePlayerController();
	else
		Create();
}

CRigidBody3D::~CRigidBody3D()
{
	//m_pMaterial->release();
	delete m_pRayCast;
	delete m_pController;
	// PX_RELEASE(m_pCapsuleController);
}

void CRigidBody3D::finaltick()
{
	if (KEY_TAP(KEY::M))
	{
		m_bisDinamic = false;
		Create();
	}
	tickActor();

	if (PhysXMgr::GetInst()->Raycast(m_pRayCast))
	{
		int a = 0;
	}
}


void CRigidBody3D::Create()
{
	Delete();
	SettingColliderType();
	m_pRayCast = new CRayCast;
	//shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
	//shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	//shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	PxTransform trans = PxTransform();
	///물리 객체 생성
	if (m_bisDinamic)
		Create_Dynamic(CreateShape(), &trans);
	else
		Create_Static(CreateShape(), &trans);

	///생성이 되었다면 사용한 물리객체를 삭제해준다

}
// 예시
//PxRigidDynamic* createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxReal& mass, const PxVec3& velocity = PxVec3(0))
//{
//	PxRigidDynamic* dynamic = PxCreateDynamic(*gPhysics, t, geometry, *gMaterial, 10.0f);
//	dynamic->setAngularDamping(0.5f);
//	dynamic->setLinearVelocity(velocity);
//	PxRigidBodyExt::setMassAndUpdateInertia(*dynamic, mass);
//	gScene->addActor(*dynamic);
//	return dynamic;
//}

void CRigidBody3D::Create_Dynamic(PxShape* shape, PxTransform* Tr)
{
	if (!m_bisDinamic) return;
	PxVec3 Pos = PxVec3(Transform()->GetRelativePos().x, Transform()->GetRelativePos().y, Transform()->GetRelativePos().z);
	PxTransform tr = PxTransform(Pos, PxQuat(0, 0, 0, 1));
	//tr.q.rotate(PxVec3(3.141529 / 4, 0, 0));
	///물리 객체 생성
	PxRigidDynamic* body = PhysXMgr::GetInst()->GetPhys()->createRigidDynamic(tr);
	
	shape->getFlags();

	//const PxFilterData triggerFilterData(0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff);
	//shape->setSimulationFilterData(triggerFilterData);
	//shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	// shape->setFlag(PxShapeFlag::eVISUALIZATION, true); //Ray, sweep등 할때 사용됨
	//shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	//shape->setSimulationFilterData(filterDataGroup1);
	body->attachShape(*shape);
	// 무게설정칸 임의로 설정 일단 테스트 코드라 5
	PxRigidBodyExt::updateMassAndInertia(*body, m_fMass);
	
	// 왜 둘다 false인데 떨어지는거지?
	// body->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	// body->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_POSE_INTEGRATION_PREVIEW, true);
	// body->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);

	//서로 연결
	body->userData = GetOwner();
	ActorObj = body;

	string name;
	name.assign(GetOwner()->GetName().begin(), GetOwner()->GetName().end());
	body->setName(name.c_str());
	
	//setupFiltering(body, FilterGroup::eCRAB, FilterGroup::eHEIGHTFIELD);
	//setupFiltering(body, FilterGroup::eSUBMARINE, FilterGroup::eMINE_HEAD |
	//	FilterGroup::eMINE_LINK);
	//setupFiltering(body, FilterGroup::eMINE_LINK, FilterGroup::eSUBMARINE);
	//setupFiltering(body, FilterGroup::eMINE_HEAD, FilterGroup::eSUBMARINE);
	//setupFiltering(body, FilterGroup::eHEIGHTFIELD, FilterGroup::eCRAB);
	
	//setupFiltering(m_pController->GetActor(), FilterGroup::eMINE_HEAD, FilterGroup::eCRAB);

	/*PxFilterData filterData;
	filterData.word0 = FilterGroup::eSPHERE;
	filterData.word1 = FilterGroup::ePLAYER;*/
	body->setGlobalPose(tr);
	PhysXMgr::GetInst()->addActor(body, true);
	setupFiltering(body, FilterGroup::eSPHERE, FilterGroup::ePLAYER);

}


void CRigidBody3D::Create_Static(PxShape* shape, PxTransform* Tr)
{
	PxVec3 Pos = PxVec3(Transform()->GetRelativePos().x, Transform()->GetRelativePos().y, Transform()->GetRelativePos().z);
	PxTransform tr = PxTransform(Pos, PxQuat(0, 0, 0, 1));
	PxRigidStatic* body = PhysXMgr::GetInst()->GetPhys()->createRigidStatic(tr);

	body->attachShape(*shape);

	//서로 연결
	body->userData = GetOwner();
	ActorObj = body;

	string name;
	name.assign(GetOwner()->GetName().begin(), GetOwner()->GetName().end());
	body->setName(name.c_str());

	PhysXMgr::GetInst()->addActor(body, false);
}

void CRigidBody3D::begin()
{
	if (ActorObj == nullptr)
	{
		m_bisDinamic = true;
		Create();
	}
}
#include <cmath>

void CRigidBody3D::tickActor()
{
	if (ActorObj == nullptr) { return; }
	if (!m_bisDinamic) { return; }
	if (LandScape() != nullptr) { return; }

	//if (KEY_TAP(KEY::W))
	//{
	//	SetVelocity(Vec3(100.f, 0.f, 100.f));
	//
	//}
	/*if (KEY_PRESSED(KEY::W))
	{
		if (m_pController != nullptr)
		{
			m_pController->GetController()->move(PxVec3(1.f, 0.f, 1.f), 0.01f, CTimeMgr::GetInst()->GetDeltaTime(), PxControllerFilters());
		}
	}*/
	if (m_pController != nullptr)
	m_pController->GetController()->move(PxVec3(0.f, -9.8f, 0.f), 0.01f, CTimeMgr::GetInst()->GetDeltaTime(), PxControllerFilters());
	PxRigidDynamic* Dynamic = reinterpret_cast<PxRigidDynamic*>(ActorObj);
	if (Dynamic == nullptr) { return; }

	if (m_isForce == true) { tickDynamicForce(Dynamic); }
	if (m_isVelocity == true) { tickDynamicVelocity(Dynamic); }
	if (m_isPosition == true) { tickDynamicPosition(Dynamic); }
	tickPosnRot(Dynamic);

	if (Dynamic->isSleeping())
		Dynamic->wakeUp();
}

void CRigidBody3D::tickDynamicPosition(PxRigidDynamic* _Dynamic)
{
	if (ActorObj == nullptr) { return; }
	//위치값이 변경되었다면 변경된 값으로 업데이트
	PxTransform p = _Dynamic->getGlobalPose();
	Vec3 Pos = Transform()->GetRelativePos();
	Vec3 Rot = Transform()->GetRelativeRotation();
	PxVec3 P = PxVec3(Pos.x, Pos.y, -Pos.z);
	p.p = P;

	Quaternion a;
	Matrix b = Transform()->GetWorldMat();
	a = a.CreateFromRotationMatrix(Transform()->GetWorldMat());
	// a = a.CreateFromYawPitchRoll(-Rot.z, Rot.y, Rot.x);
	p = PxTransform(p.p, PxQuat(a.x,a.y, a.z, a.w));

	m_isPosition = false;
	
	_Dynamic->setGlobalPose(p);

	// Create();
}

void CRigidBody3D::tickDynamicForce(PxRigidDynamic* _Dynamic)
{
	if (ActorObj == nullptr) { return; }

	Vec3 Pos = Transform()->GetRelativePos();
	//포스값이 변경되었다면 변경된 값으로 업데이트
	PxVec3 Force = PxVec3(m_vForce.x, m_vForce.y, m_vForce.z);
	PxVec3 Pos1 = PxVec3(Pos.x, Pos.y, Pos.z);
	PxRigidBodyExt::addForceAtPos(*_Dynamic, Force, Pos1);
	m_isForce = false;
}

void CRigidBody3D::tickDynamicVelocity(PxRigidDynamic* _Dynamic)
{
	if (ActorObj == nullptr) { return; }

	//속력값이 변경되었다면 변경된 값으로 업데이트
	PxVec3 Velocity = _Dynamic->getLinearVelocity();
	PxVec3 Pox;
	if (m_vVelocity.y == 0.0f)
	{
		Pox = PxVec3(m_vVelocity.x, m_vVelocity.y, m_vVelocity.z);
	}
	else
	{
		Pox = PxVec3(m_vVelocity.x, Velocity.y, m_vVelocity.z);
	}
	//PxVec3 Pox = PxVec3(data->Velocity.x, data->Velocity.y, data->Velocity.z);
	_Dynamic->setLinearVelocity(Pox);
	m_isVelocity = false;
}

void CRigidBody3D::tickPosnRot(PxRigidDynamic* Dynamic)
{
	// 왼손좌표계 오른손좌표계 위치변환?
	//받아온 Data를 Actor와 동기화시켜준다
	PxTransform Tr = Dynamic->getGlobalPose();
	// 위치 동기화
	Vec3 Pos = Vec3(Tr.p.x, Tr.p.y, -Tr.p.z);
	Transform()->SetRelativePos(Pos);
	// 매트릭스로 전환
	// 퀀터니엄 구해주기
	Quaternion qu = Quaternion(Tr.q.x, Tr.q.y, Tr.q.z, Tr.q.w);
	Matrix mat = XMMatrixRotationQuaternion(XMVectorSet(-qu.x, -qu.y, qu.z, qu.w));
	PxVec3 Rot;
	float a = 0;
	Tr.q.toRadiansAndUnitAxis(a,Rot);

	// 회전매트릭스 연산하러 보내기
	Transform()->SetMatrigidRot(mat);
	// 회전값 동기화
	float pitch, roll, yaw;
	// pitch 계산
	pitch = asin(-mat._23);

	// roll 계산
	if (cos(pitch) != 0) {
		roll = atan2(mat._13, mat._33);
	}
	else {
		roll = 0.0;
	}
	// yaw 계산
	if (cos(pitch) != 0) {
		yaw = atan2(mat._21, mat._22);
	}
	else {
		yaw = 0.0;
	}
	if(m_pController == nullptr)
	Transform()->SetRelativeRotation(Rot.x, Rot.y, -Rot.z);
}

void CRigidBody3D::Delete()
{
	//한개의 엑터 삭제
	if (ActorObj != nullptr)
	{
		PxActor* rig = reinterpret_cast<PxActor*>(ActorObj);
		// PhysXMgr::GetInst()->DelActor(rig);
		PhysXMgr::GetInst()->GetScene()->removeActor(*rig);

		rig->release();
		rig = nullptr;

		delete m_pRayCast;
		m_pRayCast = nullptr;
	}
	ActorObj = nullptr;
}

void CRigidBody3D::CreateHeightMap()
{
	int offset = 1;
	// 높이맵 텍스쳐 가로세로 길이
	uint32_t width = GetOwner()->LandScape()->GetHeightTex()->GetWidth();
	uint32_t height = GetOwner()->LandScape()->GetHeightTex()->GetHeight();

	// 높이맵 텍스쳐가 컴퓨트쉐이더에서 수정되기 때문에 높이맵 수정	
	// 디버그 단계에서만 수정되기 때문에 릴리즈 되면 한 번만 함수 사용
	static vector<vector<float>> heightData;
	GetOwner()->LandScape()->GetHeightTex()->GetPixelVector_float(heightData);

	// Compute the minimum and maximum heights from the height data
	// max min 충돌나서 메크로 해제
#undef max
	float minHeight = std::numeric_limits<float>::max(); // 최소치를 설정
	float maxHeight = -std::numeric_limits<float>::max();
	for (uint32_t col = 0; col < width; col += offset)
	{
		for (int row = 0; row < height; row += offset)
		{
			float height = heightData[col][row];
			minHeight = min(minHeight, height);
			maxHeight = std::max(maxHeight, height);
		}
	}



	// Create the PhysX heightfield samples data
	// 기반으로 만들기
	physx::PxHeightFieldSample* samplesData = new physx::PxHeightFieldSample[(width * height) / offset];
	for (uint32_t row = 0; row < height / offset; row++)
	{

		for (uint32_t col = 0; col < width / offset; col++)
		{
			uint32_t index = row * width + col;
			float height = heightData[row * offset][col * offset];

			// Set the height value in the sample data
			samplesData[index].height = static_cast<int16_t>((height - minHeight) / (maxHeight - minHeight) * std::numeric_limits<int16_t>::max()) / 10;
			// samplesData[index].height = height;
			// Set the material index and tessellation flag if needed
			samplesData[index].materialIndex0 = 1;	// Set the material index for the sample
			samplesData[index].materialIndex1 = 1;	// Set the second material index if using blended materials
			samplesData[index].setTessFlag();			// Set the tessellation flag if using adaptive tessellation
		}
	}

	// Create the PhysX heightfield descriptor
	physx::PxHeightFieldDesc heightFieldDesc;
	heightFieldDesc.format = physx::PxHeightFieldFormat::eS16_TM;
	heightFieldDesc.nbColumns = width / offset;
	heightFieldDesc.nbRows = height / offset;
	heightFieldDesc.samples.data = samplesData;
	heightFieldDesc.samples.stride = sizeof(physx::PxHeightFieldSample);

	// Create the PhysX heightfield using the descriptor
	PxHeightField* heightField = PhysXMgr::GetInst()->GetCooking()->createHeightField(heightFieldDesc, PhysXMgr::GetInst()->GetPhys()->getPhysicsInsertionCallback());
	if (!heightField)
	{
		// Handle error: Heightfield creation failed
		delete[] samplesData;
		return;
	}

	// Compute the heightfield scale and vertical extent
	float heightScale = (maxHeight - minHeight) / std::numeric_limits<int16_t>::max();
	float verticalExtent = maxHeight;


	// ... Continue with the code to create the heightfield shape, actor, and add it to the scene
	PxHeightFieldGeometry hfGeom(heightField, PxMeshGeometryFlags(), 1.f, 1.f, 1.f);


	PxShape* heightFieldShape = PhysXMgr::GetInst()->GetPhys()->createShape(hfGeom, *m_pMaterial);
	if (!heightFieldShape)
	{
		return;
	}
	// Set the eSIMULATION_SHAPE flag to false

	// 플래그 설정
	PxShapeFlags shapeFlags = PxShapeFlag::eVISUALIZATION // 디버그 랜더링 플래그
		| PxShapeFlag::eSIMULATION_SHAPE;
	// 충돌할 때 트리거를 보낼지에 대한 플래그 - 렉오지게 걸림

	heightFieldShape->setFlags(shapeFlags);


	PxVec3 Pos = PxVec3(Transform()->GetRelativePos().x, Transform()->GetRelativePos().y, Transform()->GetRelativePos().z);
	PxQuat Rot = PxQuat(Transform()->GetRelativeRotation().x, Transform()->GetRelativeRotation().y, Transform()->GetRelativeRotation().z, 1);
	PxTransform  mTransform = PxTransform(Pos, Rot);
	heightFieldShape->setLocalPose(mTransform);

	// Create a rigid body for the heightfield
	// 정적으로 설정
	physx::PxRigidActor* heightFieldActor = PhysXMgr::GetInst()->GetPhys()->createRigidStatic(mTransform);
	if (!heightFieldActor)
	{
		return;
	}
	/*PxFilterData filterData;
	filterData.word0 = 0;
	heightFieldShape->setSimulationFilterData(filterData);*/

	heightFieldActor->attachShape(*heightFieldShape);

	// 서로 연결
	heightFieldActor->userData = GetOwner();
	ActorObj = heightFieldActor;
	// Add the heightfield actor to the scene
	PhysXMgr::GetInst()->GetScene()->addActor(*heightFieldActor);

	// Don't forget to release the samples data when you're done with it
	delete[] samplesData;

}

void CRigidBody3D::CreateHeightMap1()
{
	//PxReal heightScale = 0.001f;

	float offset = 1.f;
	// 높이맵 텍스쳐 가로세로 길이
	uint32_t width = GetOwner()->LandScape()->GetHeightTex()->GetWidth();
	uint32_t height = GetOwner()->LandScape()->GetHeightTex()->GetHeight();
	PxReal hfScale = width / offset;
	// 높이맵 텍스쳐가 컴퓨트쉐이더에서 수정되기 때문에 높이맵 수정	
	// 디버그 단계에서만 수정되기 때문에 릴리즈 되면 한 번만 함수 사용
	static vector<vector<float>> heightData;
	GetOwner()->LandScape()->GetHeightTex()->GetPixelVector_float(heightData);

	// Compute the minimum and maximum heights from the height data
	// max min 충돌나서 메크로 해제
#undef max
	float minHeight = std::numeric_limits<float>::max(); // 최소치를 설정
	float maxHeight = -std::numeric_limits<float>::max();
	for (uint32_t col = 0; col < width; col += offset)
	{
		for (int row = 0; row < height; row += offset)
		{
			float height = heightData[col][row];
			minHeight = min(minHeight, height);
			maxHeight = std::max(maxHeight, height);
		}
	}


	PxU32* hfSamples = new PxU32[width * height / offset];
	PxReal deltaHeight = maxHeight - minHeight;
	PxReal heightScale = PxMax(deltaHeight / std::numeric_limits<int16_t>::max(), PX_MIN_HEIGHTFIELD_Y_SCALE);
	PxReal quantization = (PxReal)0x7fff;
	// Create the PhysX heightfield samples data
	// 기반으로 만들기
	for (uint32_t row = 0; row < height / offset; row++)
	{

		for (uint32_t col = 0; col < width / offset; col++)
		{
			PxI16 height = PxI16(quantization * (heightData[row * offset][col * offset] - minHeight) /
				deltaHeight);
			uint32_t index = row * width + col;
			//float height = heightData[row * offset][col * offset];
			PxHeightFieldSample& smp = (PxHeightFieldSample&)(hfSamples[(row * width) + col]);
			// Set the height value in the sample data
			//smp.height = (PxI16)((hfSamples[(row * width) + col] - minHeight) / (maxHeight - minHeight) * std::numeric_limits<int16_t>::max()) * 10;
			smp.height = (PxI16)(heightData[row * offset][col * offset] / 0.001f);
			//smp.height = height;
			// Set the material index and tessellation flag if needed
			smp.materialIndex0 = 1;	// Set the material index for the sample
			smp.materialIndex1 = 1;	// Set the second material index if using blended materials

			smp.setTessFlag();			// Set the tessellation flag if using adaptive tessellation
		}
	}

	// Create the PhysX heightfield descriptor
	physx::PxHeightFieldDesc heightFieldDesc;
	heightFieldDesc.format = physx::PxHeightFieldFormat::eS16_TM;
	heightFieldDesc.nbColumns = width / offset;
	heightFieldDesc.nbRows = height / offset;
	heightFieldDesc.samples.data = hfSamples;
	heightFieldDesc.samples.stride = sizeof(physx::PxHeightFieldSample);

	//PxHeightFieldGeometry hfGeom;
	//hfGeom.columnScale = width / (width - 1); // compute column and row scale from input terrain
	//											// height grid
	//hfGeom.rowScale = width / (width - 1);
	//hfGeom.heightScale = deltaHeight != 0.0f ? heightScale : 1.0f;
	//hfGeom.heightField = PhysXMgr::GetInst()->GetCooking()->createHeightField(heightFieldDesc, PhysXMgr::GetInst()->GetPhys()->getPhysicsInsertionCallback());
	PxHeightField* heightField = PhysXMgr::GetInst()->GetCooking()->createHeightField(heightFieldDesc, PhysXMgr::GetInst()->GetPhys()->getPhysicsInsertionCallback());
	Vec3 vscale = Transform()->GetRelativeScale();
	PxHeightFieldGeometry hfGeom(heightField, PxMeshGeometryFlags(), 1, 1, 0.1f);

	// Create the PhysX heightfield using the descriptor
	//PxHeightField* heightField = PhysXMgr::GetInst()->GetCooking()->createHeightField(heightFieldDesc, PhysXMgr::GetInst()->GetPhys()->getPhysicsInsertionCallback());
	//if (!heightField)
	//{
	//	// Handle error: Heightfield creation failed
	//	delete[] samplesData;
	//	return;
	//}

	delete[] hfSamples;

	m_pMaterial = PhysXMgr::GetInst()->GetPhys()->createMaterial(0.5f, 0.5f, 0.2f);
	PxVec3 Pos = PxVec3(Transform()->GetRelativePos().x, Transform()->GetRelativePos().y, Transform()->GetRelativePos().z);
	PxQuat Rot = PxQuat(Transform()->GetRelativeRotation().x, Transform()->GetRelativeRotation().y, Transform()->GetRelativeRotation().z, 1);
	PxTransform  mTransform = PxTransform(Pos, Rot);
	physx::PxRigidActor* heightFieldActor = PhysXMgr::GetInst()->GetPhys()->createRigidStatic(mTransform);

	// 플래그 설정
	PxShapeFlags shapeFlags = PxShapeFlag::eVISUALIZATION // 디버그 랜더링 플래그
		| PxShapeFlag::eSIMULATION_SHAPE;

	PxShape* hfShape = PxRigidActorExt::createExclusiveShape(*heightFieldActor, hfGeom, *m_pMaterial);

	// Set the eSIMULATION_SHAPE flag to false
	// 충돌할 때 트리거를 보낼지에 대한 플래그 - 렉오지게 걸림

	hfShape->setFlags(shapeFlags);
	hfShape->setLocalPose(mTransform);

	// Create a rigid body for the heightfield
	// 정적으로 설정

	if (!heightFieldActor)
	{
		return;
	}
	/*PxFilterData filterData;
	filterData.word0 = 0;
	heightFieldShape->setSimulationFilterData(filterData);*/

	//heightFieldActor->attachShape(*heightFieldShape);

	// 서로 연결
	heightFieldActor->userData = GetOwner();
	
	ActorObj = heightFieldActor;
	// Add the heightfield actor to the scene
	PhysXMgr::GetInst()->GetScene()->addActor(*heightFieldActor);

}

void CRigidBody3D::CreateConvex()
{
	// pxconvexmesh 생성
	if (GetOwner()->MeshRender() == nullptr) return;

	UINT VtxCount = GetOwner()->MeshRender()->GetMesh()->GetVtxCount();

	vector<Vtx> vertex;
	vector<PxVec3> sample;
	vertex = GetOwner()->MeshRender()->GetMesh()->GetVtx();
	for (int i = 0; i < vertex.size(); ++i)
	{
		sample.push_back(Vec3toPxVec3(vertex[i].vPos, GetOwner()->Transform()->GetRelativeScale()));
	}

	// 정점 생성 샘플
	//float scale = 100;
	//static const PxVec3 convexVerts[] = { PxVec3(0,scale,0),PxVec3(scale,0,0),PxVec3(-scale,0,0),PxVec3(0,0,scale),
	//PxVec3(0,0,-scale) };

	// convexmesh에 대한 레이아웃데이터
	// points 는 정점에 대한 정보이다 어느 위치에 있는지 서술하면 된다.
	PxConvexMeshDesc convexDesc;
	convexDesc.points.count = VtxCount;
	convexDesc.points.stride = sizeof(PxVec3);
	convexDesc.points.data = sample.data();
	convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
	// PxConvexFlag::e16_BIT_INDICES; // PxConvexMeshDesc 에서 triangles 또는 polygons에서 16비트 정점인덱스 사용을 나타냄
	// PxConvexFlag::eCHECK_ZERO_AREA_TRIANGLES; // 볼록 외피 계산 중에 거의 영역이 0인 삼각형을 확인하고 제거합니다. eCOMPUTE_CONVEX이거랑 조합해서만 사용됌
	// PxConvexFlag::eCOMPUTE_CONVEX; // 정점에서 hull을 자동으로 다시 계산 이것이 설정되어 있지 않다면 지오메트리에서 수동으로 계산해야됩니다.
	// PxConvexFlag::eDISABLE_MESH_VALIDATION; // 볼록 메시 유효성 검사를 비활성화하여 헐 생성 속도를 높입니다. 확인/디버그 빌드에서 별도의 유효성 검사 기능을 사용하십시오. 
	// 								//사전 검증 없이 유효하지 않은 입력 데이터로 볼록 메시를 생성하면 정의되지 않은 동작이 발생할 수 있습니다.
	// PxConvexFlag::eFAST_INERTIA_COMPUTATION; // 빠르지만 정밀도가 낮아 사용x
	// PxConvexFlag::eGPU_COMPATIBLE; // gpu호환 가능
	// PxConvexFlag::ePLANE_SHIFTING; // 뭔가 최적화 관련 플래그
	// PxConvexFlag::eQUANTIZE_INPUT; // k-평균 알고리즘을 통해 정점배치를 함

	// cooking과 physic를 통한 객체 생성
	PxDefaultMemoryOutputStream buf;
	PxConvexMeshCookingResult::Enum result;
	if (!PhysXMgr::GetInst()->GetCooking()->cookConvexMesh(convexDesc, buf, &result))
		return;
	PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
	PxConvexMesh* convexMesh = PhysXMgr::GetInst()->GetPhys()->createConvexMesh(input);

	PxTransform  mTransform = GetCurTransform();
	PxRigidDynamic* aConvexActor = PhysXMgr::GetInst()->GetPhys()->createRigidDynamic(mTransform);


	PxShapeFlags shapeFlags;


	//shapeFlags = PxShapeFlag::eVISUALIZATION // 디버그 랜더링 플래그
	//// | PxShapeFlag::eTRIGGER_SHAPE // 충돌할 때 트리거를 보낼지에 대한 플래그
	//	| PxShapeFlag::eSIMULATION_SHAPE
	//	// PxShapeFlag::eSIMULATION_SHAPE 물리 시뮬레이션에 참여하는지에 대한 플래그 * eTRIGGER_SHAPE 이거랑 같이 못 씀
	//	| PxShapeFlag::eSCENE_QUERY_SHAPE;// ray casts, overlap tests, sweeps 등에 참여합니다.정확히는 모르겠지만 피직스가 메인렌더링이 될 때 설정하는듯
	// 플래그 설정
	shapeFlags = PxShapeFlag::eVISUALIZATION // 디버그 랜더링 플래그
		| PxShapeFlag::eSIMULATION_SHAPE; // 충돌할 때 트리거를 보낼지에 대한 플래그

	PxShape* aConvexShape = PxRigidActorExt::createExclusiveShape(*aConvexActor,
		PxConvexMeshGeometry(convexMesh), *m_pMaterial, shapeFlags);

	aConvexActor->userData = GetOwner();
	ActorObj = aConvexActor;

	aConvexActor->attachShape(*aConvexShape);
	PhysXMgr::GetInst()->GetScene()->addActor(*aConvexActor);
	setupFiltering(aConvexActor, FilterGroup::eSPHERE, FilterGroup::ePLAYER);
}

void CRigidBody3D::CreateTriangle()
{

	// pxconvexmesh 생성
	if (GetOwner()->MeshRender() == nullptr) return;

	vector<PxVec3> sample;
	vector<Triangle> tri;
	GetVertexData(sample, tri);


	// convexmesh에 대한 레이아웃데이터
	// points 는 정점에 대한 정보이다 어느 위치에 있는지 서술하면 된다.
	PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = sample.size();
	meshDesc.points.stride = sizeof(PxVec3);
	meshDesc.points.data = sample.data();

	// triangles은 인덱스를 설명하는 것이다. 생성한 정점들이 어디와 어디를 조합해야되는지 알려주는 것이다.
	meshDesc.triangles.count = tri.size();
	meshDesc.triangles.stride = sizeof(Triangle);
	meshDesc.triangles.data = tri.data();


	bool res = PhysXMgr::GetInst()->GetCooking()->validateTriangleMesh(meshDesc);
	PX_ASSERT(res);

	// cooking과 physic를 통한 객체 생성
	PxDefaultMemoryOutputStream writeBuffer;
	PxTriangleMeshCookingResult::Enum result;
	bool status = PhysXMgr::GetInst()->GetCooking()->cookTriangleMesh(meshDesc, writeBuffer, &result);
	if (!status)
		int a = 0;
	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	PxTriangleMesh* trianglemesh = PhysXMgr::GetInst()->GetPhys()->createTriangleMesh(readBuffer);
	//PxTriangleMesh* trianglemesh = PhysXMgr::GetInst()->GetCooking()->createTriangleMesh(meshDesc,PhysXMgr::GetInst()->GetPhys()->getPhysicsInsertionCallback());

	// 모양 만들기

	PxTransform  mTransform = GetCurTransform();

	PxRigidDynamic* aConvexActor = PhysXMgr::GetInst()->GetPhys()->createRigidDynamic(mTransform);
	aConvexActor->setMass(10.f);
	aConvexActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	//aConvexActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
	//aConvexActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);


	PxVec3 scale = Vec3toPxVec3(Transform()->GetRelativeScale());
	PxShape* shape = PxRigidActorExt::createExclusiveShape(*aConvexActor,
		PxTriangleMeshGeometry(trianglemesh, PxMeshScale(scale)),
		*m_pMaterial);
	//// 플래그 설정
	PxShapeFlags shapeFlags = PxShapeFlag::eVISUALIZATION // 디버그 랜더링 플래그
		| PxShapeFlag::eTRIGGER_SHAPE; // 충돌할 때 트리거를 보낼지에 대한 플래그
	shape->setFlags(shapeFlags);

	if (!shape)
		assert(shape);
	aConvexActor->userData = GetOwner();
	ActorObj = aConvexActor;

	aConvexActor->attachShape(*shape);
	PhysXMgr::GetInst()->GetScene()->addActor(*aConvexActor);
	m_bisDinamic = false;
}

void CRigidBody3D::CreatePlayerController(Vec3 _vec)
{
	PxExtendedVec3 Pos;
	if (_vec != Vec3())
	{
		Pos = PxExtendedVec3(_vec.x, _vec.y + 500, _vec.z);
	}
	else
	Pos = PxExtendedVec3(Transform()->GetRelativePos().x, Transform()->GetRelativePos().y + 500, Transform()->GetRelativePos().z);
	PxCapsuleControllerDesc* desc = new PxCapsuleControllerDesc;
	desc->setToDefault();
	desc->height = 250.f; // 임의의 값
	desc->radius = 50.f; // 임의의 값
	desc->climbingMode = PxCapsuleClimbingMode::eEASY;
	//desc->contactOffset = 0.6f / 2.0f;
	//desc->slopeLimit = 45.f;
	desc->material = m_pMaterial;
	desc->position = Pos;
	desc->maxJumpHeight = 500.f; // player에서 지정해줘야될듯
	//desc->invisibleWallHeight = 0.0001f;
	desc->reportCallback = nullptr;
	desc->behaviorCallback = nullptr;
	if (!desc->isValid())
		int a = 0;
	//desc.reportCallback = (PxUserControllerHitReport*)PhysXMgr::GetInst()->GetControllerHit();
	//desc.behaviorCallback = (PxControllerBehaviorCallback*)PhysXMgr::GetInst()->GetControllerBehavior();
	//PxCapsuleController::create(PhysXMgr::GetInst()->GetScene(), &desc);
	m_pController = new CPlayerController(GetOwner(),m_pMaterial, static_cast<PxCapsuleController*>(PhysXMgr::GetInst()->GetControllerMgr()->createController(*desc)));
	
	PhysXMgr::GetInst()->GetControllerMgr()->getController(0);
	ActorObj = m_pController->GetActor();

	setupFiltering(m_pController->GetActor(), FilterGroup::ePLAYER, FilterGroup::eSPHERE);
	
	m_pRayCast = new CRayCast;

	delete desc;
}

PxShape* CRigidBody3D::CreateShape()
{
	PxShape* shape = nullptr;
	PxShapeFlags shapeFlags;
	shapeFlags = PxShapeFlag::eVISUALIZATION // 디버그 랜더링 플래그
		//| PxShapeFlag::eTRIGGER_SHAPE // 충돌할 때 트리거를 보낼지에 대한 플래그
		| PxShapeFlag::eSIMULATION_SHAPE;

	switch (m_ColliderType)
	{
	case TYPE::BOX:
	{
		PxBoxGeometry Geometry;
		Geometry = PxBoxGeometry(PxReal(Transform()->GetRelativeScale().x), PxReal(Transform()->GetRelativeScale().y), PxReal(Transform()->GetRelativeScale().z));

		shape = PhysXMgr::GetInst()->GetPhys()->createShape(Geometry, *m_pMaterial, false, shapeFlags);
		shape->setLocalPose(PxTransform());
	}
	break;
	case TYPE::SPHERE:
	{
		PxSphereGeometry Geometry = PxSphereGeometry(PxReal(Transform()->GetRelativeScale().x) / 4);
		shape = PhysXMgr::GetInst()->GetPhys()->createShape(Geometry, *m_pMaterial, false, shapeFlags);
		shape->setLocalPose(PxTransform());
	}
	break;
	case TYPE::CAPSULE:
	{
		PxCapsuleGeometry Geometry;
		Geometry = PxCapsuleGeometry(PxReal(50), PxReal(100));
		shape = PhysXMgr::GetInst()->GetPhys()->createShape(Geometry, *m_pMaterial, false, shapeFlags);
		shape->setLocalPose(PxTransform());
	}
	break;
	case TYPE::MESH:
		CreateConvex();
		return nullptr;
		break;
	case TYPE::TERRAIN:
		CreateHeightMap1();
		return nullptr;
		break;
	default:
		break;
	}

	return shape;
}

void CRigidBody3D::SettingColliderType()
{
	if (GetOwner()->GetName() == L"Player")
	{
		m_ColliderType = TYPE::CAPSULE;
		return;
	}
	m_ColliderType = TYPE::MESH;
	return;

	if (GetOwner()->Animator3D())
	{
		m_ColliderType = TYPE::CAPSULE;
	}
	else if (GetOwner()->GetRenderComponent()->GetMesh()->GetKey() == L"SphereMesh")
	{
		m_ColliderType = TYPE::SPHERE;
	}
	else if (GetOwner()->LandScape())
	{
		m_ColliderType = TYPE::TERRAIN;
	}
	// CubeMesh
	else if (!GetOwner()->Animator3D())
	{
		m_ColliderType = TYPE::BOX;
	}
	else
	{
		m_ColliderType = TYPE::MESH;
	}
}

void CRigidBody3D::SettingPlayer()
{
	Delete();
	CreatePlayerController();

}

void CRigidBody3D::StaticChange(bool _staticbool)
{
	m_bisDinamic = _staticbool;
	Create();
}

void CRigidBody3D::SetAxisLock(Vec3 _rockPos, Vec3 _rockRot)
{
	//회전,위치를 막는다

	PxRigidDynamicLockFlags Flag;
	if ((bool)_rockPos.x == true)
	{
		Flag |= PxRigidDynamicLockFlag::eLOCK_LINEAR_X;
	}

	if ((bool)_rockPos.y == true)
	{
		Flag |= PxRigidDynamicLockFlag::eLOCK_LINEAR_Y;
	}

	if ((bool)_rockPos.z == true)
	{
		Flag |= PxRigidDynamicLockFlag::eLOCK_LINEAR_Z;
	}

	if ((bool)_rockRot.x == true)
	{
		Flag |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_X;
	}
	if ((bool)_rockRot.y == true)
	{
		Flag |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y;
	}
	if ((bool)_rockRot.z == true)
	{
		Flag |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z;
	}
	PxRigidDynamic* Dynamic = reinterpret_cast<PxRigidDynamic*>(ActorObj);
	if (Dynamic == nullptr) { return; }
	Dynamic->setRigidDynamicLockFlags(Flag);
}

void CRigidBody3D::SetMass(float _mass)
{
	PxVec3 Pos = PxVec3(Transform()->GetRelativePos().x, Transform()->GetRelativePos().y, Transform()->GetRelativePos().z);
	PxRigidDynamic* Dynamic = reinterpret_cast<PxRigidDynamic*>(ActorObj);
	PxRigidBodyExt::updateMassAndInertia(*Dynamic, _mass, &Pos);
}

void CRigidBody3D::SetRot(Vec3 _Rot)
{
	m_vRot = Vec3();
	m_vRot = _Rot;
	/*m_vRot.x = 3.14 < m_vRot.x ? 3.14 : abs(m_vRot.x);
	m_vRot.y = 3.14 < m_vRot.y ? 3.14 : abs(m_vRot.y);
	m_vRot.z = 3.14 < m_vRot.z ? 3.14 : abs(m_vRot.z);*/
}

Vec3 CRigidBody3D::GetPosition_info()
{
	if (isvaild())
		return Vec3();
	PxRigidDynamic* Dynamic = reinterpret_cast<PxRigidDynamic*>(ActorObj);
	PxTransform Tr = Dynamic->getGlobalPose();
	return Vec3(Tr.p.x, Tr.p.y, Tr.p.z);
}

Vec3 CRigidBody3D::GetVelocity_info()
{
	if (isvaild())
		return Vec3();
	PxRigidDynamic* Dynamic = reinterpret_cast<PxRigidDynamic*>(ActorObj);
	Dynamic->getLinearVelocity();
	return Vec3(Dynamic->getLinearVelocity().x, Dynamic->getLinearVelocity().y, Dynamic->getLinearVelocity().z);
}

Vec3 CRigidBody3D::GetAngularVelocity_info()
{
	if (isvaild())
		return Vec3();
	PxRigidDynamic* Dynamic = reinterpret_cast<PxRigidDynamic*>(ActorObj);
	Dynamic->getAngularVelocity();

	return Vec3(Dynamic->getAngularVelocity().x, Dynamic->getAngularVelocity().y, Dynamic->getAngularVelocity().z);
}

PxTransform CRigidBody3D::GetCurTransform()
{
	// 모양 만들기
	PxVec3 Pos = PxVec3(Transform()->GetRelativePos().x, Transform()->GetRelativePos().y, Transform()->GetRelativePos().z);
	PxQuat Rot = PxQuat(Transform()->GetRelativeRotation().x, Transform()->GetRelativeRotation().y, Transform()->GetRelativeRotation().z, 1);
	return PxTransform(Pos, Rot);
}

void CRigidBody3D::AddControllVelocity(Vec3 _velop)
{
	m_pController->GetController()->move(PxVec3(_velop.x, _velop.y, _velop.z), 0.01f, CTimeMgr::GetInst()->GetDeltaTime(), PxControllerFilters());
	//m_pController->GetActor()->addForce(PxVec3(0, 10000.f, 0));

	//Vec3 Pos = Transform()->GetRelativePos();
	////포스값이 변경되었다면 변경된 값으로 업데이트
	//PxVec3 Force = PxVec3(0, 10000.f, 0);
	//PxVec3 Pos1 = PxVec3(Pos.x, Pos.y, Pos.z);
	//PxRigidBodyExt::addForceAtPos(*m_pController->GetActor(), Force, Pos1);

	////속력값이 변경되었다면 변경된 값으로 업데이트
	//PxVec3 Velocity = m_pController->GetActor()->getLinearVelocity();
	//PxVec3 Pox = PxVec3(0, 10000.f, 0);
	//if (m_vVelocity.y == 0.0f)
	//{
	//	Pox = PxVec3(m_vVelocity.x, m_vVelocity.y, m_vVelocity.z);
	//}
	//else
	//{
	//	Pox = PxVec3(m_vVelocity.x, Velocity.y, m_vVelocity.z);
	//}
	// Pox = PxVec3(0, 10000.f, 0);
	////PxVec3 Pox = PxVec3(data->Velocity.x, data->Velocity.y, data->Velocity.z);
	//m_pController->GetActor()->setLinearVelocity(Pox);
	///*a->addForce(PxVec3(0,500.f,0));*/
}

void CRigidBody3D::setupFiltering(PxRigidActor* actor, PxU32 filterGroup, PxU32 filterMask)
{
	PxFilterData filterData;
	filterData.word0 = filterGroup; // word0 = own ID
	filterData.word1 = filterMask;  // word1 = ID mask to filter pairs that trigger a
									// contact callback;
	const PxU32 numShapes = actor->getNbShapes();
	PxShape** shapes = new PxShape*[numShapes];
	actor->getShapes(shapes, numShapes);
	for (PxU32 i = 0; i < numShapes; i++)
	{
		PxShape* shape = shapes[i];
		shape->setSimulationFilterData(filterData);
	}
	// SAMPLE_FREE(shapes);
	delete[] shapes;
}

bool CRigidBody3D::isvaild()
{
	if (!m_bisDinamic && ActorObj == nullptr)
		return false;
	return true;
}


void CRigidBody3D::GetVertexData(vector<PxVec3>& _vdata, vector<Triangle>& _vidata)
{
	if (GetOwner()->MeshRender() == nullptr) return;
	_vdata.clear();
	_vidata.clear();
	vector<Vtx> vertex;
	vertex = GetOwner()->MeshRender()->GetMesh()->GetVtx();
	for (int i = 0; i < vertex.size(); ++i)
	{
		//sample.push_back(Vec3toPxVec3(vertex[i].vPos , GetOwner()->Transform()->GetRelativeScale()));
		_vdata.push_back(Vec3toPxVec3(vertex[i].vPos));
	}

	vector<UINT> a = GetOwner()->MeshRender()->GetMesh()->GetIdxData();
	for (int i = 0; i < a.size(); )
	{
		Triangle _tri;
		_tri.ind0 = a[i];
		_tri.ind1 = a[i + 1];
		_tri.ind2 = a[i + 2];
		_vidata.push_back(_tri);
		i += 3;
	}
}

void CRigidBody3D::SaveToFile(FILE* _File)
{
	COMPONENT_TYPE type = GetType();
	Vec3 Pos = Transform()->GetRelativePos();
	bool controller = false;

	fwrite(&type, sizeof(UINT), 1, _File);
	fwrite(&m_bisDinamic, sizeof(bool), 1, _File);
	fwrite(&m_bisgravity, sizeof(bool), 1, _File);
	fwrite(&m_vFreezePos, sizeof(Vec3), 1, _File);
	fwrite(&m_vFreezeRot, sizeof(Vec3), 1, _File);	
	fwrite(&Pos, sizeof(Vec3), 1, _File);
	
	if (m_pController != nullptr)
		controller = true;
	fwrite(&controller, sizeof(bool), 1, _File);	
}

void CRigidBody3D::LoadFromFile(FILE* _File)
{
	bool controller = false;
	Vec3 Pos;

	fread(&m_bisDinamic, sizeof(bool), 1, _File);
	fread(&m_bisgravity, sizeof(bool), 1, _File);
	fread(&m_vFreezePos, sizeof(Vec3), 1, _File);
	fread(&m_vFreezeRot, sizeof(Vec3), 1, _File);
	fread(&Pos, sizeof(Vec3), 1, _File);
	fread(&controller, sizeof(bool), 1, _File);
	if (controller)	
		CreatePlayerController(Pos);	
	else	
		Create();	
}