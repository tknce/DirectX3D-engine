#include "pch.h"
#include "CMeshRender.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CAnimator2D.h"
#include "CAnimator3D.h"

#include "CMesh.h"
#include "CMaterial.h"


CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)	
{
}

CMeshRender::~CMeshRender()
{
}


void CMeshRender::tick()
{
}

void CMeshRender::finaltick()
{
}

void CMeshRender::render()
{
	if (!IsActive())
		return;

	if (nullptr == GetMesh())
		return;

	// ��ġ���� ������Ʈ
	Transform()->UpdateData();

	// Animator2D ������Ʈ�� �ִٸ�
	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}


	// Animator3D ������Ʈ
	if (Animator3D())
	{
		Animator3D()->UpdateData();

		for (size_t i = 0; i < GetMtrlCount(); ++i)
		{
			if (nullptr == GetCurMaterial(i))
				continue;
			GetCurMaterial(i)->SetAnim3D(true); // Animation Mesh �˸���
			GetCurMaterial(i)->SetBoneCount(Animator3D()->GetBoneCount());
		}
	}



	// ����� ���� ������Ʈ
	UINT iSubsetCount = GetMesh()->GetSubsetCount();

	for (int i = 0; i < iSubsetCount; ++i)
	{
		if (nullptr != GetCurMaterial(i))
		{
			GetCurMaterial(i)->UpdateData();

			// ����� �޽� ������Ʈ �� ������
			GetMesh()->render(i);
		}
	}

	// ����� �ؽ��� �������� ����
	CMaterial::Clear();

	// Animator2D ������Ʈ�� �־�����, ���� �������� Clear
	if (Animator2D())
		Animator2D()->Clear();

	if (Animator3D())
		Animator3D()->ClearData();
}

void CMeshRender::render(UINT _iSubset)
{
	if (nullptr == GetMesh() || nullptr == GetCurMaterial(_iSubset))
		return;

	// ==========
	// UpdateData
	// ==========
	// Transform ������Ʈ
	Transform()->UpdateData();

	// Animation2D ������Ʈ
	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}

	// Animator3D ������Ʈ
	if (Animator3D())
	{
		Animator3D()->UpdateData();
		GetCurMaterial(_iSubset)->SetAnim3D(true); // Animation Mesh �˸���
		GetCurMaterial(_iSubset)->SetBoneCount(Animator3D()->GetBoneCount());
	}

	// ======
	// Render
	// ======
	GetCurMaterial(_iSubset)->UpdateData();
	GetMesh()->render(_iSubset);

	// =====
	// Clear
	// =====	
	if (Animator2D())
		Animator2D()->Clear();

	if (Animator3D())
		Animator3D()->ClearData();
}
