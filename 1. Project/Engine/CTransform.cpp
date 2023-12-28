#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "GlobalComponent.h"
#include "CAnimation3D.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
	, m_bIgnParentScale(false)
{
	m_vRelativeDir[(UINT)DIR::RIGHT]	= Vec3(1.f, 0.f, 0.f);
	m_vRelativeDir[(UINT)DIR::UP]		= Vec3(0.f, 1.f, 0.f);
	m_vRelativeDir[(UINT)DIR::FRONT]	= Vec3(0.f, 0.f, 1.f);
	if (m_pSocket != nullptr)
		m_pSocket = nullptr;
}

CTransform::CTransform(const CTransform& _origin)
: CComponent(COMPONENT_TYPE::TRANSFORM)
,m_vRelativePos(_origin.m_vRelativePos)
,m_vRelativeRotation(_origin.m_vRelativeRotation)
,m_vRelativeScale(_origin.m_vRelativeScale)
,m_matRelativeRot(_origin.m_matRelativeRot)
,m_matRegidRot(_origin.m_matRegidRot)
,m_matWorld(_origin.m_matWorld)
,m_matWorldInv(_origin.m_matWorldInv)
,m_matSocket()
,m_pSocket(nullptr)
,m_bIgnParentScale(_origin.m_bIgnParentScale)
{
	m_vRelativeDir[(UINT)DIR::RIGHT]	= Vec3(1.f, 0.f, 0.f);
	m_vRelativeDir[(UINT)DIR::UP]		= Vec3(0.f, 1.f, 0.f);
	m_vRelativeDir[(UINT)DIR::FRONT]	= Vec3(0.f, 0.f, 1.f);
}

CTransform::~CTransform()
{
}

void CTransform::tick()
{

}

void CTransform::finaltick()
{
	// m_vRelativePos, m_vRelativeScale, m_vRelativeRotation
	// ��=> ��������� ����� ��
	m_matWorld = XMMatrixIdentity();

	Matrix matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);

	Matrix matTrans = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

	if (!RigidBody3D())
	{
		m_matRelativeRot = XMMatrixRotationX(m_vRelativeRotation.x);
		m_matRelativeRot *= XMMatrixRotationY(m_vRelativeRotation.y);
		m_matRelativeRot *= XMMatrixRotationZ(m_vRelativeRotation.z);
	}
	else
	{
		if(RigidBody3D()->GetPlayercontroller() == nullptr)
			m_matRelativeRot = m_matRegidRot;	 
		else
		{
			m_matRelativeRot = XMMatrixRotationX(m_vRelativeRotation.x);
			m_matRelativeRot *= XMMatrixRotationY(m_vRelativeRotation.y);
			m_matRelativeRot *= XMMatrixRotationZ(m_vRelativeRotation.z);
		}
	}


	// ȸ������� �̿��ؼ� ���� ��ü�� ��, ��, �� ������ ���س��´�.
	m_vRelativeDir[(UINT)DIR::RIGHT] = m_vWorldDir[(UINT)DIR::RIGHT] = XMVector3TransformNormal(Vec3(1.f, 0.f, 0.f), m_matRelativeRot);
	m_vRelativeDir[(UINT)DIR::UP]	 = m_vWorldDir[(UINT)DIR::UP]	 = XMVector3TransformNormal(Vec3(0.f, 1.f, 0.f), m_matRelativeRot);
	m_vRelativeDir[(UINT)DIR::FRONT] = m_vWorldDir[(UINT)DIR::FRONT] = XMVector3TransformNormal(Vec3(0.f, 0.f, 1.f), m_matRelativeRot);

	m_matWorld = matScale * m_matRelativeRot * matTrans;

	// �θ� ������Ʈ�� �ִٸ�
	if (GetOwner()->GetParent())
	{
		// �θ� ������Ʈ�� ũ�⸦ �����ϱ�� �� ���
		if (m_bIgnParentScale)
		{
			// �������� ��� �θ��� ũ�Ⱚ�� �����ؼ� ������� ����� �д�.
			CGameObject* pParent = GetOwner()->GetParent();
			Vec3 vParentScale = Vec3(1.f, 1.f, 1.f);

			while (pParent)
			{
				vParentScale *= pParent->Transform()->GetRelativeScale();

				// �θ� ������Ʈ�� �� ���� ũ�⸦ �����ϱ�� �� ��� ũ�⸦ ���̻� ������ �ʿ䰡 ����.
				if (pParent->Transform()->m_bIgnParentScale)
					pParent = nullptr;
				else			
					pParent = pParent->GetParent();
			}

			// �θ� ����� ũ��κ��� ������� ���ؼ� ����Ų��.
			// ����� ����ó��
			if (0.f == vParentScale.x)
				vParentScale.x = 1.f;
			if (0.f == vParentScale.y)
				vParentScale.y = 1.f;
			if (0.f == vParentScale.z)
				vParentScale.z = 1.f;

			Matrix matParentScaleInv = XMMatrixScaling(vParentScale.x, vParentScale.y, vParentScale.z);
			matParentScaleInv = XMMatrixInverse(nullptr, matParentScaleInv);

			// ���� ������� �̸� ���صΰ� �θ��� ��������� ���Ѵ�.
			m_matWorld = m_matWorld * matParentScaleInv * GetOwner()->GetParent()->Transform()->GetWorldMat();			
		}
		else
		{
			m_matWorld *= GetOwner()->GetParent()->Transform()->GetWorldMat();
		}	

		// WorldDir ���ϱ�
		m_vWorldDir[(UINT)DIR::RIGHT] = XMVector3TransformNormal(Vec3(1.f, 0.f, 0.f), m_matWorld);
		m_vWorldDir[(UINT)DIR::UP]	  = XMVector3TransformNormal(Vec3(0.f, 1.f, 0.f), m_matWorld);
		m_vWorldDir[(UINT)DIR::FRONT] = XMVector3TransformNormal(Vec3(0.f, 0.f, 1.f), m_matWorld);

		// ȸ��, ũ�� ��ȯ�� �̷������ ������ ����� ũ�⸦ �ʱ�ȭ �ϱ� ���� Normalize ���ش�.����
		m_vWorldDir[(UINT)DIR::RIGHT].Normalize();
		m_vWorldDir[(UINT)DIR::UP].Normalize();
		m_vWorldDir[(UINT)DIR::FRONT].Normalize();
	}

	m_matWorldInv = XMMatrixInverse(nullptr, m_matWorld);


}


