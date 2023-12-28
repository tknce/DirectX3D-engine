#include "pch.h"
#include "CCopyBoneShader.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

CCopyBoneShader::CCopyBoneShader()
	: CComputeShader(1024, 1, 1)
	, m_pSrcBuffer(nullptr)
	, m_pDestBuffer(nullptr)
{
}

CCopyBoneShader::~CCopyBoneShader()
{
}

void CCopyBoneShader::UpdateData()
{
	// ����ȭ���� ����
	m_pSrcBuffer->UpdateData_CS(16, true);  // t16
	m_pDestBuffer->UpdateData_CS(0, false);	// u0

	// �׷� �� ���
	int iBoneCount = m_Param.iArr[0];

	m_iGroupX = iBoneCount / m_iGroupPerThreadX + 1;
	m_iGroupY = 1;
	m_iGroupZ = 1;
}

void CCopyBoneShader::Clear()
{
	// ������ ����ȭ���� Ŭ����	
	m_pSrcBuffer->Clear();
	m_pDestBuffer->Clear();
}