#include "pch.h"
#include "CRes.h"

CRes::CRes(RES_TYPE _eResType, bool _bEngineRes)
	: m_eResType(_eResType)
	, m_iRefCount(0)
	, m_bEngineRes(_bEngineRes)
{
}

CRes::CRes(const CRes& _other)
	: CEntity(_other)
	, m_eResType(_other.m_eResType)
	, m_iRefCount(0)
	, m_bEngineRes(false)
{
}

CRes::~CRes()
{
	this;
	assert(!(0 < m_iRefCount));
}

bool CRes::CheckRelativePath(const wstring& _strRelativePath)
{
	// ���� �����̸�, �ش� ��θ� ����
	if (m_strRelativePath.empty())
	{
		m_strRelativePath = _strRelativePath;
		return true;
	}
	// �ѹ� �̻� ���Ͽ� ����� ���� ������
	else
	{
		// ���� ��ο� �� ������ ����
		if (m_strRelativePath == _strRelativePath)
			return true;
		else
			return false;
	}
}

void CRes::SaveKeyPath(FILE* _pFile)
{
	SaveWStringToFile(m_strKey, _pFile);
	SaveWStringToFile(m_strRelativePath, _pFile);
}

void CRes::LoadKeyPath(FILE* _pFile)
{
	LoadWStringFromFile(m_strKey, _pFile);
	LoadWStringFromFile(m_strRelativePath, _pFile);
}

