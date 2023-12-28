#include "pch.h"
#include "CGunsparkScript.h"
#include <Engine/CAnimation2D.h>

CGunsparkScript::CGunsparkScript()
	:CScript(GUNSPARKSCRIPT)
{
}

CGunsparkScript::~CGunsparkScript()
{
}

void CGunsparkScript::begin()
{
	m_pAni = Animator2D()->Play_Rotation(L"GUNSPARK_1", false);
}

void CGunsparkScript::tick()
{
	// 오른쪽 모션 + x65,y10
	if (Animator2D() != nullptr)
	{
		m_pAni->GetFinish();
	}	
}



