#include "pch.h"
#include "CCollisionMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CCollider2D.h"


CCollisionMgr::CCollisionMgr()
	: m_matrix{}
{

}

CCollisionMgr::~CCollisionMgr()
{

}

void CCollisionMgr::tick()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (int iRow = 0; iRow < MAX_LAYER; ++iRow)
	{
		for (int iCol = iRow; iCol < MAX_LAYER; ++iCol)
		{
			if (m_matrix[iRow] & (1 << iCol))
			{
				CollisionBtwLayer(pCurLevel, iRow, iCol);
			}
		}
	}
}


void CCollisionMgr::CollisionBtwLayer(CLevel* _pCurLevel, int _iLeft, int _iRight)
{
	const vector<CGameObject*>& vecLeft = _pCurLevel->GetLayer(_iLeft)->GetObjects();
	const vector<CGameObject*>& vecRight = _pCurLevel->GetLayer(_iRight)->GetObjects();

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		if (nullptr == vecLeft[i]->Collider2D())
			continue;

		size_t j = 0;
		if (_iLeft == _iRight)
		{
			j = i + 1;
		}

		for (; j < vecRight.size(); ++j)
		{
			if (nullptr == vecRight[j]->Collider2D())
			{
				continue;
			}

			CollisionBtwCollider(vecLeft[i]->Collider2D(), vecRight[j]->Collider2D());
		}
	}
}

void CCollisionMgr::CollisionBtwCollider(CCollider2D* _pLeft, CCollider2D* _pRight)
{
	// �� �浹ü�� ID �� Ȯ��
	COLLIDER_ID id;
	id.LEFT_ID = _pLeft->GetID();
	id.RIGHT_ID = _pRight->GetID();

	// ���� �浹 ������ �˻��Ѵ�.
	map<ULONGLONG, bool>::iterator iter = m_mapColInfo.find(id.ID);

	// �浹������ �ƿ� ������ ������ش�.
	if (iter == m_mapColInfo.end())
	{
		m_mapColInfo.insert(make_pair(id.ID, false));
		iter = m_mapColInfo.find(id.ID);
	}

	// �� �浹ü�� �ϳ� �̻��� Dead ��������
	bool IsDead = _pLeft->GetOwner()->IsDead() || _pRight->GetOwner()->IsDead();


	// ���� �浹������ �˻� �Ѵ�.			
	if (IsCollision(_pLeft, _pRight))
	{
		// �浹 ���̴�.
		if (false == iter->second)
		{
			// ���� �����ӿ��� �浹���� �ʾҴ�.
			// ��--> �̹� �����ӿ� �浹 ����
			if (!IsDead)
			{
				_pLeft->BeginOverlap(_pRight);
				_pRight->BeginOverlap(_pLeft);
				iter->second = true;
			}
		}

		// �浹 ��
		else
		{
			// ���� ������ ���, �浹�� �����ϴ� �������� ����
			if (IsDead)
			{
				_pLeft->EndOverlap(_pRight);
				_pRight->EndOverlap(_pLeft);
				iter->second = false;
			}

			// �������� �浹, ���ݵ� �浹 ��
			else
			{
				_pLeft->Overlap(_pRight);
				_pRight->Overlap(_pLeft);
			}
		}
	}

	else
	{
		// �浹 X
		if (iter->second)
		{
			// �������� �浹�ϰ� �־���
			// ��--> �浹�� �� ��� ����
			_pLeft->EndOverlap(_pRight);
			_pRight->EndOverlap(_pLeft);
			iter->second = false;
		}
	}
}

bool CCollisionMgr::IsCollision(CCollider2D* _pLeft, CCollider2D* _pRight)
{
	// 0 --- 1
	// |  \  | 
	// 3 --- 2	
	static const Vec3 arrLocalPos[4]
		=
	{
		Vec3(-0.5f, 0.5f, 0.f),
		Vec3(0.5f, 0.5f, 0.f),
		Vec3(0.5f, -0.5f, 0.f),
		Vec3(-0.5f, -0.5f, 0.f)
	};


	// �и��� ���ϱ�
	Vec3 vAxis[4] = {};

	const Matrix& matLeft = _pLeft->GetWorldMat();
	const Matrix& matRight = _pRight->GetWorldMat();

	// �и��� ���� == ��������
	vAxis[0] = XMVector3TransformCoord(arrLocalPos[1], matLeft) - XMVector3TransformCoord(arrLocalPos[0], matLeft);
	vAxis[1] = XMVector3TransformCoord(arrLocalPos[3], matLeft) - XMVector3TransformCoord(arrLocalPos[0], matLeft);
	vAxis[2] = XMVector3TransformCoord(arrLocalPos[1], matRight) - XMVector3TransformCoord(arrLocalPos[0], matRight);
	vAxis[3] = XMVector3TransformCoord(arrLocalPos[3], matRight) - XMVector3TransformCoord(arrLocalPos[0], matRight);

	for (int i = 0; i < 4; ++i)
		vAxis[i].z = 0.f;
	

	Vec2 vC = _pLeft->GetFinalPos() - _pRight->GetFinalPos();
	Vec3 vCenterDir = Vec3(vC.x, vC.y, 0.f);


	for (int i = 0; i < 4; ++i)
	{
		Vec3 vA = vAxis[i];
		vA.Normalize();

		float fProjDist = 0.f;
		for (int j = 0; j < 4; ++j)
		{
			fProjDist += fabsf(vAxis[j].Dot(vA)) / 2.f;
		}

		if (fProjDist < fabsf(vCenterDir.Dot(vA)))
		{
			return false;
		}
	}

	return true;
}


void CCollisionMgr::CollisionLayerCheck(int _iLeft, int _iRight)
{
	// �� ���� ���ڸ� ��(�迭�� �ε���) ����, �� ū ���ڸ� ��(��Ʈ ��ġ) ��
	UINT iRow = 0, iCol = 0;

	if (_iLeft <= _iRight)
	{
		iRow = _iLeft;
		iCol = _iRight;
	}
	else
	{
		iRow = _iRight;
		iCol = _iLeft;
	}

	m_matrix[iRow] |= (1 << iCol);
}

void CCollisionMgr::CollisionLayerRelease(int _iLeft, int _iRight)
{
	// �� ���� ���ڸ� ��(�迭�� �ε���) ����, �� ū ���ڸ� ��(��Ʈ ��ġ) ��
	UINT iRow = 0, iCol = 0;

	if (_iLeft <= _iRight)
	{
		iRow = _iLeft;
		iCol = _iRight;
	}
	else
	{
		iRow = _iRight;
		iCol = _iLeft;
	}

	m_matrix[iRow] &= ~(1 << iCol);
}
