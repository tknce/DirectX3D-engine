#include "pch.h"
#include "CreateTestLevel.h"

#include <Engine\CResMgr.h>
#include <Engine\CCollisionMgr.h>

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include <Engine\GlobalComponent.h>

#include <Engine\CPaintShader.h>
#include <Engine/CLandScape.h>

#include <Script\CPlayerScript.h>
#include <Script/CScriptMgr.h>
#include "CSaveLoadMgr.h"
#include "CCameraScript.h"

//#include "CSaveLoadMgr.h"

void CreateTestLevel()
{/*
	CCollisionMgr::GetInst()->CollisionLayerCheck(0, 1);
	CCollisionMgr::GetInst()->CollisionLayerCheck(0, 2);*/



	/*CLevel* Level = CSaveLoadMgr::GetInst()->LoadLevel(L"3DTest.lv");
	CLevelMgr::GetInst()->ChangeLevel(Level);
	return;*/

	// Level 하나 제작하기
	CLevel* pLevel = new CLevel;

	// Layer 이름 설정
	pLevel->GetLayer(1)->SetName(L"Player");
	pLevel->GetLayer(2)->SetName(L"PlayerProjectile");
	pLevel->GetLayer(3)->SetName(L"Monster");
	pLevel->GetLayer(4)->SetName(L"MonsterProjectile");





	// Camera Object 추가
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");

	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	//pCamObj->AddComponent(new CMeshRender);
	pCamObj->AddComponent(CScriptMgr::GetScript(L"CCamereScript"));

	//pCamObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"FrustumMesh"));
	//pCamObj->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"frustumMtrl"),0);
	pCamObj->Transform()->SetRelativePos(0, 300.f, -300.f);


	pCamObj->Camera()->SetLayerMaskAll();
	pCamObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);

	// Instantiate(pCamObj, Vec3(0.f, 0.f, 0.f), 0);


	// Directional Light 추가
	//CGameObject* pDirLight = new CGameObject;
	//pDirLight->SetName(L"DirectionalLight");

	//pDirLight->AddComponent(new CTransform);
	//pDirLight->AddComponent(new CLight3D);

	//pDirLight->Light3D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	//pDirLight->Light3D()->SetLightAmbient(Vec3(0.15f, 0.15f, 0.15f));
	//pDirLight->Light3D()->SetRadius(300.f);
	//pDirLight->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);

	//Instantiate(pDirLight, Vec3(0.f, 0.f, 0.f), 0);

	// PointLight 추가
	CGameObject* pPointLight = new CGameObject;
	pPointLight->SetName(L"PointLight");

	pPointLight->AddComponent(new CTransform);
	pPointLight->AddComponent(new CLight3D);

	pPointLight->Transform()->SetRelativePos(Vec3(-1000.f, 1000.f, 0.f));
	pPointLight->Transform()->SetRelativeScale(800.f, 800.f, 800.f);

	pPointLight->Light3D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pPointLight->Light3D()->SetLightSpecular(Vec3(0.f, 0.f, 0.f));
	pPointLight->Light3D()->SetLightAmbient(Vec3(0.5f, 0.5f, 0.5f));
	pPointLight->Light3D()->SetRadius(800.f);
	pPointLight->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pPointLight->Light3D()->SetLightDirection(Vec3(1.f, -1.f, 1.f));

	Instantiate(pPointLight, Vec3(-1000.f, 1000.f, 0.f), 0);
	


	// GameObject 초기화
	CGameObject* pObject = nullptr;

	pObject = new CGameObject;
	pObject->SetName(L"Sphere");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	// pObject->AddComponent(new CPlayerScript);

	pObject->Transform()->SetRelativePos(Vec3(0.f, 500.f, 400.f));
	pObject->Transform()->SetRelativeScale(Vec3(256.f, 256.f, 256.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DDeferredMtrl"),0);
	pObject->MeshRender()->GetCurMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\TILE_01.tga"));
	pObject->MeshRender()->GetCurMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\tile\\TILE_01_N.tga"));

	Instantiate(pObject, Vec3(0.f, 500.f, 400.f), 0);

	pObject = new CGameObject;
	pObject->SetName(L"Plane");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetRelativePos(Vec3(0.f, -200.f, 400.f));
	pObject->Transform()->SetRelativeScale(Vec3(1000.f, 1.f, 1000.f));
	pObject->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DDeferredMtrl"),0);

	Instantiate(pObject, Vec3(0.f, 500.f, 400.f), 0);

	//pObject = new CGameObject;
	//pObject->SetName(L"Decal");

	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CDecal);

	//pObject->Transform()->SetRelativePos(Vec3(0.f, -200.f, 400.f));
	//pObject->Transform()->SetRelativeScale(Vec3(250.f, 250.f, 250.f));

	//pObject->Decal()->SetDecalTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\MagicCircle.png"));

	//Instantiate(pObject, Vec3(0.f, -200.f, 400.f), 0);

	// SkyBox 추가
	CGameObject* pSkyBox = new CGameObject;
	pSkyBox->SetName(L"SkyBox");

	pSkyBox->AddComponent(new CTransform);
	pSkyBox->AddComponent(new CSkyBox);

	pSkyBox->Transform()->SetRelativeScale(300.f, 300.f, 300.f);
	pSkyBox->SkyBox()->SetSkyBoxTex(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\skybox\\SkyWater.dds"));
	pSkyBox->SkyBox()->SetType(SKYBOX_TYPE::CUBE);
	pSkyBox->SkyBox()->SetDynamicShadow(false);

	Instantiate(pSkyBox, Vec3(0.f, 500.f, 400.f), 0);

	// LandScape 추가
	CGameObject* pLandScape = new CGameObject;
	pLandScape->SetName(L"LandScape");

	pLandScape->AddComponent(new CTransform);
	pLandScape->AddComponent(new CLandScape);

	pLandScape->Transform()->SetRelativeScale(100.f, 100.f, 100.f);
	pLandScape->LandScape()->SetFaceCount(4, 4);
	pLandScape->LandScape()->SetFrustumCulling(false);

	Instantiate(pLandScape, Vec3(0.f, 0.f, 0.f), 0);

	// ============
	// FBX Loading
	// ============	
	{
		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj1 = nullptr;
		//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\monster.fbx");
		pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\monster.mdat");

		pObj1 = pMeshData->Instantiate();
		pObj1->SetName(L"Monster");
		pObj1->Transform()->SetRelativeScale(10.f, 10.f, 10.f);
		Instantiate(pObj1, Vec3(5000.f, 0.f, 0.f), 0);
	}
	{
		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = nullptr;
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Player_Artorias.fbx");
		pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\Player_Artorias_0.mdat");

		pObj = pMeshData->Instantiate();
		pObj->SetName(L"map");
		// pObj->AddComponent(CScriptMgr::GetScript(L"CThreeDPlayerScript"));

		pObj->Transform()->SetRelativeScale(1.f, 1.f, 1.f);
		pObj->Transform()->SetRelativeRotation(-3.1415926/2, 0.f, 0.f);
		// Instantiate(pObj, Vec3(50.f, 0.f, 50.f), 0);
		for (int i = 1; i < 15; ++i)
		{
			Ptr<CMeshData> pMeshData = nullptr;
			CGameObject* pObj1 = nullptr;
			//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Player_Artorias.fbx");
			wstring mesh = L"meshdata\\";
			wstring artor = L"Player_Artorias_";
			mesh += artor + std::to_wstring(i) + L".mdat";
			pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(mesh);

			pObj1 = pMeshData->Instantiate();
			pObj1->SetName(L"map" + std::to_wstring(i));
			pObj1->Transform()->SetRelativeScale(1.f, 1.f, 1.f);
			//Instantiate(pObj1, Vec3(0.f, 0.f, 0.f), 0);

			pObj->AddChild(pObj1);
			
		}
		pObj->Animator3D()->SetClipTime(0,0);
		for (int i = 0; i < pObj->GetChildObject().size(); ++i)
		{

			pObj->GetChildObject()[i]->Animator3D()->SetClipTime(0,0);
		}
		pMeshData = nullptr;
		CGameObject* pObj1 = nullptr;
		pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\GreateSword.mdat");

		pObj1 = pMeshData->Instantiate();
		pObj1->SetName(L"sword");
		pObj1->Transform()->SetRelativeScale(1.f, 1.f, 1.f);
		// Instantiate(pObj1, Vec3(0.f, 0.f, 0.f), 0);
		pObj1->Transform()->SetSocket(pObj);
		pObj->AddChild(pObj1);


		pObj1 = new CGameObject;
		pObj1->SetName(L"CameraArm");
		pObj1->AddComponent(new CTransform);
		pObj1->AddChild(pCamObj);

		CGameObject* pPlayer = nullptr;
		pPlayer = new CGameObject;
		pPlayer->AddComponent(new CTransform);
		pPlayer->AddComponent(new CRigidBody3D);
		pPlayer->RigidBody3D()->CreatePlayerController();
		pPlayer->AddComponent(CScriptMgr::GetScript(L"CThreeDPlayerScript"));
		pPlayer->SetName(L"Player");
		pPlayer->AddChild(pObj);
		pPlayer->AddChild(pObj1);
		Instantiate(pPlayer, Vec3(0.f, 500.f, 0.f), 0);

	}


	//for (int i = 0; i < 5; ++i)
	//{
	//	pObject = new CGameObject;
	//	wstring name;
	//	name = L"Plane";
	//	name += std::to_wstring(i);
	//	pObject->SetName(L"plane");

	//	pObject->AddComponent(new CTransform);
	//	pObject->AddComponent(new CMeshRender);


	//	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f * i));
	//	pObject->Transform()->SetRelativeScale(Vec3(1024.f, 1024.f, 1.f));

	//	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));

	//	pLevel->AddGameObject(pObject, 0);
	//}

	// PointLight 추가
	//CGameObject* pPointLight = new CGameObject;
	//pPointLight->SetName(L"PointLight");

	//pPointLight->AddComponent(new CTransform);
	//pPointLight->AddComponent(new CLight2D);

	//pPointLight->Transform()->SetRelativePos(0.f, 0.f, 0.f);

	//pPointLight->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	//pPointLight->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
	//pPointLight->Light2D()->SetRadius(500.f);
	//pPointLight->Light2D()->SetAngle(XM_PI / 3);


	//pLevel->AddGameObject(pPointLight, 0);


	// GameObject 초기화
	/*CGameObject* pObject = nullptr;

	pObject = new CGameObject;
	pObject->SetName(L"Player");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CAnimator2D);
	pObject->AddComponent(new CPlayerScript);
	pObject->AddComponent(new CRigidBody2D);

	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 800.f));
	pObject->Transform()->SetRelativeScale(Vec3(1024.f, 1024.f, 0.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	pObject->Animator2D()->CreateAnimation(L"LeftWalk", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\link.png"), Vec2(0.f, 650.f), Vec2(120.f, 130.f), 120.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"Walk", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\link.png"), Vec2(0.f, 350.f), Vec2(120.f, 130.f), 120.f, 10, 16);
	
	pObject->Animator2D()->Play(L"LeftWalk", true);

	int a = 0;
	Vec4 vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	pObject->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, &vColor);
	pObject->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &a);
	pObject->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\particle\\smokeparticle.png"));

	pLevel->AddGameObject(pObject, 0);*/


	//CGameObject* pChild = new CGameObject;
	//pChild->SetName(L"Child");

	//pChild->AddComponent(new CTransform);
	//pChild->AddComponent(new CMeshRender);
	//pChild->AddComponent(new CCollider2D);

	//pChild->Transform()->SetIgnoreParentScale(true);
	//pChild->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	//pChild->Transform()->SetRelativeScale(Vec3(2000.f, 2000.f, 1.f));

	//pChild->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pChild->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	//pChild->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Plane"));




	//pLevel->AddGameObject(pChild, 1);



	// Particle Object
	//CGameObject* pParticle = new CGameObject;
	//pParticle->SetName(L"particle");
	//pParticle->AddComponent(new CTransform);
	//pParticle->AddComponent(new CParticleSystem);

	//pParticle->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	//pParticle->ParticleSystem()->SetWorldSpawn(true);

	//pLevel->AddGameObject(pParticle, 0);


	// PostProcess Object
	//CGameObject* pPostProcess = new CGameObject;
	//pPostProcess->SetName(L"postprocess");
	//pPostProcess->AddComponent(new CTransform);
	//pPostProcess->AddComponent(new CMeshRender);
	//

	//pPostProcess->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));
	//pPostProcess->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));

	//pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pPostProcess->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"PostProcessMtrl"));

	//pLevel->AddGameObject(pPostProcess, 0);




	//pObject = new CGameObject;
	//pObject->SetName(L"Monster");

	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->AddComponent(new CCollider2D);


	//pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 800.f));
	//pObject->Transform()->SetRelativeScale(Vec3(256.f, 256.f, 0.f));

	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	//pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);

	//a = 0;
	//vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	//pObject->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, &vColor);
	//pObject->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &a);
	//pObject->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Character"));

	//pLevel->AddGameObject(pObject, 0);


	// TileMap Object
	//CGameObject* pTileMapObj = new CGameObject;
	//pTileMapObj->SetName(L"TileMap");
	//pTileMapObj->AddComponent(new CTransform);
	//pTileMapObj->AddComponent(new CTileMap);

	//pTileMapObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 1000.f));
	//pTileMapObj->Transform()->SetRelativeScale(Vec3(1000.f, 800.f, 1.f));

	//pTileMapObj->TileMap()->SetTileAtlas(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\TILE.bmp"));
	//pTileMapObj->TileMap()->SetTileCount(10, 10);

	//pLevel->AddGameObject(pTileMapObj, 0);

	// ComputeShader 테스트
	//Ptr<CPaintShader> pComputeShader = (CPaintShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"PaintShader").Get();
	//pComputeShader->SetTexture(CResMgr::GetInst()->FindRes<CTexture>(L"UAVTex"));
	//pComputeShader->SetColor(Vec4(0.f, 0.f, 1.f, 1.f));
	//pComputeShader->Execute();

	// 충돌 레이어 설정
	// 충돌 레이어 설정
	


	// Level 저장
	//CSaveLoadMgr::GetInst()->SaveLevel(pLevel, L"level\\Test.lv");

	pLevel->SetName(L"3DTest.lv");

	// 레벨 설정
	CLevelMgr::GetInst()->ChangeLevel(pLevel);
}
