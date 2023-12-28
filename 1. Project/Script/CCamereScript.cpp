#include "pch.h"
#include "CCamereScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CCamereScript::CCamereScript()
	: CScript(CAMERESCRIPT)
	, m_fSpeed(100.f)
{
}

CCamereScript::~CCamereScript()
{
}

void CCamereScript::begin()
{
	/*CGameObject* Player = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"map");

	Camera()->SetObject(Player);*/
}

void CCamereScript::tick()
{
	Move();
}

void CCamereScript::RotateCamera(float dx, float dy)
{
	Vec3 cameraTarget = GetOwner()->Transform()->GetRelativePos();

	// Adjust rotation angles based on mouse movement
	static float yaw = 0.0f;
	static float pitch = 0.0f;

	const float rotationSpeed = 0.005f;
	yaw = dx * rotationSpeed * DT;
	pitch = dy * rotationSpeed * DT;

	// Limit pitch to avoid gimbal lock
	const float maxPitch = XM_PI / 2.0f - 0.01f;

	pitch  = -maxPitch > (maxPitch < pitch ? maxPitch : pitch) ? -maxPitch : (maxPitch < pitch ? maxPitch : pitch);
	//pitch = max(-maxPitch, std::min(maxPitch, pitch));

	// Create rotation matrix
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f);

	// Update camera vectors based on rotation matrix
	XMFLOAT3 worldUp(0.0f, 1.0f, 0.0f);
	XMFLOAT3 rotatedUp, rotatedTarget;
	XMStoreFloat3(&rotatedUp, XMVector3TransformCoord(XMLoadFloat3(&worldUp), rotationMatrix));
	XMStoreFloat3(&rotatedTarget, XMVector3TransformCoord(XMLoadFloat3(&cameraTarget), rotationMatrix));

	// Set new camera target and up vectors
	cameraTarget = rotatedTarget;
	if (cameraTarget != GetOwner()->Transform()->GetRelativePos())
		int a = 0;

	GetOwner()->Transform()->SetRelativePos(cameraTarget);
	//cameraUp = rotatedUp;

	// Recreate the view matrix
	// XMStoreFloat4x4(&viewMatrix, XMMatrixLookAtLH(XMLoadFloat3(&cameraPosition), XMLoadFloat3(&cameraTarget), XMLoadFloat3(&cameraUp)));
}


void CCamereScript::Move()
{
	if (Camera()->GetProjType() == PROJ_TYPE::PERSPECTIVE)
	{
		Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();


		if (KEY_PRESSED(KEY::RBTN))
		{
			Vec3 vRot = GetOwner()->GetParent()->Transform()->GetRelativeRotation();

			vRot.y += vMouseDir.x * DT * XM_PI;
			vRot.x -= vMouseDir.y * DT * XM_PI;

			GetOwner()->GetParent()->Transform()->SetRelativeRotation(vRot);
			GetOwner()->Transform()->SetRelativeRotation(vRot);
			GetOwner()->GetParent()->GetParent()->GetChildwstring(L"map")->Transform()->SetRelativeRotationY(vRot.y);
		}
	}
}