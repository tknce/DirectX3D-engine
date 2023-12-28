#include "pch.h"
#include "CParticleUpdateShader.h"

#include "CDevice.h"
#include "CStructuredBuffer.h"

CParticleUpdateShader::CParticleUpdateShader()
	: CComputeShader(128, 1, 1)
	, m_ParticleBuffer(nullptr)
{
}

CParticleUpdateShader::CParticleUpdateShader(const CParticleUpdateShader& _origin)
	: CComputeShader(128, 1, 1)
	, m_ParticleBuffer(nullptr)
	, m_ParticleShare(nullptr)
	
{
	//m_ParticleBuffer = _origin.m_ParticleBuffer->Clone();
	//m_ParticleShare = _origin.m_ParticleShare->Clone();
}

CParticleUpdateShader::~CParticleUpdateShader()
{
}


void CParticleUpdateShader::SetParticleBuffer(CStructuredBuffer* _Buffer)
{
	assert(_Buffer);

	m_ParticleBuffer = _Buffer;
	m_Param.iArr[0] = m_ParticleBuffer->GetElementCount();
}

void CParticleUpdateShader::UpdateData()
{
	assert(m_ParticleBuffer);
	assert(m_ParticleShare);

	m_ParticleBuffer->UpdateData_CS(0, false);
	m_ParticleShare->UpdateData_CS(1, false);



	m_iGroupX = m_Param.iArr[0] / m_iGroupPerThreadX + 1;
	m_iGroupY = 1;
	m_iGroupZ = 1;
}

void CParticleUpdateShader::Clear()
{
	m_ParticleBuffer->Clear();
	m_ParticleShare->Clear();
}
