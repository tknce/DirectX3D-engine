#include "stdio.h"
#include "Factory.h"
#include "PxPhysicsAPI.h"
#include "PhysData.h"
#include "PhysEngine.h"

#include "Phys_Collider_Box.h"
#include "Phys_Collider_Capsule.h"
#include "Phys_Collider_Sphere.h"
#include "Phys_Collider_Triangle.h"

#include "BaseData.h"


using namespace physx;
Factory::Factory()
{
	m_Phys		= nullptr;
	m_Scene		= nullptr;
	m_Cooking	= nullptr;
}

Factory::~Factory()
{
	m_Phys = nullptr;
	m_Scene = nullptr;
}

void Factory::Initialize(physx::PxPhysics* Phys, physx::PxScene* Scene, physx::PxCooking* Cooking)
{
	m_Phys		= Phys;
	m_Scene		= Scene;
	m_Cooking	= Cooking;
}

physx::PxShapeFlags Factory::CreateShapeFlag(bool isTrigger)
{
	PxShapeFlags shapeFlags;

	if (isTrigger == true)
	{
		shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eTRIGGER_SHAPE;
	}
	else
	{
		shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSIMULATION_SHAPE | PxShapeFlag::eSCENE_QUERY_SHAPE;
	}
	
	return shapeFlags;
}

physx::PxShape* Factory::CreateShape(PhysMaterial* m, PhysCollider* c, bool isTrigger)
{
	///재질을 지정해준다
	PxMaterial* pMaterial = CreateMaterial(m);

	///타입별 shape 생성
	Vector3 Size = c->GetSize();
	PxShape* shape = nullptr;
	switch (c->GetType())
	{
	case PhysCollider::TYPE::BOX:
		shape = CreateBoxCollider(pMaterial, c->GetBoxCollider(),isTrigger);
		break;
	case PhysCollider::TYPE::SPHERE:
		shape = CreateSphereCollider(pMaterial, c->GetSphereCollider(), isTrigger);
		break;
	case PhysCollider::TYPE::CAPSULE:
		shape = CreateCapsuleCollider(pMaterial, c->GetCapsuleCollider(), isTrigger);
		break;
	case PhysCollider::TYPE::MESH:
		shape = CreateTriangleCollider(pMaterial, c->GetTriangleCollider());
		break;
	case PhysCollider::TYPE::TERRAIN:
		//shape = CreateHeightFieldCollider(pMaterial, c);
		break;
	}

	
	if (pMaterial != nullptr)
	{
		pMaterial->release();
	}

	return shape;
}

PxShape* Factory::CreateBoxCollider(physx::PxMaterial* m, Phys_Collider_Box* c, bool isTrigger)
{
	Phys_Base_Vector3&& Size = c->GetSize();
	PxBoxGeometry Geometry	= PxBoxGeometry(PxReal(Size.x), PxReal(Size.y), PxReal(Size.z));
	PxShape* shape			= m_Phys->createShape(Geometry, *m, false , CreateShapeFlag(isTrigger));
	return shape;
}

PxMaterial* Factory::CreateMaterial(PhysMaterial* m)
{
	if (m == nullptr)
	{
		return m_Phys->createMaterial(0.5f, 0.5f, 0);
	}
	else
	{
		return m_Phys->createMaterial(m->MT_StaticFriction, m->MT_DynamicFriction, m->MT_Restitution);
	}
}

physx::PxShape* Factory::CreateSphereCollider(physx::PxMaterial* m, Phys_Collider_Sphere* c ,bool isTrigger)
{
	PxSphereGeometry temp = PxSphereGeometry(PxReal(c->Radius));
	PxShape* shape = m_Phys->createShape(temp, *m, false, CreateShapeFlag(isTrigger));
	return shape;
}

physx::PxShape* Factory::CreateCapsuleCollider(physx::PxMaterial* m, Phys_Collider_Capsule* c, bool isTrigger)
{
	PxCapsuleGeometry temp = PxCapsuleGeometry(c->Radius,c->Helght);
	PxShape* shape = m_Phys->createShape(temp, *m, false, CreateShapeFlag(isTrigger));
	return shape;
}

void Factory::CreateDinamicActor(PhysData* Data, physx::PxShape* shape, physx::PxTransform* Tr)
{
	if (Data->isDinamic == false) { return; }
	PxRigidDynamic* body = m_Phys->createRigidDynamic(*Tr);
	SetAxisLock(body, Data);

	//const PxFilterData triggerFilterData(0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff);
	//shape->setSimulationFilterData(triggerFilterData);

	shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	shape->setFlag(PxShapeFlag::eVISUALIZATION, true); //Ray, sweep등 할때 사용됨
	shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, Data->isTrigger);

	body->attachShape(*shape);

	physx::PxVec3 temp = physx::PxVec3(Data->CenterPoint.x, Data->CenterPoint.y, Data->CenterPoint.z);

	PxRigidBodyExt::updateMassAndInertia(*body, Data->MT_Mass,&temp);

	body->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, Data->isKinematic);
	body->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !Data->isGravity);
	

	//서로 연결
	body->userData = Data;
	Data->ActorObj = body;


	m_Scene->addActor(*body);
}

