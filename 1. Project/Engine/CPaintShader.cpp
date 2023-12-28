#include "pch.h"
#include "CPaintShader.h"

CPaintShader::CPaintShader()
	: CComputeShader(32, 32, 1)
{
}

CPaintShader::~CPaintShader()
{
}

void CPaintShader::UpdateData()
{
	// Target Textrue �� U0 �� ���ε�
	m_pTargetTex->UpdateData_CS(0, false);

	// �׷� �� ���
	m_iGroupX = m_pTargetTex->GetWidth() / m_iGroupPerThreadX + 1;
	m_iGroupY = m_pTargetTex->GetHeight() / m_iGroupPerThreadY + 1;
	m_iGroupZ = 1;
}

void CPaintShader::Clear()
{
	CTexture::Clear_CS(0);
}