void CTransform::UpdateData()
{
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
		
	g_transform.matWorld = m_matWorld;
	g_transform.matWorldInv = m_matWorldInv;
	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;

	int a = -1;

	static int b = 505;
	static float acctime = 0;
	if (m_pSocket != nullptr)
	{
		if (m_pSocket->Animator3D() != nullptr)
		{
			acctime += DT;
			// null�̷� �����ʿ�
			m_pSocket->Animator3D()->GetOriginBoneMat()->UpdateData(31, PIPELINE_STAGE::VS);

			if (MeshRender() != nullptr)
			{
				a = 1;
				m_pSocket->Animator3D()->SetSocket(true);
				vector<Vtx> _vtc = m_pSocket->MeshRender()->GetMesh()->GetVtx();

				if (KEY_TAP(KEY::K))
				{
					++b;
					if (_vtc.size()-2 < b)
					{
						b = 0;
					}
				}
				if (acctime > 0.05f)
				{
					//++b;
					acctime = 0;
					if (_vtc.size()-2 < b)
					{
						b = 0;
					}
				}
				
				MeshRender()->GetSharedMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_2, &a);
				MeshRender()->GetSharedMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_2, &_vtc[b].vWeights);
				MeshRender()->GetSharedMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_3, &_vtc[b].vIndices);			
				//m_pSocket->Animator3D()->GetOriginBoneMat()->Clear();
			}
		}
	}



	
	pCB->SetData(&g_transform);
	pCB->UpdateData(PIPELINE_STAGE::ALL_STAGE);
}

Vec3 CTransform::GetWorldScale()
{
	// �������� ��� �θ��� ũ�Ⱚ�� �����ؼ� ������� ����� �д�.	
	Vec3 vWorldScale = m_vRelativeScale;

	if (m_bIgnParentScale)
		return vWorldScale;

	CGameObject* pParent = GetOwner()->GetParent();
	while (pParent)
	{		
		vWorldScale *= pParent->Transform()->GetRelativeScale();

		// �θ� ������Ʈ�� �� ���� ũ�⸦ �����ϱ�� �� ��� ũ�⸦ ���̻� ������ �ʿ䰡 ����.
		if (pParent->Transform()->m_bIgnParentScale)
			pParent = nullptr;
		else
			pParent = pParent->GetParent();
	}

	return vWorldScale;
}

void CTransform::SetSocket(CGameObject* _socket)
{
	m_pSocket = _socket;
	int a = 0;
	MeshRender()->GetSharedMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_2, &a);
}

Matrix CTransform::GetWorldRotation()
{
	Matrix matWorldRot = m_matRelativeRot;

	CGameObject* pParent = GetOwner()->GetParent();

	while (pParent)
	{
		// �����ؼ� ���ϱ�
		matWorldRot *= pParent->Transform()->m_matRelativeRot;
		pParent = pParent->GetParent();
	}

	return matWorldRot;
}

void CTransform::SaveToFile(FILE* _File)
{
	COMPONENT_TYPE type = GetType();
	fwrite(&type, sizeof(UINT), 1, _File);

	fwrite(&m_vRelativePos, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeRotation, sizeof(Vec3), 1, _File);
	fwrite(&m_bIgnParentScale, sizeof(bool), 1, _File);

}

void CTransform::LoadFromFile(FILE* _File)
{
	fread(&m_vRelativePos,sizeof(Vec3),1,_File);
	fread(&m_vRelativeScale,sizeof(Vec3),1,_File);
	fread(&m_vRelativeRotation,sizeof(Vec3),1,_File);
	fread(&m_bIgnParentScale, sizeof(bool), 1, _File);
}

void CTransform::SaveTurnBackTime(FILE* _File)
{
	fwrite(&m_vRelativePos, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeRotation, sizeof(Vec3), 1, _File);
}

void CTransform::LoadTurnBackTime(FILE* _File)
{
	fread(&m_vRelativePos, sizeof(Vec3), 1, _File);
	fread(&m_vRelativeRotation, sizeof(Vec3), 1, _File);

}