void Factory::CreateStaticActor(PhysData* Data, physx::PxShape* shape, physx::PxTransform* Tr)
{
	if (Data->isDinamic == true) { return; }
	PxRigidStatic* body = m_Phys->createRigidStatic(*Tr);
	
	//const PxFilterData triggerFilterData(0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff);
	//shape->setSimulationFilterData(triggerFilterData);

	body->attachShape(*shape);
	m_Scene->addActor(*body);
	
	//서로 연결
	body->userData = Data;
	Data->ActorObj = body;
}

void Factory::CreateActoer(PhysData* data)
{
	PhysMaterial* mMaterial = data->mMeterial;
	PhysCollider* mCollider = data->mCollider;
	
	///위치,회전을 지정해준다
	PxVec3 Pos				= PxVec3(data->WorldPosition.x, data->WorldPosition.y, data->WorldPosition.z);
	PxQuat Rot				= PxQuat(data->Rotation.x, data->Rotation.y, data->Rotation.z, data->Rotation.w);
	PxTransform  mTransform = PxTransform(Pos,Rot);

	PxShape* shape = CreateShape(mMaterial, mCollider,data->isTrigger);

	///로컬 포지션을 지정
	shape->setLocalPose(PxTransform(data->CenterPoint.x, data->CenterPoint.y, data->CenterPoint.z));

	///물리 객체 생성
	if (data->isDinamic == true)
	{
		CreateDinamicActor(data, shape, &mTransform);
	}
	else
	{
		CreateStaticActor(data, shape, &mTransform);
	}

	///생성이 되었다면 사용한 물리객체를 삭제해준다
	if (shape != nullptr)
	{
		shape->release();
	}

}


void Factory::SetAxisLock(PxRigidDynamic* Actor, PhysData* Data)
{	
	//회전,위치를 막는다

	PxRigidDynamicLockFlags Flag;
	if((bool)Data->FreezePositon.x == true)
	{
		Flag |=PxRigidDynamicLockFlag::eLOCK_LINEAR_X;
	}

	if ((bool)Data->FreezePositon.y == true)
	{
		Flag |= PxRigidDynamicLockFlag::eLOCK_LINEAR_Y;
	}
	
	if ((bool)Data->FreezePositon.z == true)
	{
		Flag |= PxRigidDynamicLockFlag::eLOCK_LINEAR_Z;
	}

	if ((bool)Data->FreezeRotaticon.x == true)
	{
		Flag |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_X;
	}
	if ((bool)Data->FreezeRotaticon.x == true)
	{
		Flag |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y;
	}
	if ((bool)Data->FreezeRotaticon.x == true)
	{
		Flag |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z;
	}

	Actor->setRigidDynamicLockFlags(Flag);
}

physx::PxShape* Factory::CreateTriangleCollider(physx::PxMaterial* m, Phys_Collider_Triangle* c)
{ 
	PxTriangleMeshDesc meshDesc;
	//버텍스 관련 데이터
	meshDesc.points.count		= c->GetVertexCount();
	meshDesc.points.stride		= sizeof(Phys_Base_Vector3);
	meshDesc.points.data		= c->GetVertexList();

	//페이스 관련 데이터
	meshDesc.triangles.count	= c->GetIndexCount() / 3;
	meshDesc.triangles.stride	= 3 * sizeof(unsigned int);
	meshDesc.triangles.data		= c->GetIndexList();

	PxTriangleMesh* triMesh = m_Cooking->createTriangleMesh(meshDesc, m_Phys->getPhysicsInsertionCallback());
	PxTriangleMeshGeometry geom;
	geom.triangleMesh = triMesh;
	physx::PxShape* shape = m_Phys->createShape(geom, *m);

	return shape;
}

physx::PxShape* Factory::CreateHeightFieldCollider(physx::PxMaterial* m, Phys_Collider_Triangle* c)
{
	// 높이맵 작성중
	//PxTriangleMeshDesc meshDesc;
	//meshDesc.points.count = vertices->size();
	//meshDesc.points.stride = sizeof(PxVec3);
	//meshDesc.points.data = vertices;

	//meshDesc.triangles.count = vertices->size() / 3;
	//meshDesc.triangles.stride = 3 * sizeof(unsigned int);
	//meshDesc.triangles.data = indices;

	//assert(meshDesc.isValid());

	//PxDefaultMemoryOutputStream writeBuffer;
	//PxTriangleMeshCookingResult::Enum result;
	//bool status = my_cooking->cookTriangleMesh(meshDesc, writeBuffer, &result);
	//if (!status)
	//	return NULL;

	//PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	//return p.createTriangleMesh(readBuffer);

	return nullptr;

}

