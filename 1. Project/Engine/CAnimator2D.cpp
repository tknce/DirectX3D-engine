#include "pch.h"
#include "CAnimator2D.h"
#include "CAnimation2D.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CTexture.h"



CAnimator2D::CAnimator2D()
	: CComponent(COMPONENT_TYPE::ANIMATOR2D)
	, m_bRepeat(false)
{
}

CAnimator2D::CAnimator2D(const CAnimator2D& _origin)
	: CComponent(COMPONENT_TYPE::ANIMATOR2D)
	, m_bRepeat(_origin.m_bRepeat)
{
	map<wstring, CAnimation2D*>::const_iterator iter = _origin.m_mapAnim.begin();
	for (; iter != _origin.m_mapAnim.end(); ++iter)
	{
		wstring first = iter->first;
		CAnimation2D* ani = iter->second->Clone();
		m_mapAnim.insert(make_pair(first, ani));
		if (_origin.m_pCurAnim == iter->second)
		{
			Play(first, true);
		}
	}
}

CAnimator2D::~CAnimator2D()
{
	Safe_Del_Map(m_mapAnim);
}

void CAnimator2D::finaltick()
{
	if (!IsValid(m_pCurAnim))
		return;

	// Animation 이 Finish 상태이고, 반복재생을 하기로 한 경우
	if (m_pCurAnim->m_bFinish && m_bRepeat)
	{
		// 다시 0 프레임으로 리셋시켜서 동작하게 한다.
		m_pCurAnim->Reset();
	}

	// Animation 업데이트
	m_pCurAnim->finaltick();
}

void CAnimator2D::CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS)
{
	assert(_AtlasTex.Get());

	CAnimation2D* pAnim = FindAnimation(_strKey);
	assert(!pAnim);

	pAnim = new CAnimation2D;
	pAnim->Create(_strKey, _AtlasTex, _vLeftTop, _vSlice, _fStep, _iMaxFrm, _FPS);

	pAnim->m_pOwner = this;
	m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
}

CAnimation2D* CAnimator2D::FindAnimation(const wstring& _strKey)
{
	map<wstring, CAnimation2D*>::iterator iter = m_mapAnim.find(_strKey);

	if (iter == m_mapAnim.end())
		return nullptr;

	return iter->second;
}


CAnimation2D* CAnimator2D::Play(const wstring& _strKey, bool _bRepeat, bool _bReverse)
{
	CAnimation2D* pAnim = FindAnimation(_strKey);

	if (IsValid(pAnim))
	{
		m_pCurAnim = pAnim;
	}
	/*if (_bReverse)
	{
		m_fPI = XM_PI;
		GetOwner()->Transform()->SetRelativeRotation(0.f,XM_PI,0.f);
	}
	else
	{
		m_fPI = 0;
		GetOwner()->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	}*/
	// 반복재생
	m_bRepeat = _bRepeat;

	// 지정된 Animation 을 초기화
	//if(m_pCurAnim->GetFinish())
	m_pCurAnim->Reset();	
	return m_pCurAnim;
}

CAnimation2D* CAnimator2D::Play_Rotation(const wstring& _strKey, bool _bRepeat, bool _bReverse)
{
	CAnimation2D* pAnim = FindAnimation(_strKey);

	if (IsValid(pAnim))
	{
		m_pCurAnim = pAnim;
	}
	// 반복재생
	m_bRepeat = _bRepeat;

	// 지정된 Animation 을 초기화
	//if(m_pCurAnim->GetFinish())
	m_pCurAnim->Reset();
	return m_pCurAnim;
}

void CAnimator2D::Play_Load(const wstring& _strKey, int _idx)
{
	CAnimation2D* pAnim = FindAnimation(_strKey);

	if (IsValid(pAnim))
	{
		m_pCurAnim = pAnim;
	}
	m_pCurAnim->SetLoad(true);
	m_pCurAnim->SetIdx(_idx);
}

void CAnimator2D::UpdateData()
{
	if (!IsValid(m_pCurAnim))
		return;
	
	m_pCurAnim->UpdateData();
}

void CAnimator2D::Clear()
{
	if (!IsValid(m_pCurAnim))
		return;

	m_pCurAnim->Clear();
}


void CAnimator2D::ChangeName(wstring _Name, wstring _NewName)
{
	
	CAnimation2D* Ani = FindAnimation(_Name);
	Ani->SetName(_NewName);

	m_mapAnim.insert(make_pair(Ani->GetName(), Ani));
	m_mapAnim.erase(_Name);
}


void CAnimator2D::SaveToFile(FILE* _File)
{
	COMPONENT_TYPE type = GetType();
	fwrite(&type, sizeof(UINT), 1, _File);

	// Animation 저장
	size_t iAnimCount = m_mapAnim.size();
	fwrite(&iAnimCount, sizeof(size_t), 1, _File);

	for (const auto& pair : m_mapAnim)
	{
		pair.second->SaveToFile(_File);
	}
}

void CAnimator2D::LoadFromFile(FILE* _File)
{
	// Animation
	size_t iAnimCount = 0;
	fread(&iAnimCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < iAnimCount; ++i)
	{
		CAnimation2D* pAnim = new CAnimation2D;
		pAnim->LoadFromFile(_File);

		pAnim->m_pOwner = this;
		m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
	}
}



void CAnimator2D::TurnbacktimeSave(FILE* _File)
{
	SaveWStringToFile(m_pCurAnim->GetName(), _File);

	int IDX = m_pCurAnim->GetIDX();
	fwrite(&IDX, sizeof(int), 1, _File);

}

void CAnimator2D::TurnbacktimeLoad(FILE* _File)
{
	wstring AniName;
	LoadWStringFromFile(AniName, _File);
	int IDX;
	fread(&IDX, sizeof(int), 1, _File);

	Play_Load(AniName, IDX);
}