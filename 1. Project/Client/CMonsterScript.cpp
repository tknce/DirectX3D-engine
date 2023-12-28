#include "pch.h"
#include "CMonsterScript.h"

#include "CGameObject.h"


CMonsterScript::CMonsterScript()
	: m_pTargetObject(nullptr)
{
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::tick()
{	
	if (KEY_TAP(KEY::I))
	{
		Vec3 setScale = Transform()->GetRelativeScale();
		setScale += Vec3(5.f, 200.f, 0.f);
		Transform()->SetRelativeScale(setScale);
	}


	if (!IsValid(m_pTargetObject))
		return;
	
	
}