//PxVec3* heightfieldVertexes;
//PxU32* heightfieldIndices;
//heightfield:addHeightfield("particles_heightmap.tga", -10.0f, 2.0f, 2.0f, 0.3f, physicsSimulator->getInstance().createMaterial(0.9f, 0.9f, 0.001f), heightfieldVertexes, heightfieldIndices);
//
//void PhysicsSimulator::addHeightfield(char* heightfieldTexturePath, PxReal thickness, PxReal xScale, PxReal yScale, PxReal zScale, PxMaterial* material, PxVec3*& vertices, PxU32*& indices)
//{
//
//
//
//	PxU16 nbColumns = PxU16(FreeImage_GetWidth(heightfieldTexture));
//	PxU16 nbRows = PxU16(FreeImage_GetHeight(heightfieldTexture));
//	PxHeightFieldDesc heightfieldDesc;
//	heightfieldDesc.nbColumns = nbColumns;
//	heightfieldDesc.nbRows = nbRows;
//	PxU32* samplesData = (PxU32*)malloc(sizeof(PxU32) * (nbColumns * nbRows));
//	heightfieldDesc.samples.data = samplesData;
//	heightfieldDesc.samples.stride = sizeof(PxU32);
//	heightfieldDesc.convexEdgeThreshold = 0;
//	PxU8* currentByte = (PxU8*)heightfieldDesc.samples.data;
//	PxU32 texturePitch = PxU32(FreeImage_GetPitch(heightfieldTexture));
//	PxU8* loaderPtr = (PxU8*)FreeImage_GetBits(heightfieldTexture);
//	vertices = (PxVec3*)malloc(sizeof(PxVec3) * (nbColumns * nbRows));
//	PxReal* UVs = (PxReal*)malloc(sizeof(PxReal) * ((nbColumns * nbRows * 2)));
//
//	for (PxU32 row = 0; row < nbRows; row++)
//	{
//		PxU8* pixel = (PxU8*)loaderPtr;
//		for (PxU32 column = 0; column < nbColumns; column++)
//		{
//			PxHeightFieldSample* currentSample = (PxHeightFieldSample*)currentByte;
//			currentSample->height = pixel[FI_RGBA_BLUE];
//			vertices[row * nbColumns + column] = PxVec3(PxReal(row) * xScale, PxReal(currentSample->height * zScale), PxReal(column) * yScale);
//			UVs[2 * (row * nbColumns + column)] = (PxReal(row) / PxReal(nbRows)) * 7.0f;
//			UVs[2 * (row * nbColumns + column) + 1] = (PxReal(column) / PxReal(nbColumns)) * 7.0f;
//			currentSample->materialIndex0 = 0;
//			currentSample->materialIndex1 = 0;
//			currentSample->clearTessFlag();
//			currentByte += heightfieldDesc.samples.stride;
//			pixel += 3;
//		}
//		loaderPtr += texturePitch;
//	}
//
//
//	PxHeightField* heightfield = mPhysics->createHeightField(heightfieldDesc);
//	// free alocated memory for heightfield samples description
//	//free(samplesData);
//	// create shape for heightfield
//	PxTransform pose(PxVec3(-((PxReal)nbRows * yScale) / 2.0f, 0.0f, -((PxReal)nbColumns * xScale) / 2.0f), PxQuat(PxIdentity));
//	PxRigidActor* hf = mPhysics->createRigidStatic(pose);
//	PxShape* shape = hf->createShape(PxHeightFieldGeometry(heightfield, PxMeshGeometryFlags(), zScale, xScale, yScale), *material);
//	shape->setFlag(PxShapeFlag::ePARTICLE_DRAIN, false);
//	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
//	// add actor to the scene
//	mScene->addActor(*hf);
//	mActors.push_back(hf);
//	// create indices and UVs
//	indices = (PxU32*)malloc(sizeof(PxU32) * (nbColumns - 1) * (nbRows - 1) * 3 * 2);
//	for (int i = 0; i < (nbColumns - 1); ++i)
//	{
//		for (int j = 0; j < (nbRows - 1); ++j)
//		{
//			// first triangle
//			indices[6 * (i * (nbRows - 1) + j) + 0] = (i + 1) * nbRows + j;
//			indices[6 * (i * (nbRows - 1) + j) + 1] = i * nbRows + j;
//			indices[6 * (i * (nbRows - 1) + j) + 2] = i * nbRows + j + 1;
//			// second triangle
//			indices[6 * (i * (nbRows - 1) + j) + 3] = (i + 1) * nbRows + j + 1;
//			indices[6 * (i * (nbRows - 1) + j) + 4] = (i + 1) * nbRows + j;
//			indices[6 * (i * (nbRows - 1) + j) + 5] = i * nbRows + j + 1;
//		}
//	}
//}



